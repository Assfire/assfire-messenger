#include "KafkaMessenger.hpp"

#include "KafkaConsumer.hpp"
#include "KafkaExceptions.hpp"
#include "assfire/logger/api/LoggerProvider.hpp"
#include "kafka/KafkaConsumer.h"

#include <memory>

namespace assfire::messenger {

    KafkaMessenger::KafkaMessenger() : _logger(logger::LoggerProvider::get("assfire.messenger.KafkaMessenger")) {}

    std::shared_ptr<Publisher> KafkaMessenger::get_publisher(const ChannelId& channel_id) {
        std::shared_ptr<Publisher> returnValue;
        return returnValue;
    }

    std::shared_ptr<Consumer> KafkaMessenger::get_consumer(const ChannelId& channel_id) {
        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaConsumer>>::const_accessor accessor;
        bool found_consumer = _consumers.find(accessor, channel_id);

        if (found_consumer) {
            return accessor->second;
        } else {
            _logger->error("Channel {} is not declared", channel_id.name());
            throw ChannelNotDeclaredError(channel_id);
        }
    }

    std::shared_ptr<KafkaConsumer> KafkaMessenger::create_consumer(ChannelId channel_id, KafkaConsumerOptions options) {
        _logger->info("Creating kafka consumer channel {} (options: {})", channel_id.name(), options.to_string());

        kafka::Properties props;

        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaConsumer>>::accessor write_accessor;
        bool is_new = _consumers.insert(write_accessor, channel_id);

        if (is_new) {
            write_accessor->second = std::shared_ptr<KafkaConsumer>(
                new KafkaConsumer(std::make_shared<kafka::clients::KafkaConsumer>(props), std::move(options)), [&channel_id, this](auto consumer) {
                    _consumers.erase(channel_id);
                    delete consumer;
                });
        } else {
            if (write_accessor->second->options() != options) {
                _logger->error("Trying to redeclare channel {} with different options - this is not allowed", channel_id.name());
                throw ChannelRedeclarationAttemptError(channel_id);
            }
        }
        return write_accessor->second;
    }

} // namespace assfire::messenger