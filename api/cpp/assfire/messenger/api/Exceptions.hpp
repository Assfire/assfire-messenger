#pragma once

#include "ChannelId.hpp"

#include <stdexcept>
#include <string>

namespace assfire::messenger {
    class ConsumerError : public std::runtime_error {
      public:
        ConsumerError(const std::string& what) : std::runtime_error(what) {}
    };

    class EndOfStreamError : public ConsumerError {
      public:
        EndOfStreamError() : ConsumerError("EOF on stream") {};
        EndOfStreamError(const std::string& what) : ConsumerError(what) {}
    };

    class TimeoutError : public ConsumerError {
      public:
        TimeoutError() : ConsumerError("Timeout on waiting for messages") {};
        TimeoutError(const std::string& what) : ConsumerError(what) {}
    };
    class ConsumerConstructionError : public ConsumerError {
      public:
        ConsumerConstructionError(const std::string& what) : ConsumerError(what) {}
    };
} // namespace assfire::messenger