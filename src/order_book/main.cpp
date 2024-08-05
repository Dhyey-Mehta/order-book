#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstring>

#include "Book.h"
#include "../common/Order.h"
#include "Limit.h"
#include "../common/kafka.cpp"

static volatile sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop(int sig) {
    run = 0;
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

int main() {
    Book* book = new Book();

    rd_kafka_t *consumer;
    rd_kafka_conf_t *conf;
    rd_kafka_resp_err_t err;
    char errstr[512];

    // Create client configuration
    conf = rd_kafka_conf_new();

    std::map<std::string, char*> config = read_config("../common/kafka_config.txt");
    set_config(conf, "bootstrap.servers", config["bootstrap.servers"]);
    set_config(conf, "sasl.username", config["sasl.username"]);
    set_config(conf, "sasl.password", config["sasl.password"]);

    // Fixed properties
    set_config(conf, "security.protocol", "SASL_SSL");
    set_config(conf, "sasl.mechanisms", "PLAIN");
    set_config(conf, "group.id", "kafka-c-getting-started");
    set_config(conf, "auto.offset.reset", "earliest");

    // Create the Consumer instance.
    consumer = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
    if (!consumer) {
        std::cerr << "Failed to intialize consumer: " << errstr << std::endl;
        delete book;
        return 1;
    }
    rd_kafka_poll_set_consumer(consumer);

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = NULL;

    // Convert the list of topics to a format suitable for librdkafka.
    const char *topic = "orders";
    rd_kafka_topic_partition_list_t *subscription = rd_kafka_topic_partition_list_new(1);
    rd_kafka_topic_partition_list_add(subscription, topic, RD_KAFKA_PARTITION_UA);

    // Subscribe to the list of topics.
    err = rd_kafka_subscribe(consumer, subscription);
    if (err) {
        std::cerr << "Failed to subscribe to topics: " << rd_kafka_err2str(err) << std::endl;
        rd_kafka_topic_partition_list_destroy(subscription);
        rd_kafka_destroy(consumer);
        delete book;
        return 1;
    }

    rd_kafka_topic_partition_list_destroy(subscription);

    // Install a signal handler for clean shutdown.
    signal(SIGINT, stop);

    // Start polling for messages.
    while (run) {
        rd_kafka_message_t *consumer_message;

        consumer_message = rd_kafka_consumer_poll(consumer, 500);
        if (!consumer_message) {
            continue;
        }

        if (consumer_message->err) {
            if (consumer_message->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
                // We can ignore this error - it just means we've read
                // everything and are waiting for more data.
            } else {
                std::cerr << "Consumer error: " << rd_kafka_message_errstr(consumer_message) << std::endl;
                break;
            }
        } else {
            std::string payload(static_cast<char*>(consumer_message->payload), consumer_message->len);
            try {
                Order* order = Order::deserializeOrder(payload);
                book->new_order(order);
                // book->print_book();
            } catch (const std::exception& e) {
                std::cerr << "Failed to deserialize order: " << e.what() << std::endl;
            }
        }

        // Free the message when we're done.
        rd_kafka_message_destroy(consumer_message);
    }

    // Close the consumer: commit final offsets and leave the group.
    std::cout << "Closing consumer" << std::endl;
    rd_kafka_consumer_close(consumer);

    // Destroy the consumer.
    rd_kafka_destroy(consumer);

    book->print_book();

    delete book;
    return 0;
}
