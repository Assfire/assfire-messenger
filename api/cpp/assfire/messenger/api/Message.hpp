#pragma once

#include "Header.hpp"
#include "Payload.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace assfire::messenger {
    class Message {
      public:
        using Headers = std::unordered_map<Header::Id, Header>;

        Message() = default;
        Message(Headers headers, Payload payload) : _headers(std::move(headers)), _payload(std::move(payload)) {}
        explicit Message(Payload payload) : _payload(std::move(payload)) {}
        Message(const Message& rhs) = default;
        Message(Message&& rhs)      = default;

        Message& operator=(const Message& rhs) = default;
        Message& operator=(Message&& rhs) = default;

        bool operator==(const Message& rhs) const = default;

        void add_header(Header header) {
            _headers.emplace(header.id(), std::move(header));
        }

        void set_payload(Payload payload) {
            _payload = std::move(payload);
        }

        const Headers& headers() const {
            return _headers;
        }

        const Payload& payload() const {
            return _payload;
        }

      private:
        std::unordered_map<Header::Id, Header> _headers;
        Payload _payload;
    };
} // namespace assfire::messenger