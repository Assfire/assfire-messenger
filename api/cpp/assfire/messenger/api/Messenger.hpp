#pragma once

#include "ChannelId.hpp"
#include "Consumer.hpp"
#include "Options.hpp"
#include "Publisher.hpp"

#include <memory>

namespace assfire::messenger {
    class Messenger {
      public:
        virtual ~Messenger() = default;

        virtual std::shared_ptr<Publisher> create_publisher(const ChannelId& channel_id, const PublisherOptions& options = PublisherOptions()) = 0;
        virtual std::shared_ptr<Consumer> create_consumer(const ChannelId& channel_id, const ConsumerOptions& options = ConsumerOptions())     = 0;
    };
} // namespace assfire::messenger