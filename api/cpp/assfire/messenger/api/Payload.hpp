#pragma once

#include <concepts>
#include <cstdint>
#include <string>

namespace assfire::messenger {
    using Payload = std::basic_string<uint8_t>;

    template<typename T>
    concept ProtoMessage = requires(T m) {
        { m.ByteSizeLong() }
        ->std::convertible_to<std::size_t>;

        { m.SerializeToArray((void *) 0, 0) }
        ->std::convertible_to<bool>;

        { m.ParseFromArray((void *) 0, 0) }
        ->std::convertible_to<bool>;
    };

    template<ProtoMessage T>
    Payload pack(const T &msg) {
        messenger::Payload payload;
        payload.resize(msg.ByteSizeLong());
        msg.SerializeToArray(payload.data(), payload.size());
        return payload;
    };

    template<ProtoMessage T>
    T unpack(const Payload p) {
        T result;
        result.ParseFromArray(p.data(), p.size());
        return result;
    };

} // namespace assfire::messenger