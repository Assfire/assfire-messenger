#pragma once

#include "KafkaOptions.hpp"
#include "kafka/ConsumerConfig.h"

#include <absl/strings/str_join.h>
#include <cstdint>
#include <string>
#include <unordered_set>

namespace assfire::messenger {
    class KafkaPublisherOptions {
      public:
        KafkaPublisherOptions()                                 = default;
        KafkaPublisherOptions(const KafkaPublisherOptions &rhs) = default;
        KafkaPublisherOptions(KafkaPublisherOptions &&rhs)      = default;

        KafkaPublisherOptions &operator=(const KafkaPublisherOptions &rhs) = default;
        KafkaPublisherOptions &operator=(KafkaPublisherOptions &&rhs) = default;

        bool operator==(const KafkaPublisherOptions &rhs) const = default;

        std::string to_string() const {
            std::vector<std::string> tokens;
            tokens.push_back(_bootstrap_servers.to_string());
            tokens.push_back(_client_id.to_string());
            tokens.push_back(_acks.to_string());
            tokens.push_back(_queue_buffering_max_messages.to_string());
            tokens.push_back(_queue_buffering_max_kbytes.to_string());
            tokens.push_back(_linger_ms.to_string());
            tokens.push_back(_batch_num_messages.to_string());
            tokens.push_back(_batch_size.to_string());
            tokens.push_back(_message_max_bytes.to_string());
            tokens.push_back(_message_timeout_ms.to_string());
            tokens.push_back(_request_timeout_ms.to_string());
            tokens.push_back(_partitioner.to_string());
            tokens.push_back(_max_in_flight.to_string());
            tokens.push_back(_enable_idempotence.to_string());
            tokens.push_back(_transactional_id.to_string());
            tokens.push_back(_transaction_timeout_ms.to_string());
            tokens.push_back(_security_protocol.to_string());
            return "{" + absl::StrJoin(tokens, ",") + "}";
        }

        kafka::clients::producer::Config to_kafka_config() const {
            kafka::clients::producer::Config result;
            _bootstrap_servers.fill_config(result);
            _client_id.fill_config(result);
            _acks.fill_config(result);
            _queue_buffering_max_messages.fill_config(result);
            _queue_buffering_max_kbytes.fill_config(result);
            _linger_ms.fill_config(result);
            _batch_num_messages.fill_config(result);
            _batch_size.fill_config(result);
            _message_max_bytes.fill_config(result);
            _message_timeout_ms.fill_config(result);
            _request_timeout_ms.fill_config(result);
            _partitioner.fill_config(result);
            _max_in_flight.fill_config(result);
            _enable_idempotence.fill_config(result);
            _transactional_id.fill_config(result);
            _transaction_timeout_ms.fill_config(result);
            _security_protocol.fill_config(result);
            return result;
        }

        KafkaOptions::BootstrapServers bootstrap_servers() const {
            return _bootstrap_servers;
        }
        void set_bootstrap_servers(const KafkaOptions::BootstrapServers &bootstrap_servers) {
            _bootstrap_servers = bootstrap_servers;
        }

        KafkaOptions::ClientId client_id() const {
            return _client_id;
        }
        void set_client_id(const KafkaOptions::ClientId &client_id) {
            _client_id = client_id;
        }

        KafkaOptions::Acks acks() const {
            return _acks;
        }
        void set_acks(const KafkaOptions::Acks &acks) {
            _acks = acks;
        }

        KafkaOptions::QueueBufferingMaxMessages queue_buffering_max_messages() const {
            return _queue_buffering_max_messages;
        }
        void set_queue_buffering_max_messages(const KafkaOptions::QueueBufferingMaxMessages &queue_buffering_max_messages) {
            _queue_buffering_max_messages = queue_buffering_max_messages;
        }

        KafkaOptions::QueueBufferingMaxKbytes queue_buffering_max_kbytes() const {
            return _queue_buffering_max_kbytes;
        }
        void set_queue_buffering_max_kbytes(const KafkaOptions::QueueBufferingMaxKbytes &queue_buffering_max_kbytes) {
            _queue_buffering_max_kbytes = queue_buffering_max_kbytes;
        }

        KafkaOptions::LingerMs linger_ms() const {
            return _linger_ms;
        }
        void set_linger_ms(const KafkaOptions::LingerMs &linger_ms) {
            _linger_ms = linger_ms;
        }

        KafkaOptions::BatchNumMessages batch_num_messages() const {
            return _batch_num_messages;
        }
        void set_batch_num_messages(const KafkaOptions::BatchNumMessages &batch_num_messages) {
            _batch_num_messages = batch_num_messages;
        }

        KafkaOptions::BatchSize batch_size() const {
            return _batch_size;
        }
        void set_batch_size(const KafkaOptions::BatchSize &batch_size) {
            _batch_size = batch_size;
        }

        KafkaOptions::MessageMaxBytes message_max_bytes() const {
            return _message_max_bytes;
        }
        void set_message_max_bytes(const KafkaOptions::MessageMaxBytes &message_max_bytes) {
            _message_max_bytes = message_max_bytes;
        }

        KafkaOptions::MessageTimeoutMs message_timeout_ms() const {
            return _message_timeout_ms;
        }
        void set_message_timeout_ms(const KafkaOptions::MessageTimeoutMs &message_timeout_ms) {
            _message_timeout_ms = message_timeout_ms;
        }

        KafkaOptions::RequestTimeoutMs request_timeout_ms() const {
            return _request_timeout_ms;
        }
        void set_request_timeout_ms(const KafkaOptions::RequestTimeoutMs &request_timeout_ms) {
            _request_timeout_ms = request_timeout_ms;
        }

        KafkaOptions::Partitioner partitioner() const {
            return _partitioner;
        }
        void set_partitioner(const KafkaOptions::Partitioner &partitioner) {
            _partitioner = partitioner;
        }

        KafkaOptions::MaxInFlight max_in_flight() const {
            return _max_in_flight;
        }
        void set_max_in_flight(const KafkaOptions::MaxInFlight &max_in_flight) {
            _max_in_flight = max_in_flight;
        }

        KafkaOptions::EnableIdempotence enable_idempotence() const {
            return _enable_idempotence;
        }
        void set_enable_idempotence(const KafkaOptions::EnableIdempotence &enable_idempotence) {
            _enable_idempotence = enable_idempotence;
        }

        KafkaOptions::TransactionalId transactional_id() const {
            return _transactional_id;
        }
        void set_transactional_id(const KafkaOptions::TransactionalId &transactional_id) {
            _transactional_id = transactional_id;
        }

        KafkaOptions::TransactionTimeoutMs transaction_timeout_ms() const {
            return _transaction_timeout_ms;
        }
        void set_transaction_timeout_ms(const KafkaOptions::TransactionTimeoutMs &transaction_timeout_ms) {
            _transaction_timeout_ms = transaction_timeout_ms;
        }

        KafkaOptions::SecurityProtocol security_protocol() const {
            return _security_protocol;
        }
        void set_security_protocol(const KafkaOptions::SecurityProtocol &security_protocol) {
            _security_protocol = security_protocol;
        }

        const std::string &topic_name() const {
            return _topic_name;
        }
        void set_topic_name(std::string topic_name) {
            _topic_name = std::move(topic_name);
        }

      private:
        KafkaOptions::BootstrapServers _bootstrap_servers;
        KafkaOptions::ClientId _client_id;
        KafkaOptions::Acks _acks;
        KafkaOptions::QueueBufferingMaxMessages _queue_buffering_max_messages;
        KafkaOptions::QueueBufferingMaxKbytes _queue_buffering_max_kbytes;
        KafkaOptions::LingerMs _linger_ms;
        KafkaOptions::BatchNumMessages _batch_num_messages;
        KafkaOptions::BatchSize _batch_size;
        KafkaOptions::MessageMaxBytes _message_max_bytes;
        KafkaOptions::MessageTimeoutMs _message_timeout_ms;
        KafkaOptions::RequestTimeoutMs _request_timeout_ms;
        KafkaOptions::Partitioner _partitioner;
        KafkaOptions::MaxInFlight _max_in_flight;
        KafkaOptions::EnableIdempotence _enable_idempotence;
        KafkaOptions::TransactionalId _transactional_id;
        KafkaOptions::TransactionTimeoutMs _transaction_timeout_ms;
        KafkaOptions::SecurityProtocol _security_protocol;

        std::string _topic_name;
    };
} // namespace assfire::messenger