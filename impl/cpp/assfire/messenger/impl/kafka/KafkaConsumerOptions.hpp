#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

namespace assfire::messenger {
    class KafkaConsumerOptions {
      public:
        enum class IsolationLevel { READ_UNCOMMITTED, READ_COMMITTED };

        KafkaConsumerOptions()                                = default;
        KafkaConsumerOptions(const KafkaConsumerOptions& rhs) = default;
        KafkaConsumerOptions(KafkaConsumerOptions&& rhs)      = default;

        KafkaConsumerOptions& operator=(const KafkaConsumerOptions& rhs) = default;
        KafkaConsumerOptions& operator=(KafkaConsumerOptions&& rhs) = default;

        bool operator==(const KafkaConsumerOptions& rhs) const = default;

        const std::string& topic_name() {
            return _topic_name;
        }

        const std::unordered_set<std::uint32_t>& partitions() {
            return _partitions;
        }

        std::string to_string() {
            return std::string();
        }

      private:
        std::vector<std::string> brokers;
        std::string group_id;
        std::string client_id;
        bool auto_commit;
        bool auto_offset_reset;
        int max_poll_records;
        int queued_min_messages;
        std::chrono::milliseconds session_timeout;
        std::chrono::milliseconds socket_timeout;
        IsolationLevel isolation_level;
        std::string partition_assignment_strategy;
        std::string security_protocol;

        std::string _topic_name;
        std::unordered_set<std::uint32_t> _partitions;
    };
} // namespace assfire::messenger