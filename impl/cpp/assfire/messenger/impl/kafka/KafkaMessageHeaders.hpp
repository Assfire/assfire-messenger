#pragma once

#include <string>
#include <cstdint>

namespace assfire::messenger {
    constexpr const char* KAFKA_HEADER_OFFSET          = "KAFKA_HEADER_OFFSET";
    constexpr const char* KAFKA_HEADER_TOPIC_NAME      = "KAFKA_HEADER_TOPIC_NAME";
    constexpr const char* KAFKA_HEADER_TOPIC_PARTITION = "KAFKA_HEADER_TOPIC_PARTITION";

    std::string encode_offset_header(uint64_t offset);
    uint64_t decode_offset_header(const std::string& value);

    std::string encode_partition_header(int32_t partition);
    int32_t decode_partition_header(const std::string& value);
} // namespace assfire::messenger