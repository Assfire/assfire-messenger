#pragma once

#include "Message.hpp"

#include <functional>

namespace assfire::messenger {
    class Consumer {
      public:
        virtual ~Consumer()                           = default;
        virtual std::optional<Message> next_message() = 0;
        virtual void interrupt()                      = 0;
    };
} // namespace assfire::messenger