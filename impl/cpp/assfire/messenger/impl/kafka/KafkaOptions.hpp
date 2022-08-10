#pragma once

#include <absl/strings/str_join.h>
#include <functional>
#include <kafka/ConsumerConfig.h>
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
                return name() + " = " + format();
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
        };

        class GroupId : public StringProperty {
          public:
            GroupId(std::optional<std::string> group_id = std::nullopt)
                : StringProperty(kafka::clients::consumer::Config::GROUP_ID, std::move(group_id)) {};
        };

        class ClientId : public StringProperty {
          public:
            ClientId(std::optional<std::string> client_id = std::nullopt)
                : StringProperty(kafka::clients::consumer::Config::CLIENT_ID, std::move(client_id)) {};
        };

        class EnableAutoCommit : public BoolProperty {
          public:
            EnableAutoCommit(std::optional<bool> value = std::nullopt) : BoolProperty(kafka::clients::consumer::Config::ENABLE_AUTO_COMMIT, value) {};
        };

        class AutoOffsetReset : public BoolProperty {
          public:
            AutoOffsetReset(std::optional<bool> value = std::nullopt) : BoolProperty(kafka::clients::consumer::Config::AUTO_OFFSET_RESET, value) {};
        };

        class EnablePartitionEof : public BoolProperty {
          public:
            EnablePartitionEof(std::optional<bool> value = std::nullopt)
                : BoolProperty(kafka::clients::consumer::Config::ENABLE_PARTITION_EOF, value) {};
        };

        class MaxPollRecords : public ConstrainedIntProperty<int32_t, 1, 2000> {
          public:
            MaxPollRecords(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 2000>(kafka::clients::consumer::Config::MAX_POLL_RECORDS, value) {};
        };

        class QueuedMinMessages : public ConstrainedIntProperty<int32_t, 1, 10000000> {
          public:
            QueuedMinMessages(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 10000000>(kafka::clients::consumer::Config::QUEUED_MIN_MESSAGES, value) {};
        };

        class SessionTimeoutMs : public ConstrainedIntProperty<int32_t, 1, 3600000> {
          public:
            SessionTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 1, 3600000>(kafka::clients::consumer::Config::SESSION_TIMEOUT_MS, value) {};
        };

        class SocketTimeoutMs : public ConstrainedIntProperty<int32_t, 10, 300000> {
          public:
            SocketTimeoutMs(std::optional<int32_t> value = std::nullopt)
                : ConstrainedIntProperty<int32_t, 10, 300000>(kafka::clients::consumer::Config::SOCKET_TIMEOUT_MS, value) {};
        };

        enum class IsolationLevelEnum { READ_UNCOMMITTED, READ_COMMITTED };

        class IsolationLevel : public Property<IsolationLevelEnum> {
          public:
            IsolationLevel(std::optional<IsolationLevelEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::ISOLATION_LEVEL, value, [](const auto& v) {
                      switch (v) {
                      case IsolationLevelEnum::READ_COMMITTED: return "read_committed";
                      case IsolationLevelEnum::READ_UNCOMMITTED: return "read_uncommitted";
                      default: throw std::invalid_argument("Unexpected isolation level enum value");
                      }
                  }) {};
        };

        enum class SecurityProtocolEnum { PLAIN_TEXT, SSL, SASL_PLAIN_TEXT, SASL_SSL };

        class SecurityProtocol : public Property<SecurityProtocolEnum> {
          public:
            SecurityProtocol(std::optional<SecurityProtocolEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::SECURITY_PROTOCOL, value, [](const auto& v) {
                      switch (v) {
                      case SecurityProtocolEnum::PLAIN_TEXT: return "plaintext";
                      case SecurityProtocolEnum::SSL: return "ssl";
                      case SecurityProtocolEnum::SASL_PLAIN_TEXT: return "sasl_plaintext";
                      case SecurityProtocolEnum::SASL_SSL: return "sasl_ssl";
                      default: throw std::invalid_argument("Unexpected security protocol enum value");
                      }
                  }) {};
        };

        enum class PartitionAssignmentStrategyEnum { RANGE, ROUND_ROBIN };

        class PartitionAssignmentStrategy : public Property<PartitionAssignmentStrategyEnum> {
          public:
            PartitionAssignmentStrategy(std::optional<PartitionAssignmentStrategyEnum> value = std::nullopt)
                : Property(kafka::clients::consumer::Config::PARTITION_ASSIGNMENT_STRATEGY, value, [](const auto& v) {
                      switch (v) {
                      case PartitionAssignmentStrategyEnum::RANGE: return "range";
                      case PartitionAssignmentStrategyEnum::ROUND_ROBIN: return "round_robin";
                      default: throw std::invalid_argument("Unexpected partition assignment strategy enum value");
                      }
                  }) {};
        };

    }; // namespace KafkaOptions
} // namespace assfire::messenger