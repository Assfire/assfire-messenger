#pragma once

#include "Header.hpp"
#include "Payload.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace assfire::messenger {
    class Message {
      public:
        void add_header(Header header) {
            _headers.emplace(header.id(), std::move(header));
        }

        void set_payload(Payload payload) {
            _payload = std::move(payload);
        }

        const std::unordered_map<Header::Id, Header> headers() const {
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