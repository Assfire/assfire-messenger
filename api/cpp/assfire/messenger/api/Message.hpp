#pragma once

#include "Header.hpp"
#include "Payload.hpp"

#include <absl/strings/str_join.h>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>

namespace assfire::messenger {
    class Message {
      public:
        using Headers = std::unordered_map<Header::Id, Header>;

        Message() = default;
        Message(Headers headers, Payload payload) : _headers(std::move(headers)), _payload(std::move(payload)) {}
        explicit Message(Payload payload) : _payload(std::move(payload)) {}
        template<ProtoMessage T>
        explicit Message(const T msg) : _payload(pack(msg)) {};
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

        std::optional<std::string> header(const Header::Id& id) const {
            auto iter = _headers.find(id);
            if (iter == _headers.end()) {
                return std::nullopt;
            } else {
                return iter->second.value();
            }
        }

        const Headers& headers() const {
            return _headers;
        }

        const Payload& payload() const {
            return _payload;
        }

        std::string headers_to_string() const {
            std::vector<std::string> headers;
            headers.reserve(_headers.size());
            for (const auto& h : _headers) {
                headers.push_back(h.second.to_string());
            }
            return "{" + absl::StrJoin(headers, ",") + "}";
        }

      private:
        std::unordered_map<Header::Id, Header> _headers;
        Payload _payload;
    };
} // namespace assfire::messenger