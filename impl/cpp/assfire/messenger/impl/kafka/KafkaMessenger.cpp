#include "KafkaMessenger.hpp"

#include "KafkaConsumer.hpp"
#include "KafkaExceptions.hpp"
#include "assfire/logger/api/LoggerProvider.hpp"
#include "kafka/KafkaConsumer.h"

#include <memory>

namespace assfire::messenger {

    KafkaMessenger::KafkaMessenger() : _logger(logger::LoggerProvider::get("assfire.messenger.KafkaMessenger")) {}

    std::shared_ptr<Publisher> KafkaMessenger::get_publisher(const ChannelId& channel_id) {
        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaPublisher>>::const_accessor accessor;
        bool found_publisher = _publishers.find(accessor, channel_id);

        if (found_publisher) {
            return accessor->second;
        } else {
            _logger->error("Publisher channel {} is not declared", channel_id.name());
            throw ChannelNotDeclaredError(channel_id);
        }
    }

    std::shared_ptr<Consumer> KafkaMessenger::get_consumer(const ChannelId& channel_id) {
        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaConsumer>>::const_accessor accessor;
        bool found_consumer = _consumers.find(accessor, channel_id);

        if (found_consumer) {
            return accessor->second;
        } else {
            _logger->error("Consumer channel {} is not declared", channel_id.name());
            throw ChannelNotDeclaredError(channel_id);
        }
    }

    std::shared_ptr<KafkaConsumer> KafkaMessenger::create_consumer(ChannelId channel_id, KafkaConsumerOptions options) {
        try {
            _logger->info("Creating kafka consumer channel {} (options: {})", channel_id.name(), options.to_string());

            kafka::clients::consumer::Config props = options.to_kafka_config();

            tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaConsumer>>::accessor write_accessor;
            bool is_new = _consumers.insert(write_accessor, channel_id);

            if (is_new) {
                write_accessor->second =
                    std::shared_ptr<KafkaConsumer>(new KafkaConsumer(std::make_shared<kafka::clients::KafkaConsumer>(props), std::move(options)),
                                                   [&channel_id, this](auto consumer) {
                                                       _consumers.erase(channel_id);
                                                       delete consumer;
                                                   });
            } else {
                if (write_accessor->second->options() != options) {
                    _logger->error("Trying to redeclare consumer channel {} with different options - this is not allowed", channel_id.name());
                    throw ChannelRedeclarationAttemptError(channel_id);
                }
            }
            return write_accessor->second;
        } catch (const std::exception& e) {
            _logger->error("Failed to create kafka consumer channel {}: {}", channel_id.name(), e.what());
            throw;
        }
    }

    std::shared_ptr<KafkaPublisher> KafkaMessenger::create_publisher(ChannelId channel_id, KafkaPublisherOptions options) {
        try {
            _logger->info("Creating kafka publisher channel {} (options: {})", channel_id.name(), options.to_string());

            kafka::clients::producer::Config props = options.to_kafka_config();

            tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaPublisher>>::accessor write_accessor;
            bool is_new = _publishers.insert(write_accessor, channel_id);

            if (is_new) {
                write_accessor->second =
                    std::shared_ptr<KafkaPublisher>(new KafkaPublisher(std::make_shared<kafka::clients::KafkaProducer>(props), std::move(options)),
                                                    [&channel_id, this](auto publisher) {
                                                        _publishers.erase(channel_id);
                                                        delete publisher;
                                                    });
            } else {
                if (write_accessor->second->options() != options) {
                    _logger->error("Trying to redeclare publisher channel {} with different options - this is not allowed", channel_id.name());
                    throw ChannelRedeclarationAttemptError(channel_id);
                }
            }
            return write_accessor->second;
        } catch (const std::exception& e) {
            _logger->error("Failed to create kafka publisher channel {}: {}", channel_id.name(), e.what());
            throw;
        }
    }

} // namespace assfire::messenger