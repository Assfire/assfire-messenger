#include "assfire/messenger/impl/kafka/KafkaMessageHeaders.hpp"

#include <gtest/gtest.h>

using namespace assfire::messenger;

TEST(KafkaMessageHeaders, PartitionHeaderIsEncodedAndDecodedCorrectly) {
    int32_t partition = 5;
    auto enc          = encode_partition_header(partition);
    auto dec          = decode_partition_header(enc);

    EXPECT_EQ(dec, 5);
}

TEST(KafkaMessageHeaders, OffsetHeaderIsEncodedAndDecodedCorrectly) {
    uint64_t offset = 5;
    auto enc        = encode_offset_header(offset);
    auto dec        = decode_offset_header(enc);

    EXPECT_EQ(dec, 5);
}