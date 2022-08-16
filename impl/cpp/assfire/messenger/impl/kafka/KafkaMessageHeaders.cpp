#include "KafkaMessageHeaders.hpp"

namespace assfire::messenger {

    std::string encode_offset_header(uint64_t offset) {
        return std::to_string(offset);
    }

    uint64_t decode_offset_header(const std::string& value) {
        return std::stoll(value);
    }

    std::string encode_partition_header(int32_t partition) {
        return std::to_string(partition);
    }

    int32_t decode_partition_header(const std::string& value) {
        return std::stol(value);
    }

} // namespace assfire::messenger