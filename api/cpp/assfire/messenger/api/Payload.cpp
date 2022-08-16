#include "Payload.hpp"

namespace assfire::messenger {
    Payload pack(const std::string &msg) {
        static_assert(sizeof(std::string::value_type) == sizeof(uint8_t), "Incompatible string characters size");
        return messenger::Payload((uint8_t *) msg.data(), msg.size());
    };

    std::string_view to_string_view(const Payload& payload) {
        return std::string_view((std::string::value_type*) payload.data(), payload.size());
    }
} // namespace assfire::messenger