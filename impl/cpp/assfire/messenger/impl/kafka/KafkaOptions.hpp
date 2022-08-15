#pragma once

#include <absl/strings/str_join.h>
#include <functional>
#include <kafka/ConsumerConfig.h>
#include <kafka/ProducerConfig.h>
#include <kafka/Properties.h>
#include <optional>
#include <string>
#include <vector>

namespace assfire::messenger {
    namespace KafkaOptions {

        template<typename T>
        class Property {
          protected:
            using Validator = std::function<T(T)>;
            using Formatter = std::function<std::string(const T&)>;

            static Formatter default_formatter() {
                return [](const T& value) {
                    return std::to_string(value);
                };
            }

            static Validator default_validator() {
                return [](T value) {
                    return value;
                };
            }

          public:
            Property(std::string name, Formatter format = default_formatter(), Validator validate = default_validator())
                : _name(name),
                  _format(format),
                  _validate(validate) {};
            Property(std::string name, std::optional<T> value, Formatter format = default_formatter(), Validator validate = default_validator())
                : _name(name),
                  _value(value ? validate(*value) : value),
                  _format(format),
                  _validate(validate) {}
            Property(const Property& rhs) = default;
            Property(Property&& rhs)      = default;

            Property<T>& operator=(const Property<T>& rhs) = default;
            Property<T>& operator=(Property<T>&& rhs) = default;

            Property<T>& operator=(std::string rhs) {
                set_value(std::move(rhs));
            };

            bool operator==(const Property<T>& rhs) const {
                return _name == rhs._name && _value == rhs._value;
            }

            std::string format() const {
                return _value ? _format(*_value) : "<unset>";
            }

            void fill_config(kafka::Properties& config) const {
                if (_value) config.put(name(), format());
            }

            std::string to_string() const {
                return _value ? name() + " = " + format() : "";
            }

            const std::string& name() const {
                return _name;
            }

            std::optional<T> value() const {
                return _value;
            }

            void set_value(T value) {
                _value = _validate(std::move(value));
            }

          private:
            std::string _name;
            std::optional<T> _value;
            Formatter _format;
            Validator _validate;
        };

        class StringProperty : public Property<std::string> {
          protected:
            static Formatter default_string_formatter() {
                return [](const std::string& s) {
                    return s;
                };
            }

          public:
            StringProperty(std::string name, Validator validate = default_validator(), Formatter format = default_string_formatter())
                : Property(std::move(name), format, validate) {};
            StringProperty(std::string name, std::optional<std::string> value, Validator validate = default_validator(),
                           Formatter format = default_string_formatter())
                : Property(std::move(name), std::move(value), format, validate) {}
        };

        class BoolProperty : public Property<bool> {
          protected:
            static Formatter default_bool_formatter() {
                return [](const bool& s) {
                    return s ? "true" : "false";
                };
            }

          public:
            BoolProperty(std::string name, Validator validate = default_validator(), Formatter format = default_bool_formatter())
                : Property(std::move(name), format, validate) {};
            BoolProperty(std::string name, std::optional<bool> value, Validator validate = default_validator(),
                         Formatter format = default_bool_formatter())
                : Property(std::move(name), std::move(value), format, validate) {}
        };

        template<typename T, T LBound, T RBound>
        class ConstrainedIntProperty : public Property<T> {
          protected:
            static typename Property<T>::Validator default_int_validator() {
                return [](T v) {
                    if (v < LBound || v > RBound) { throw std::invalid_argument("Kafka property value is out of range"); }
                    return v;
                };
            }

          public:
            ConstrainedIntProperty(std::string name, typename Property<T>::Validator validate = default_int_validator(),
                                   typename Property<T>::Formatter format = Property<T>::default_formatter())
                : Property<T>(std::move(name), format, validate) {};
            ConstrainedIntProperty(std::string name, std::optional<T> value, typename Property<T>::Validator validate = default_int_validator(),
                                   typename Property<T>::Formatter format = Property<T>::default_formatter())
                : Property<T>(std::move(name), std::move(value), format, validate) {}
        };

        template<typename T>
        class VectorProperty : public Property<std::vector<T>> {
          protected:
            using ElementFormatter = std::function<std::string(const T&)>;

            static ElementFormatter default_element_formatter() {
                return [](const T& v) {
                    return v;
                };
            }

            static typename Property<std::vector<T>>::Formatter default_vector_formatter(ElementFormatter element_formatter) {
                return [element_formatter](const std::vector<T>& v) {
                    std::vector<std::string> strings;
                    std::transform(v.begin(), v.end(), std::back_inserter(strings), element_formatter);
                    return absl::StrJoin(strings, ",");
                };
            }

          public:
            VectorProperty(std::string name, typename Property<std::vector<T>>::Validator validate = Property<std::vector<T>>::default_validator(),
                           typename Property<std::vector<T>>::Formatter format = default_vector_formatter(default_element_formatter()))
                : Property<std::vector<T>>(std::move(name), format, validate) {};
            VectorProperty(std::string name, std::optional<std::vector<T>> value,
                           typename Property<std::vector<T>>::Validator validate = Property<std::vector<T>>::default_validator(),
                           typename Property<std::vector<T>>::Formatter format   = default_vector_formatter(default_element_formatter()))
                : Property<std::vector<T>>(std::move(name), std::move(value), format, validate) {}
        };

        // ##### Consumer properties #####

        class BootstrapServers : public VectorProperty<std::string> {
          public:
            BootstrapServers(std::optional<std::vector<std::string>> value = std::nullopt)
                : VectorProperty(kafka::clients::consumer::Config::BOOTSTRAP_SERVERS, std::move(value)) {};
            BootstrapServers(std::vector<std::string> value)
                : VectorProperty(kafka::clients::consumer::Config::BOOTSTRAP_SERVERS, std::move(value)) {};
        };

        class GroupId : public StringProperty {
          public:
            GroupId(std::optional<std::string> group_id = std::nullopt)
                : StringProperty(kafka::clients::consumer::Config::GROUP_ID, std::move(group_id)) {};
            GroupId(std::string group_id) : StringProperty(kafka::clients::consumer::Config::GROUP_ID, std::move(group_id)) {};
            GroupId(const char* group_id) : StringProperty(kafka::clients::consumer::Config::GROUP_ID, group_id) {};
        };

        class ClientId : public StringProperty {
          public:
            ClientId(std::optional<std::string> client_id = std::nullopt)
                : StringProperty(kafka::clients::consumer::Config::CLIENT_ID, std::move(client_id)) {};
            ClientId(std::string client_id) : StringProperty(kafka::clients::consumer::Config::CLIENT_ID, std::move(client_id)) {};
            ClientId(const char* client_id) : StringProperty(kafka::clients::consumer::Config::CLIENT_ID, client_id) {};
        };

        class EnableAutoCommit : public BoolProperty {
          public:
            EnableAutoCommit(std::optional<bool> value = std::nullopt) : BoolProperty(kafka::clients::consumer::Config::ENABLE_AUTO_COMMIT, value) {};
            EnableAutoCommit(bool value) : BoolProperty(kafka::clients::consumer::Config::ENABLE_AUTO_COMMIT, value) {};
        };

        class AutoOffsetReset : public BoolProperty {
          public:
            AutoOffsetReset(std::optional<bool> value = std::nullopt) : BoolProperty(kafka::clients::consumer::Config::AUTO_OFFSET_RESET, value) {};
            AutoOffsetReset(bool value) : BoolProperty(kafka::clients::consumer::Config::AUTO_OFFSET_RESET, value) {};
        };

        class EnablePartitionEof : public BoolProperty {
          public:
            EnablePartitionEof(std::optional<bool> value = std::nullopt)
                : BoolProperty(kafka::clients::consumer::Config::ENABLE_PARTITION_EOF, value) {};
            EnablePartitionEof(bool value) : BoolProperty(kafka::clients::consumer::Config::ENABLE_PARTITION_EOF, value) {};
        };

        class MaxPollRecords : public ConstrainedIntProperty<int32_t, 1, 2000> {
          public:
            MaxPollRecords(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 2000>(kafka::clients::consumer::Config::MAX_POLL_RECORDS, value) {};
            MaxPollRecords(int32_t value) : ConstrainedIntProperty<int32_t, 1, 2000>(kafka::clients::consumer::Config::MAX_POLL_RECORDS, value) {};
        };

        class QueuedMinMessages : public ConstrainedIntProperty<int32_t, 1, 10000000> {
          public:
            QueuedMinMessages(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 10000000>(kafka::clients::consumer::Config::QUEUED_MIN_MESSAGES, value) {};
            QueuedMinMessages(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 10000000>(kafka::clients::consumer::Config::QUEUED_MIN_MESSAGES, value) {};
        };

        class SessionTimeoutMs : public ConstrainedIntProperty<int32_t, 1, 3600000> {
          public:
            SessionTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 3600000>(kafka::clients::consumer::Config::SESSION_TIMEOUT_MS, value) {};
            SessionTimeoutMs(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 3600000>(kafka::clients::consumer::Config::SESSION_TIMEOUT_MS, value) {};
        };

        class SocketTimeoutMs : public ConstrainedIntProperty<int32_t, 10, 300000> {
          public:
            SocketTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 10, 300000>(kafka::clients::consumer::Config::SOCKET_TIMEOUT_MS, value) {};
            SocketTimeoutMs(int32_t value)
                : ConstrainedIntProperty<int32_t, 10, 300000>(kafka::clients::consumer::Config::SOCKET_TIMEOUT_MS, value) {};
        };

        enum class IsolationLevelEnum { READ_UNCOMMITTED, READ_COMMITTED };

        class IsolationLevel : public Property<IsolationLevelEnum> {
          private:
            static Property<IsolationLevelEnum>::Formatter isolation_level_formatter() {
                return [](const auto& v) {
                    switch (v) {
                    case IsolationLevelEnum::READ_COMMITTED: return "read_committed";
                    case IsolationLevelEnum::READ_UNCOMMITTED: return "read_uncommitted";
                    default: throw std::invalid_argument("Unexpected isolation level enum value");
                    }
                };
            }

          public:
            IsolationLevel(std::optional<IsolationLevelEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::ISOLATION_LEVEL, value, isolation_level_formatter()) {};
            IsolationLevel(IsolationLevelEnum value)
                : Property(kafka::clients::consumer::Config::ISOLATION_LEVEL, value, isolation_level_formatter()) {};
        };

        enum class SecurityProtocolEnum { PLAIN_TEXT, SSL, SASL_PLAIN_TEXT, SASL_SSL };

        class SecurityProtocol : public Property<SecurityProtocolEnum> {
          private:
            static Property<SecurityProtocolEnum>::Formatter security_protocol_formatter() {
                return [](const auto& v) {
                    switch (v) {
                    case SecurityProtocolEnum::PLAIN_TEXT: return "plaintext";
                    case SecurityProtocolEnum::SSL: return "ssl";
                    case SecurityProtocolEnum::SASL_PLAIN_TEXT: return "sasl_plaintext";
                    case SecurityProtocolEnum::SASL_SSL: return "sasl_ssl";
                    default: throw std::invalid_argument("Unexpected security protocol enum value");
                    }
                };
            }

          public:
            SecurityProtocol(std::optional<SecurityProtocolEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::SECURITY_PROTOCOL, value, security_protocol_formatter()) {};
            SecurityProtocol(SecurityProtocolEnum value)
                : Property(kafka::clients::consumer::Config::SECURITY_PROTOCOL, value, security_protocol_formatter()) {};
        };

        enum class PartitionAssignmentStrategyEnum { RANGE, ROUND_ROBIN };

        class PartitionAssignmentStrategy : public Property<PartitionAssignmentStrategyEnum> {
          private:
            static Property<PartitionAssignmentStrategyEnum>::Formatter partition_assignment_strategy_formatter() {
                return [](const auto& v) {
                    switch (v) {
                    case PartitionAssignmentStrategyEnum::RANGE: return "range";
                    case PartitionAssignmentStrategyEnum::ROUND_ROBIN: return "round_robin";
                    default: throw std::invalid_argument("Unexpected partition assignment strategy enum value");
                    }
                };
            }

          public:
            PartitionAssignmentStrategy(std::optional<PartitionAssignmentStrategyEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::PARTITION_ASSIGNMENT_STRATEGY, value, partition_assignment_strategy_formatter()) {};
            PartitionAssignmentStrategy(PartitionAssignmentStrategyEnum value)
                : Property(kafka::clients::consumer::Config::PARTITION_ASSIGNMENT_STRATEGY, value, partition_assignment_strategy_formatter()) {};
        };

        class Acks : public ConstrainedIntProperty<int32_t, -1, 1000> {
          public:
            Acks(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, -1, 1000>(kafka::clients::producer::Config::ACKS, value) {};
            Acks(int32_t value) : ConstrainedIntProperty<int32_t, -1, 1000>(kafka::clients::producer::Config::ACKS, value) {};
        };

        class QueueBufferingMaxMessages : public ConstrainedIntProperty<int32_t, 1, 10000000> {
          public:
            QueueBufferingMaxMessages(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 10000000>(kafka::clients::producer::Config::QUEUE_BUFFERING_MAX_MESSAGES, value) {};
            QueueBufferingMaxMessages(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 10000000>(kafka::clients::producer::Config::QUEUE_BUFFERING_MAX_MESSAGES, value) {};
        };

        class QueueBufferingMaxKbytes : public ConstrainedIntProperty<int32_t, 1, 2147483647> {
          public:
            QueueBufferingMaxKbytes(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 2147483647>(kafka::clients::producer::Config::QUEUE_BUFFERING_MAX_KBYTES, value) {};
            QueueBufferingMaxKbytes(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 2147483647>(kafka::clients::producer::Config::QUEUE_BUFFERING_MAX_KBYTES, value) {};
        };

        class LingerMs : public ConstrainedIntProperty<int32_t, 0, 900000> {
          public:
            LingerMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 0, 900000>(kafka::clients::producer::Config::LINGER_MS, value) {};
            LingerMs(int32_t value) : ConstrainedIntProperty<int32_t, 0, 900000>(kafka::clients::producer::Config::LINGER_MS, value) {};
        };

        class BatchNumMessages : public ConstrainedIntProperty<int32_t, 1, 1000000> {
          public:
            BatchNumMessages(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 1000000>(kafka::clients::producer::Config::BATCH_NUM_MESSAGES, value) {};
            BatchNumMessages(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 1000000>(kafka::clients::producer::Config::BATCH_NUM_MESSAGES, value) {};
        };

        class BatchSize : public ConstrainedIntProperty<int32_t, 1, 2147483647> {
          public:
            BatchSize(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 2147483647>(kafka::clients::producer::Config::BATCH_SIZE, value) {};
            BatchSize(int32_t value) : ConstrainedIntProperty<int32_t, 1, 2147483647>(kafka::clients::producer::Config::BATCH_SIZE, value) {};
        };

        class MessageMaxBytes : public ConstrainedIntProperty<int32_t, 1000, 1000000000> {
          public:
            MessageMaxBytes(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1000, 1000000000>(kafka::clients::producer::Config::MESSAGE_MAX_BYTES, value) {};
            MessageMaxBytes(int32_t value)
                : ConstrainedIntProperty<int32_t, 1000, 1000000000>(kafka::clients::producer::Config::MESSAGE_MAX_BYTES, value) {};
        };

        class MessageTimeoutMs : public ConstrainedIntProperty<int32_t, 0, 2147483647> {
          public:
            MessageTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 0, 2147483647>(kafka::clients::producer::Config::MESSAGE_TIMEOUT_MS, value) {};
            MessageTimeoutMs(int32_t value)
                : ConstrainedIntProperty<int32_t, 0, 2147483647>(kafka::clients::producer::Config::MESSAGE_TIMEOUT_MS, value) {};
        };

        class RequestTimeoutMs : public ConstrainedIntProperty<int32_t, 1, 900000> {
          public:
            RequestTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 900000>(kafka::clients::producer::Config::REQUEST_TIMEOUT_MS, value) {};
            RequestTimeoutMs(int32_t value)
                : ConstrainedIntProperty<int32_t, 1, 900000>(kafka::clients::producer::Config::REQUEST_TIMEOUT_MS, value) {};
        };

        enum class PartitionerEnum { RANDOM, CONSISTENT, CONSISTENT_RANDOM, MURMUR2, MURMUR2_RANDOM, FNV1A, FNV1A_RANDOM };

        class Partitioner : public Property<PartitionerEnum> {
          private:
            static Property<PartitionerEnum>::Formatter partitioner_formatter() {
                return [](const auto& v) {
                    switch (v) {
                    case PartitionerEnum::RANDOM: return "random";
                    case PartitionerEnum::CONSISTENT: return "consistent";
                    case PartitionerEnum::CONSISTENT_RANDOM: return "consistent_random";
                    case PartitionerEnum::MURMUR2: return "murmur2";
                    case PartitionerEnum::MURMUR2_RANDOM: return "murmur2_random";
                    case PartitionerEnum::FNV1A: return "fnv1a";
                    case PartitionerEnum::FNV1A_RANDOM: return "fnv1a_random";
                    default: throw std::invalid_argument("Unexpected partitioner enum value");
                    }
                };
            }

          public:
            Partitioner(std::optional<PartitionerEnum> value = std::nullopt)
                : Property(kafka::clients::producer::Config::PARTITIONER, value, partitioner_formatter()) {};
            Partitioner(PartitionerEnum value) : Property(kafka::clients::producer::Config::PARTITIONER, value, partitioner_formatter()) {};
        };

        class MaxInFlight : public ConstrainedIntProperty<int32_t, 1, 1000000> {
          public:
            MaxInFlight(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 1000000>(kafka::clients::producer::Config::MAX_IN_FLIGHT, value) {};
            MaxInFlight(int32_t value) : ConstrainedIntProperty<int32_t, 1, 1000000>(kafka::clients::producer::Config::MAX_IN_FLIGHT, value) {};
        };

        class EnableIdempotence : public BoolProperty {
          public:
            EnableIdempotence(std::optional<bool> value = std::nullopt)
                : BoolProperty(kafka::clients::producer::Config::ENABLE_IDEMPOTENCE, value) {};
            EnableIdempotence(bool value) : BoolProperty(kafka::clients::producer::Config::ENABLE_IDEMPOTENCE, value) {};
        };

        class TransactionalId : public StringProperty {
          public:
            TransactionalId(std::optional<std::string> value = std::nullopt)
                : StringProperty(kafka::clients::producer::Config::TRANSACTIONAL_ID, std::move(value)) {};
            TransactionalId(std::string value) : StringProperty(kafka::clients::producer::Config::TRANSACTIONAL_ID, std::move(value)) {};
            TransactionalId(const char* value) : StringProperty(kafka::clients::producer::Config::TRANSACTIONAL_ID, value) {};
        };

        class TransactionTimeoutMs : public ConstrainedIntProperty<int32_t, 1000, 2147483647> {
          public:
            TransactionTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1000, 2147483647>(kafka::clients::producer::Config::TRANSACTION_TIMEOUT_MS, value) {};
            TransactionTimeoutMs(int32_t value)
                : ConstrainedIntProperty<int32_t, 1000, 2147483647>(kafka::clients::producer::Config::TRANSACTION_TIMEOUT_MS, value) {};
        };

    }; // namespace KafkaOptions
} // namespace assfire::messenger