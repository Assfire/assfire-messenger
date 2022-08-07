#pragma once

#include "assfire/messenger/api/Messenger.hpp"

#include <memory>

namespace assfire::messenger {
    class KafkaMessenger : public Messenger {
      public:
        virtual std::shared_ptr<Publisher> create_publisher(const ChannelId& channel_id,
                                                            const PublisherOptions& options = PublisherOptions()) override;
        virtual std::shared_ptr<Consumer> create_consumer(const ChannelId& channel_id, const ConsumerOptions& options = ConsumerOptions()) override;
    };
} // namespace assfire::messenger