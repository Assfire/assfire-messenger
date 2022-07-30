#pragma once

#include "Message.hpp"

namespace assfire::messenger {
    class Publisher {
      public:
        virtual ~Publisher()                     = default;
        virtual void publish(const Message& msg) = 0;
    };
} // namespace assfire::messenger