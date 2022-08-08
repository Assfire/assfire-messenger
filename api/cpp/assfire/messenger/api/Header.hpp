#pragma once

#include <string>

namespace assfire::messenger {
    class Header {
      public:
        using Id    = std::string;
        using Value = std::string;

        Header() = default;
        Header(const Id& id, const Value& value) : _id(id), _value(value) {}
        Header(const Header& rhs) = default;
        Header(Header&& rhs)      = default;

        Header& operator=(const Header& rhs) = default;
        Header& operator=(Header&& rhs) = default;

        bool operator==(const Header& rhs) const = default;

        const Id& id() const {
            return _id;
        }

        const Value& value() const {
            return _value;
        }

      private:
        Id _id;
        Value _value;
    };
} // namespace assfire::messenger