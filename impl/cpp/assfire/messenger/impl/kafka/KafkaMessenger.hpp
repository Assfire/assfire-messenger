#pragma once

#include "KafkaConsumer.hpp"
#include "KafkaConsumerOptions.hpp"
#include "assfire/logger/api/Logger.hpp"
#include "assfire/messenger/api/Messenger.hpp"

#include <memory>
#include <oneapi/tbb/concurrent_hash_map.h>
#include <string>

namespace assfire::messenger {
    class KafkaMessenger : public Messenger {
      public:
        KafkaMessenger();

        virtual std::shared_ptr<Publisher> get_publisher(const ChannelId& channel_id) override;
        virtual std::shared_ptr<Consumer> get_consumer(const ChannelId& channel_id) override;

        std::shared_ptr<KafkaConsumer> create_consumer(ChannelId channel_id, KafkaConsumerOptions options);

      private:
        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<KafkaConsumer>> _consumers;
        std::shared_ptr<logger::Logger> _logger;
    };
} // namespace assfire::messenger