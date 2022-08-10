#pragma once

#include "KafkaOptions.hpp"
#include "kafka/ConsumerConfig.h"

#include <absl/strings/str_join.h>
#include <cstdint>
#include <string>
#include <unordered_set>

namespace assfire::messenger {
    class KafkaConsumerOptions {
      public:
        enum class IsolationLevel { READ_UNCOMMITTED, READ_COMMITTED };

        KafkaConsumerOptions()                                = default;
        KafkaConsumerOptions(const KafkaConsumerOptions &rhs) = default;
        KafkaConsumerOptions(KafkaConsumerOptions &&rhs)      = default;

        KafkaConsumerOptions &operator=(const KafkaConsumerOptions &rhs) = default;
        KafkaConsumerOptions &operator=(KafkaConsumerOptions &&rhs) = default;

        bool operator==(const KafkaConsumerOptions &rhs) const = default;

        std::string to_string() const {
            std::vector<std::string> tokens;
            tokens.push_back(_bootstrap_servers.to_string());
            tokens.push_back(_group_id.to_string());
            tokens.push_back(_client_id.to_string());
            tokens.push_back(_enable_auto_commit.to_string());
            tokens.push_back(_auto_offset_reset.to_string());
            tokens.push_back(_enable_partition_eof.to_string());
            tokens.push_back(_max_poll_records.to_string());
            tokens.push_back(_queued_min_messages.to_string());
            tokens.push_back(_session_timeout_ms.to_string());
            tokens.push_back(_socket_timeout_ms.to_string());
            tokens.push_back(_isolation_level.to_string());
            tokens.push_back(_partition_assignment_strategy.to_string());
            tokens.push_back(_security_protocol.to_string());
            return "{" + absl::StrJoin(tokens, ",") + "}";
        }

        kafka::clients::consumer::Config to_kafka_config() const {
            kafka::clients::consumer::Config result;
            _bootstrap_servers.fill_config(result);
            _group_id.fill_config(result);
            _client_id.fill_config(result);
            _enable_auto_commit.fill_config(result);
            _auto_offset_reset.fill_config(result);
            _enable_partition_eof.fill_config(result);
            _max_poll_records.fill_config(result);
            _queued_min_messages.fill_config(result);
            _session_timeout_ms.fill_config(result);
            _socket_timeout_ms.fill_config(result);
            _isolation_level.fill_config(result);
            _partition_assignment_strategy.fill_config(result);
            _security_protocol.fill_config(result);
            return result;
        }

        KafkaOptions::BootstrapServers bootstrap_servers() const {
            return _bootstrap_servers;
        }
        void set_bootstrap_servers(const KafkaOptions::BootstrapServers &bootstrap_servers) {
            _bootstrap_servers = bootstrap_servers;
        }

        KafkaOptions::GroupId group_id() const {
            return _group_id;
        }
        void set_group_id(const KafkaOptions::GroupId &group_id) {
            _group_id = group_id;
        }

        KafkaOptions::ClientId client_id() const {
            return _client_id;
        }
        void set_client_id(const KafkaOptions::ClientId &client_id) {
            _client_id = client_id;
        }

        KafkaOptions::EnableAutoCommit enable_auto_commit() const {
            return _enable_auto_commit;
        }
        void set_enable_auto_commit(const KafkaOptions::EnableAutoCommit &enable_auto_commit) {
            _enable_auto_commit = enable_auto_commit;
        }

        KafkaOptions::AutoOffsetReset auto_offset_reset() const {
            return _auto_offset_reset;
        }
        void set_auto_offset_reset(const KafkaOptions::AutoOffsetReset &auto_offset_reset) {
            _auto_offset_reset = auto_offset_reset;
        }

        KafkaOptions::EnablePartitionEof enable_partition_eof() const {
            return _enable_partition_eof;
        }
        void set_enable_partition_eof(const KafkaOptions::EnablePartitionEof &enable_partition_eof) {
            _enable_partition_eof = enable_partition_eof;
        }

        KafkaOptions::MaxPollRecords max_poll_records() const {
            return _max_poll_records;
        }
        void set_max_poll_records(const KafkaOptions::MaxPollRecords &max_poll_records) {
            _max_poll_records = max_poll_records;
        }

        KafkaOptions::QueuedMinMessages queued_min_messages() const {
            return _queued_min_messages;
        }
        void set_queued_min_messages(const KafkaOptions::QueuedMinMessages &queued_min_messages) {
            _queued_min_messages = queued_min_messages;
        }

        KafkaOptions::SessionTimeoutMs session_timeout_ms() const {
            return _session_timeout_ms;
        }
        void set_session_timeout_ms(const KafkaOptions::SessionTimeoutMs &session_timeout_ms) {
            _session_timeout_ms = session_timeout_ms;
        }

        KafkaOptions::SocketTimeoutMs socket_timeout_ms() const {
            return _socket_timeout_ms;
        }
        void set_socket_timeout_ms(const KafkaOptions::SocketTimeoutMs &socket_timeout_ms) {
            _socket_timeout_ms = socket_timeout_ms;
        }

        KafkaOptions::IsolationLevel isolation_level() const {
            return _isolation_level;
        }
        void set_isolation_level(const KafkaOptions::IsolationLevel &isolation_level) {
            _isolation_level = isolation_level;
        }

        KafkaOptions::PartitionAssignmentStrategy partition_assignment_strategy() const {
            return _partition_assignment_strategy;
        }
        void set_partition_assignment_strategy(const KafkaOptions::PartitionAssignmentStrategy &partition_assignment_strategy) {
            _partition_assignment_strategy = partition_assignment_strategy;
        }

        KafkaOptions::SecurityProtocol security_protocol() const {
            return _security_protocol;
        }
        void set_security_protocol(const KafkaOptions::SecurityProtocol &security_protocol) {
            _security_protocol = security_protocol;
        }

        std::string topic_name() const {
            return _topic_name;
        }
        void set_topic_name(const std::string &topic_name) {
            _topic_name = topic_name;
        }

        std::unordered_set<std::uint32_t> partitions() const {
            return _partitions;
        }
        void set_partitions(const std::unordered_set<std::uint32_t> &partitions) {
            _partitions = partitions;
        }

      private:
        KafkaOptions::BootstrapServers _bootstrap_servers;
        KafkaOptions::GroupId _group_id;
        KafkaOptions::ClientId _client_id;
        KafkaOptions::EnableAutoCommit _enable_auto_commit;
        KafkaOptions::AutoOffsetReset _auto_offset_reset;
        KafkaOptions::EnablePartitionEof _enable_partition_eof;
        KafkaOptions::MaxPollRecords _max_poll_records;
        KafkaOptions::QueuedMinMessages _queued_min_messages;
        KafkaOptions::SessionTimeoutMs _session_timeout_ms;
        KafkaOptions::SocketTimeoutMs _socket_timeout_ms;
        KafkaOptions::IsolationLevel _isolation_level;
        KafkaOptions::PartitionAssignmentStrategy _partition_assignment_strategy;
        KafkaOptions::SecurityProtocol _security_protocol;

        std::string _topic_name;
        std::unordered_set<std::uint32_t> _partitions;
    };
} // namespace assfire::messenger