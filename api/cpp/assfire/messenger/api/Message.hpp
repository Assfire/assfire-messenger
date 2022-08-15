#pragma once

#include "Header.hpp"
#include "Payload.hpp"

#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>

namespace assfire::messenger {
    class Message {
      public:
        using Headers = std::unordered_map<Header::Id, Header>;
        using Id      = std::uint64_t;

        constexpr static Id UNSPECIFIED_ID = std::numeric_limits<Id>::max();

        Message() = default;
        Message(Headers headers, Payload payload) : _id(UNSPECIFIED_ID), _headers(std::move(headers)), _payload(std::move(payload)) {}
        Message(Id id, Headers headers, Payload payload) : _id(id), _headers(std::move(headers)), _payload(std::move(payload)) {}
        explicit Message(Payload payload) : _id(UNSPECIFIED_ID), _payload(std::move(payload)) {}
        explicit Message(Id id, Payload payload) : _id(id), _payload(std::move(payload)) {}
        template<ProtoMessage T>
        explicit Message(const T msg) : _id(UNSPECIFIED_ID),
                                        _payload(pack(msg)) {};
        template<ProtoMessage T>
        explicit Message(Id id, const T msg) : _id(id),
                                               _payload(pack(msg)) {};
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

        void set_id(Id id) {
            _id = id;
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

        Id id() const {
            return _id;
        }

      private:
        Id _id;
        std::unordered_map<Header::Id, Header> _headers;
        Payload _payload;
    };
} // namespace assfire::messenger