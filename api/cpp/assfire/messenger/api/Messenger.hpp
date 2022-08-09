#pragma once

#include "ChannelId.hpp"
#include "Consumer.hpp"
#include "Publisher.hpp"

#include <memory>

namespace assfire::messenger {
    class Messenger {
      public:
        virtual ~Messenger() = default;

        virtual std::shared_ptr<Publisher> get_publisher(const ChannelId& channel_id) = 0;
        virtual std::shared_ptr<Consumer> get_consumer(const ChannelId& channel_id)   = 0;
    };
} // namespace assfire::messenger