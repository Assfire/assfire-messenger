#include "absl/strings/str_split.h"
#include "assfire/logger/impl/spdlog/SpdlogLoggerFactory.hpp"
#include "assfire/messenger/api/Exceptions.hpp"
#include "assfire/messenger/impl/kafka/KafkaExceptions.hpp"
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

    publisher->publish(KafkaMessage(pack("Test message 1")));
    publisher->publish(KafkaMessage(pack("Test message 2")));
    publisher->publish(KafkaMessage(pack("Test message 3")));

    KafkaMessage received_msg1 = consumer->poll(30s);
    KafkaMessage received_msg2 = consumer->poll(30s);
    KafkaMessage received_msg3 = consumer->poll(30s);

    std::unordered_set<std::string> messages;
    messages.emplace(to_string_view(received_msg1.payload()));
    messages.emplace(to_string_view(received_msg2.payload()));
    messages.emplace(to_string_view(received_msg3.payload()));

    EXPECT_TRUE(messages.contains("Test message 1"));
    EXPECT_TRUE(messages.contains("Test message 2"));
    EXPECT_TRUE(messages.contains("Test message 3"));

    EXPECT_EQ(received_msg1.header(KAFKA_HEADER_TOPIC_NAME), "topic1");
    EXPECT_TRUE(received_msg1.header(KAFKA_HEADER_OFFSET));
    EXPECT_TRUE(received_msg1.header(KAFKA_HEADER_TOPIC_PARTITION));

    EXPECT_EQ(received_msg2.header(KAFKA_HEADER_TOPIC_NAME), "topic1");
    EXPECT_TRUE(received_msg2.header(KAFKA_HEADER_OFFSET));
    EXPECT_TRUE(received_msg2.header(KAFKA_HEADER_TOPIC_PARTITION));

    EXPECT_EQ(received_msg3.header(KAFKA_HEADER_TOPIC_NAME), "topic1");
    EXPECT_TRUE(received_msg3.header(KAFKA_HEADER_OFFSET));
    EXPECT_TRUE(received_msg3.header(KAFKA_HEADER_TOPIC_PARTITION));
}

TEST_F(KafkaMessengerTest, Messenger_PollingIsInterruptedOnTimeout) {
    KafkaMessenger messenger;

    KafkaConsumerOptions consumer_opts;
    consumer_opts.set_bootstrap_servers(_servers);
    consumer_opts.set_topic_name("topic1");
    messenger.create_consumer(ChannelId("cons1"), consumer_opts);

    auto consumer = messenger.get_consumer(ChannelId("cons1"));

    EXPECT_THROW(consumer->poll(5s), TimeoutError);
}

TEST_F(KafkaMessengerTest, Messenger_RedeclarationOfChannelWithDifferentOptionsIsNotAllowed) {
    KafkaMessenger messenger;

    KafkaPublisherOptions publisher_opts;
    publisher_opts.set_bootstrap_servers(_servers);
    publisher_opts.set_topic_name("topic1");
    auto publisher = messenger.create_publisher(ChannelId("pub1"), publisher_opts);

    KafkaConsumerOptions consumer_opts;
    consumer_opts.set_bootstrap_servers(_servers);
    consumer_opts.set_topic_name("topic1");
    auto consumer = messenger.create_consumer(ChannelId("cons1"), consumer_opts);

    publisher_opts.set_batch_size(8);
    consumer_opts.set_group_id("some");

    EXPECT_THROW(messenger.create_publisher(ChannelId("pub1"), publisher_opts), ChannelRedeclarationAttemptError);
    EXPECT_THROW(messenger.create_consumer(ChannelId("cons1"), consumer_opts), ChannelRedeclarationAttemptError);

    // 1 held by messenger and 1 held by local var
    EXPECT_EQ(consumer.use_count(), 2);
    EXPECT_EQ(publisher.use_count(), 2);
}

TEST_F(KafkaMessengerTest, Messenger_ChannelIsReusedOnTryingToCreateWithSameOptions) {
    KafkaMessenger messenger;

    KafkaPublisherOptions publisher_opts;
    publisher_opts.set_bootstrap_servers(_servers);
    publisher_opts.set_topic_name("topic1");
    auto publisher = messenger.create_publisher(ChannelId("pub1"), publisher_opts);

    KafkaConsumerOptions consumer_opts;
    consumer_opts.set_bootstrap_servers(_servers);
    consumer_opts.set_topic_name("topic1");
    auto consumer = messenger.create_consumer(ChannelId("cons1"), consumer_opts);

    auto publisher2 = messenger.create_publisher(ChannelId("pub1"), publisher_opts);
    auto consumer2  = messenger.create_consumer(ChannelId("cons1"), consumer_opts);

    EXPECT_EQ(consumer.use_count(), 3);
    EXPECT_EQ(publisher.use_count(), 3);
    EXPECT_EQ(consumer2.use_count(), 3);
    EXPECT_EQ(publisher2.use_count(), 3);

    EXPECT_EQ(publisher.get(), publisher2.get());
    EXPECT_EQ(consumer.get(), consumer2.get());
}

