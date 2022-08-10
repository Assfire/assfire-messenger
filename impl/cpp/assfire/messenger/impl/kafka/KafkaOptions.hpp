#pragma once

#include <string>

namespace assfire::messenger {
    namespace KafkaOptions {

        enum class PropId { CLIENT_ID };

        template<PropId Id>
        class StringProperty {
          public:
            StringProperty() = default;
            StringProperty(std::string value) : _value(std::move(value)) {}
            StringProperty(const StringProperty& rhs) = default;
            StringProperty(StringProperty&& rhs)      = default;

            StringProperty& operator=(const StringProperty& rhs) = default;
            StringProperty& operator=(StringProperty&& rhs) = default;

            StringProperty& operator=(std::string rhs) {
                _value = std::move(rhs);
            };

            bool operator==(const StringProperty& rhs) const = default;

            std::string to_string_representation() {
                return _value;
            }

            PropId id() const {
                return Id;
            }

            std::string value() const {
                return _value;
            }

            void set_value(std::string value) {
                _value = std::move(value);
            }

          private:
            std::string _value;
        };

        using ClientId = StringProperty<PropId::CLIENT_ID>;
    }; // namespace KafkaOptions
} // namespace assfire::messenger