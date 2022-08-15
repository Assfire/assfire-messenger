#include "assfire/messenger/impl/kafka/KafkaMessenger.hpp"

#include <iostream>
#include <string>

using namespace assfire::messenger;

int main() {
    int id = 0;

    assfire::messenger::KafkaMessenger messenger;

    KafkaPublisherOptions options;
    options.set_bootstrap_servers(std::vector<std::string> {"localhost"});
    options.set_client_id("client");

    messenger.create_publisher(ChannelId("pub1"), options);

    while (true) {
        std::cout << ">" << std::endl;
        std::string line;
        std::getline(std::cin, line);

        if (line == "q") break;

        auto publisher = messenger.get_publisher(ChannelId("pub1"));

        Message msg;
        msg.set_id(id++);
        msg.set_payload(pack(line));

        publisher->publish(msg);
    }

    return 0;
}