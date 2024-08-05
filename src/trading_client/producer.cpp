#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstring>

#include "../common/kafka.cpp"
#include "Order.h"


/* Optional per-message delivery callback (triggered by poll() or flush())
 * when a message has been successfully delivered or permanently
 * failed delivery (after retries).
 */
static void dr_msg_cb (rd_kafka_t *kafka_handle,
                       const rd_kafka_message_t *rkmessage,
                       void *opaque) {
    if (rkmessage->err) {
        g_error("Message delivery failed: %s", rd_kafka_err2str(rkmessage->err));
    }
}


std::map<std::string, char*> read_config(const std::string &filename) {
    std::map<std::string, char*> config;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        auto delimiter_pos = line.find("=");
        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);
        char* value_cstr = new char[value.length() + 1];
        std::strcpy(value_cstr, value.c_str());
        config[key] = value_cstr;
    }
    return config;
}

int main (int argc, char **argv) {
    rd_kafka_t *producer;
    rd_kafka_conf_t *conf;
    char errstr[512];

    // Create client configuration
    conf = rd_kafka_conf_new();

    // User-specific properties that you must set
    std::map<std::string, char*> config = read_config("../common/kafka_config.txt");
    set_config(conf, "bootstrap.servers", config["bootstrap.servers"]);
    set_config(conf, "sasl.username", config["sasl.username"]);
    set_config(conf, "sasl.password", config["sasl.password"]);

    // Fixed properties
    set_config(conf, "security.protocol", "SASL_SSL");
    set_config(conf, "sasl.mechanisms",   "PLAIN");
    set_config(conf, "acks",              "all");

    // Install a delivery-error callback.
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // Create the Producer instance.
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        g_error("Failed to create new producer: %s", errstr);
        return 1;
    }

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = NULL;

    int message_count = 1;
    const char *topic = "orders";
    Order testOrder("1", "cust123", 100.5, 10, BUY);
    std::string serializedOrder = testOrder.serializeOrder();
    const char *key = testOrder.id.c_str();
    const char *value = serializedOrder.c_str();
    size_t key_len = strlen(key);
    size_t value_len = serializedOrder.length();

    rd_kafka_resp_err_t err;

    err = rd_kafka_producev(producer,
                            RD_KAFKA_V_TOPIC(topic),
                            RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                            RD_KAFKA_V_KEY((void*)key, key_len),
                            RD_KAFKA_V_VALUE((void*)value, value_len),
                            RD_KAFKA_V_OPAQUE(NULL),
                            RD_KAFKA_V_END);

    if (err) {
        g_error("Failed to produce to topic %s: %s", topic, rd_kafka_err2str(err));
        return 1;
    } else {
        g_message("Produced event to topic %s: key = %12s value = %12s", topic, key, value);
    }

    rd_kafka_poll(producer, 0);

    // Block until the messages are all sent.
    g_message("Flushing final messages..");
    rd_kafka_flush(producer, 10 * 1000);

    if (rd_kafka_outq_len(producer) > 0) {
        g_error("%d message(s) were not delivered", rd_kafka_outq_len(producer));
    }

    g_message("%d events were produced to topic %s.", message_count, topic);

    rd_kafka_destroy(producer);

    return 0;
}
