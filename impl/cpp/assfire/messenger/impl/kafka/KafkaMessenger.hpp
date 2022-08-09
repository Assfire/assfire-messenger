#pragma once

#include "KafkaChannelOptions.hpp"
#include "assfire/messenger/api/Messenger.hpp"

#include <memory>
#include <string>
#include <oneapi/tbb/concurrent_hash_map.h>

namespace assfire::messenger {
    class KafkaMessenger : public Messenger {
      public:
        KafkaMessenger();

        virtual std::shared_ptr<Publisher> create_publisher(const ChannelId& channel_id,
                                                            const PublisherOptions& options = PublisherOptions()) override;
        virtual std::shared_ptr<Consumer> create_consumer(const ChannelId& channel_id, const ConsumerOptions& options = ConsumerOptions()) override;

        void declare_channel(ChannelId channel_id, KafkaChannelOptions options);

      private:
        tbb::concurrent_hash_map<ChannelId, KafkaChannelOptions> _declared_channels;
        tbb::concurrent_hash_map<ChannelId, std::shared_ptr<Consumer>> _consumers;
    };
} // namespace assfire::messenger