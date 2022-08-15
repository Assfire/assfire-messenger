#include "absl/strings/str_split.h"
#include "assfire/logger/impl/spdlog/SpdlogLoggerFactory.hpp"
#include "assfire/messenger/impl/kafka/KafkaMessageHeaders.hpp"
#include "assfire/messenger/impl/kafka/KafkaMessenger.hpp"

#include <gtest/gtest.h>
#include <librdkafka/rdkafka_mock.h>

using namespace assfire::messenger;
using namespace testing;
using namespace std::chrono_literals;

using KafkaMessage = assfire::messenger::Message;

class KafkaMessengerTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() {
        assfire::logger::SpdlogLoggerFactory::register_static_factory();
    }

    static void TearDownTestCase() {}

    void SetUp() override {
        char errstr[256];
        rd_kafka_conf_t* conf = rd_kafka_conf_new();
        _kafka_instance       = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
        EXPECT_NE(nullptr, _kafka_instance);

        _mock_cluster = rd_kafka_mock_cluster_new(_kafka_instance, 3);
        EXPECT_NE(nullptr, _mock_cluster);

        rd_kafka_topic_conf_t* tconf = rd_kafka_topic_conf_new();
        _topic                       = rd_kafka_topic_new(_kafka_instance, "topic1", tconf);

        _servers = absl::StrSplit(rd_kafka_mock_cluster_bootstraps(_mock_cluster), ",");
    }

    void TearDown() override {
        rd_kafka_topic_destroy(_topic);
        rd_kafka_mock_cluster_destroy(_mock_cluster);
        rd_kafka_destroy(_kafka_instance);
    }

    rd_kafka_topic_t* _topic;
    rd_kafka_t* _kafka_instance;
    rd_kafka_mock_cluster_t* _mock_cluster;
    std::vector<std::string> _servers;
};

TEST_F(KafkaMessengerTest, Messenger_MessagesAreSentAndReceivedOverTopic) {
    KafkaMessenger messenger;

    KafkaPublisherOptions publisher_opts;
    publisher_opts.set_bootstrap_servers(_servers);
    publisher_opts.set_topic_name("topic1");
    messenger.create_publisher(ChannelId("pub1"), publisher_opts);

    KafkaConsumerOptions consumer_opts;
    consumer_opts.set_bootstrap_servers(_servers);
    consumer_opts.set_topic_name("topic1");
    messenger.create_consumer(ChannelId("cons1"), consumer_opts);

    auto publisher = messenger.get_publisher(ChannelId("pub1"));
    auto consumer  = messenger.get_consumer(ChannelId("cons1"));

    KafkaMessage msg;
    msg.set_payload(pack("Test message"));

    publisher->publish(msg);

    KafkaMessage received_msg = consumer->poll(30s);

    EXPECT_EQ(received_msg.payload(), msg.payload());
    EXPECT_EQ(received_msg.id(), 0);
    EXPECT_EQ(received_msg.header(KAFKA_HEADER_TOPIC_NAME), "topic1");
    EXPECT_TRUE(received_msg.header(KAFKA_HEADER_TOPIC_PARTITION));
}
