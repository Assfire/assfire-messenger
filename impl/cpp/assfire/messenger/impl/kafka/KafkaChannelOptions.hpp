#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

namespace assfire::messenger {
    class KafkaChannelOptions {
      public:
        const std::string& topic_name() {
            return _topic_name;
        }

        const std::unordered_set<std::uint32_t>& partitions() {
            return _partitions;
        }

      private:
        std::string _topic_name;
        std::unordered_set<std::uint32_t> _partitions;
    };
} // namespace assfire::messenger