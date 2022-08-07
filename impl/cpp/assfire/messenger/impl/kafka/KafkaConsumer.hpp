#pragma once

#include "assfire/messenger/api/Consumer.hpp"

#include <functional>

namespace assfire::messenger {
    class KafkaConsumer : public Consumer {
      public:
        virtual std::optional<Message> next_message() override;
        virtual void stop() override;
    };
} // namespace assfire::messenger