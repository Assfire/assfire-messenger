#include "KafkaMessenger.hpp"

#include "KafkaConsumer.hpp"
#include "kafka/KafkaConsumer.h"

#include <memory>

namespace assfire::messenger {

    KafkaMessenger::KafkaMessenger() {}

    std::shared_ptr<Publisher> KafkaMessenger::create_publisher(const ChannelId& channel_id, const PublisherOptions& options) {
        std::shared_ptr<Publisher> returnValue;
        return returnValue;
    }

    std::shared_ptr<Consumer> KafkaMessenger::create_consumer(const ChannelId& channel_id, const ConsumerOptions& options) {
        std::shared_ptr<Consumer> result;

        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<Consumer>>::const_accessor accessor;
        bool found_consumer = _consumers.find(accessor, channel_id);

        if (found_consumer) {
            return accessor->second;
        } else {
            kafka::Properties props;

            tbb::concurrent_hash_map<ChannelId, std::shared_ptr<Consumer>>::accessor write_accessor;
            bool is_new = _consumers.insert(write_accessor, channel_id);

            if (is_new) {
                write_accessor->second = std::shared_ptr<Consumer>(new KafkaConsumer(std::make_shared<kafka::clients::KafkaConsumer>(props)),
                                                                   [&channel_id, this](auto consumer) {
                                                                       _consumers.erase(channel_id);
                                                                       delete consumer;
                                                                   });
            }

            return write_accessor->second;
        }
    }

    void KafkaMessenger::declare_channel(ChannelId channel_id, KafkaChannelOptions options) {
        _declared_channels.emplace(std::move(channel_id), std::move(options));
    }

} // namespace assfire::messenger