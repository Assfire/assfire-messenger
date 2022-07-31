#pragma once

#include "Message.hpp"

#include <functional>

namespace assfire::messenger {
    class Consumer {
      public:
        virtual ~Consumer()                                                    = default;
        virtual std::optional<Message> next_message()                          = 0;
        virtual void consume_loop(std::function<bool(const Message&)> consume) = 0;
    };
} // namespace assfire::messenger