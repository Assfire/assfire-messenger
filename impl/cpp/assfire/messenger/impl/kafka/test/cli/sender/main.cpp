#include "assfire/messenger/impl/kafka/KafkaMessenger.hpp"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <iostream>
#include <string>

using namespace assfire::messenger;

ABSL_FLAG(std::vector<std::string>, brokers, std::vector<std::string> {"localhost"}, "Comma-separated list of bootstrap servers");
ABSL_FLAG(std::string, message, "default_message", "Message to send to broker");
ABSL_FLAG(std::string, client_id, "client", "Client id to use for passing message");
ABSL_FLAG(std::string, topic, "pub1", "Topic to publish message to");

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);

    assfire::messenger::KafkaMessenger messenger;

    KafkaPublisherOptions options;
    options.set_bootstrap_servers(absl::GetFlag(FLAGS_brokers));
    options.set_client_id(absl::GetFlag(FLAGS_client_id));

    messenger.create_publisher(ChannelId(absl::GetFlag(FLAGS_topic)), options);

    auto publisher = messenger.get_publisher(ChannelId(absl::GetFlag(FLAGS_topic)));

    Message msg;
    msg.set_payload(pack(absl::GetFlag(FLAGS_message)));

    publisher->publish(msg);

    return 0;
}