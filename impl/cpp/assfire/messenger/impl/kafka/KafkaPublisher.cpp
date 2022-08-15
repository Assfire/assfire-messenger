#include "KafkaPublisher.hpp"

#include "assfire/logger/api/LoggerProvider.hpp"

namespace assfire::messenger {

    KafkaPublisher::KafkaPublisher(std::shared_ptr<kafka::clients::KafkaProducer> producer, KafkaPublisherOptions options)
        : _producer(std::move(producer)),
          _options(std::move(options)),
          _logger(logger::LoggerProvider::get("assfire.messenger.KafkaPublisher")) {}

    void KafkaPublisher::publish(const Message& msg) {
        auto record =
            kafka::clients::producer::ProducerRecord(_options.topic_name(), kafka::NullKey, kafka::Value(msg.payload().data(), msg.payload().size()));

        _producer->send(record, [this](const kafka::clients::producer::RecordMetadata& metadata, const kafka::Error& error) {
            if (error) { _logger->error("Message wasn't delivered to kafka: {}", metadata.toString()); }
        });
    }

} // namespace assfire::messenger