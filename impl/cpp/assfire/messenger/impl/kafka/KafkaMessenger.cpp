#include "KafkaMessenger.hpp"

#include "KafkaConsumer.hpp"
#include "KafkaExceptions.hpp"
#include "assfire/logger/api/LoggerProvider.hpp"
#include "assfire/messenger/api/Exceptions.hpp"
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
                auto kafka_consumer = std::make_shared<kafka::clients::KafkaConsumer>(props);
                kafka_consumer->subscribe({options.topic_name()});

                write_accessor->second = std::make_shared<KafkaConsumer>(std::move(kafka_consumer), std::move(options));
            } else {
                if (write_accessor->second->options() != options) {
                    _logger->error("Trying to redeclare existing consumer channel {} (options = {}) with different options {} - this is not allowed",
                                   write_accessor->second->options().to_string(), options.to_string(), channel_id.name());
                    throw ChannelRedeclarationAttemptError(channel_id);
                } else {
                    _logger->info("Found existing consumer for channel {}. It will be reused", channel_id.name());
                }
            }
            return write_accessor->second;
        } catch (const ChannelRedeclarationAttemptError& e) { throw e; } catch (const std::exception& e) {
            _logger->error("Failed to create kafka consumer channel {}: {}", channel_id.name(), e.what());
            std::throw_with_nested(ConsumerConstructionError("Failed to create consumer for channel " + channel_id.name()));
        }
    }

    std::shared_ptr<KafkaPublisher> KafkaMessenger::create_publisher(ChannelId channel_id, KafkaPublisherOptions options) {
        try {
            _logger->info("Creating kafka publisher channel {} (options: {})", channel_id.name(), options.to_string());

            kafka::clients::producer::Config props = options.to_kafka_config();

            tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaPublisher>>::accessor write_accessor;
            bool is_new = _publishers.insert(write_accessor, channel_id);

            if (is_new) {
                write_accessor->second = std::make_shared<KafkaPublisher>(std::make_shared<kafka::clients::KafkaProducer>(props), std::move(options));
            } else {
                if (write_accessor->second->options() != options) {
                    _logger->error("Trying to redeclare existing publisher channel {} (options = {}) with different options {} - this is not allowed",
                                   write_accessor->second->options().to_string(), options.to_string(), channel_id.name());
                    throw ChannelRedeclarationAttemptError(channel_id);
                } else {
                    _logger->info("Found existing publisher for channel {}. It will be reused", channel_id.name());
                }
            }
            return write_accessor->second;
        } catch (const ChannelRedeclarationAttemptError& e) { throw e; } catch (const std::exception& e) {
            _logger->error("Failed to create kafka publisher channel {}: {}", channel_id.name(), e.what());
            std::throw_with_nested(PublisherConstructionError("Failed to create publisher for channel " + channel_id.name()));
        }
    }

    void KafkaMessenger::destroy_consumer(ChannelId channel_id) {
        _consumers.erase(channel_id);
    }

    void KafkaMessenger::destroy_publisher(ChannelId channel_id) {
        _publishers.erase(channel_id);
    }

} // namespace assfire::messenger