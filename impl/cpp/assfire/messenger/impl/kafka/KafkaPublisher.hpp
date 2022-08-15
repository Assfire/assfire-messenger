#pragma once

#include "KafkaPublisherOptions.hpp"
#include "assfire/logger/api/Logger.hpp"
#include "assfire/messenger/api/Publisher.hpp"

#include <kafka/KafkaProducer.h>
#include <memory>

namespace assfire::messenger {
    class KafkaPublisher : public Publisher {
      public:
        KafkaPublisher(std::shared_ptr<kafka::clients::KafkaProducer> producer, KafkaPublisherOptions options);

        virtual void publish(const Message& msg) override;

        const KafkaPublisherOptions& options() const {
            return _options;
        }

      private:
        std::shared_ptr<kafka::clients::KafkaProducer> _producer;
        KafkaPublisherOptions _options;
        std::shared_ptr<logger::Logger> _logger;
    };
} // namespace assfire::messenger