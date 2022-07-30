#pragma once

#include <cstdint>
#include <string>

namespace assfire::messenger {
    class Payload {
      public:
        Payload() = default;
        Payload(uint8_t* buf, size_t size) : _data(buf, size) {}
        Payload(const Payload& rhs) = default;
        Payload(Payload&& rhs)      = default;

        Payload& operator=(const Payload& rhs) = default;
        Payload& operator=(Payload&& rhs) = default;

        bool operator==(const Payload& rhs) const = default;

        size_t size() const {
            return _data.size();
        }

        const uint8_t* data() const {
            return _data.data();
        }

      private:
        std::basic_string<uint8_t> _data;
    };
} // namespace assfire::messenger