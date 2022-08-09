#pragma once

#include "Message.hpp"

#include <chrono>
#include <functional>

namespace assfire::messenger {
    class Consumer {
      public:
        virtual ~Consumer()                                     = default;
        virtual Message poll()                                  = 0;
        virtual Message poll(std::chrono::milliseconds timeout) = 0;
        virtual void pause()                                    = 0;
        virtual void resume()                                   = 0;
        virtual void stop()                                     = 0;
        virtual void drain()                                    = 0;
        virtual void ack(const Message& msg)                    = 0;
    };
} // namespace assfire::messenger