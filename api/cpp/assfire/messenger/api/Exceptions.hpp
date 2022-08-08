#pragma once

#include <stdexcept>

namespace assfire::messenger {
    class ConsumerError : public std::runtime_error {
      public:
        ConsumerError(const char* what) : std::runtime_error(what) {}
    };

    class EndOfStreamError : public ConsumerError {
      public:
        EndOfStreamError() : ConsumerError("EOF on stream") {};
        EndOfStreamError(const char* what) : ConsumerError(what) {}
    };

    class TimeoutError : public ConsumerError {
      public:
        TimeoutError() : ConsumerError("Timeout on waiting for messages") {};
        TimeoutError(const char* what) : ConsumerError(what) {}
    };
    class ConsumerConstructionError : public ConsumerError {
      public:
        ConsumerConstructionError(const char* what) : ConsumerError(what) {}
    };
} // namespace assfire::messenger