#include <kafka/KafkaConsumer.h>
#include <kafka/KafkaProducer.h>
#include <fstream>
#include <signal.h>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <vector>

#include "Book.h"
#include "Match.h"
#include "Limit.h"
#include "Order.h"
#include "kafka.cpp"


std::atomic_bool running = {true};

void stopRunning(int sig) {
    if (sig != SIGINT) return;

    if (running) {
        running = false;
    } else {
        // Restore the signal handler, -- to avoid stuck with this handler
        signal(SIGINT, SIG_IGN); // NOLINT
    }
}

std::map<std::string, std::string> read_config(const std::string &filename) {
    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        auto delimiter_pos = line.find("=");
        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);
        config[key] = value;
    }
    return config;
}

int main() {
    using namespace kafka;
    using namespace kafka::clients::consumer;
    using namespace kafka::clients::producer;


    signal(SIGINT, stopRunning);
    std::map<std::string, std::string> config = read_config("../kafka_config.txt");

    const Topic trades_topic = "trades";
    const Topic matches_topic = "matches";

    Properties props;
    props.put("bootstrap.servers", config["bootstrap.servers"]);
    props.put("sasl.username", config["sasl.username"]);
    props.put("sasl.password", config["sasl.password"]);
    props.put("security.protocol", "SASL_SSL");
    props.put("sasl.mechanisms", "PLAIN");
    props.put("group.id", "kafkac-getting-started");
    props.put("auto.offset.reset", "earliest");

    KafkaConsumer consumer(props);
    KafkaProducer producer(props);
    consumer.subscribe({trades_topic});

    Book* book = new Book();

    // delivery callback
    auto deliveryCb = [](const RecordMetadata& metadata, const Error& error) {
        if (!error) {
            std::cout << "Message delivered: " << metadata.toString() << std::endl;
        } else {
            std::cerr << "Message failed to be delivered: " << error.message() << std::endl;
        }
    };


    while (running) {
        // Poll messages from Kafka brokers
        auto records = consumer.poll(std::chrono::milliseconds(100));

        for (const auto& record: records) {
            if (!record.error()) {
                try {
                    Order* order = Order::deserializeOrder(record.value().toString());
                    std::vector<Match> matches = book->new_order(order);
                    for (auto matching : matches) {
                        std::string match_message = matching.serializeMatch();
                        ProducerRecord record(matches_topic, NullKey, Value(match_message.c_str(), match_message.size()));
                        producer.send(record, deliveryCb, KafkaProducer::SendOption::ToCopyRecordValue);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Failed to deserialize order: " << e.what() << std::endl;
                }
            } else {
                std::cerr << record.toString() << std::endl;
            }
        }
    }

    producer.close();
    consumer.close();
    book->print_book();

    delete book;
    return 0;
}
