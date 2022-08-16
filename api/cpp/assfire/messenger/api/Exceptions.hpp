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

    class AckFailedError : public ConsumerError {
      public:
        AckFailedError() : ConsumerError("Failed to ack message") {};
        AckFailedError(const std::string& what) : ConsumerError(what) {}
    };

    class ConsumerConstructionError : public ConsumerError {
      public:
        ConsumerConstructionError() : ConsumerError("Failed to create consumer") {};
        ConsumerConstructionError(const std::string& what) : ConsumerError(what) {};
    };

    class PublisherError : public std::runtime_error {
      public:
        PublisherError(const std::string& what) : std::runtime_error(what) {}
    };

    class PublisherConstructionError : public PublisherError {
      public:
        PublisherConstructionError() : PublisherError("Failed to create publisher") {};
        PublisherConstructionError(const std::string& what) : PublisherError(what) {};
    };

} // namespace assfire::messenger