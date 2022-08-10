#pragma once

#include "kafka/ConsumerConfig.h"
#include "KafkaOptions.hpp"

#include <absl/strings/str_join.h>
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

        std::string to_string() {
            return std::string();
        }

        const std::vector<std::string>& brokers() const {
            return _brokers;
        }
        void set_brokers(std::vector<std::string> brokers) {
            _brokers = std::move(brokers);
        }

        const std::string& group_id() const {
            return _group_id;
        }
        void set_group_id(std::string group_id) {
            _group_id = std::move(group_id);
        }

        const std::string& client_id() const {
            return _client_id;
        }
        void set_client_id(std::string client_id) {
            _client_id = std::move(client_id);
        }

        bool auto_commit_enabled() const {
            return _auto_commit_enabled;
        }
        void set_auto_commit_enabled(bool auto_commit_enabled) {
            _auto_commit_enabled = auto_commit_enabled;
        }

        bool auto_offset_reset() const {
            return _auto_offset_reset;
        }
        void set_auto_offset_reset(bool auto_offset_reset) {
            _auto_offset_reset = auto_offset_reset;
        }

        int max_poll_records() const {
            return _max_poll_records;
        }
        void set_max_poll_records(int max_poll_records) {
            _max_poll_records = max_poll_records;
        }

        int queued_min_messages() const {
            return _queued_min_messages;
        }
        void set_queued_min_messages(int queued_min_messages) {
            _queued_min_messages = queued_min_messages;
        }

        std::chrono::milliseconds session_timeout() const {
            return _session_timeout;
        }
        void set_session_timeout(std::chrono::milliseconds session_timeout) {
            _session_timeout = std::move(session_timeout);
        }

        std::chrono::milliseconds socket_timeout() const {
            return _socket_timeout;
        }
        void set_socket_timeout(std::chrono::milliseconds socket_timeout) {
            _socket_timeout = std::move(socket_timeout);
        }

        IsolationLevel isolation_level() const {
            return _isolation_level;
        }
        void set_isolation_level(IsolationLevel isolation_level) {
            _isolation_level = isolation_level;
        }

        const std::string& partition_assignment_strategy() const {
            return _partition_assignment_strategy;
        }
        void set_partition_assignment_strategy(std::string partition_assignment_strategy) {
            _partition_assignment_strategy = std::move(partition_assignment_strategy);
        }

        const std::string& security_protocol() const {
            return _security_protocol;
        }
        void set_security_protocol(std::string security_protocol) {
            _security_protocol = std::move(security_protocol);
        }

        const std::string& topic_name() const {
            return _topic_name;
        }
        void set_topic_name(std::string topic_name) {
            _topic_name = std::move(topic_name);
        }

        const std::unordered_set<std::uint32_t>& partitions() const {
            return _partitions;
        }
        void set_partitions(std::unordered_set<std::uint32_t> partitions) {
            _partitions = std::move(partitions);
        }

        void fill_config(kafka::clients::consumer::Config& config) const {
            using Config = kafka::clients::consumer::Config;

        
        }

      private:
        std::string brokers_string() const {
            return "";
        }

        std::string group_id_string() const {
            return "";
        }

        std::string client_id_string() const {
            return "";
        }

        std::string auto_commit_enabled_string() const {
            return "";
        }

        std::string auto_offset_reset_string() const {
            return "";
        }

        std::string max_poll_records_string() const {
            return "";
        }

        std::string queued_min_messages_string() const {
            return "";
        }

        std::string session_timeout_string() const {
            return "";
        }

        std::string socket_timeout_string() const {
            return "";
        }

        std::string isolation_level_string() const {
            return "";
        }

        std::string partition_assignment_strategy_string() const {
            return "";
        }

        std::string security_protocol_string() const {
            return "";
        }

        std::vector<std::string> _brokers;
        std::string _group_id;
        std::string _client_id;
        bool _auto_commit_enabled;
        bool _auto_offset_reset;
        int _max_poll_records;
        int _queued_min_messages;
        std::chrono::milliseconds _session_timeout;
        std::chrono::milliseconds _socket_timeout;
        IsolationLevel _isolation_level;
        std::string _partition_assignment_strategy;
        std::string _security_protocol;

        std::string _topic_name;
        std::unordered_set<std::uint32_t> _partitions;
    };
} // namespace assfire::messenger