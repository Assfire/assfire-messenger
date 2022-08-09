#pragma once

#include <string>

namespace assfire::messenger {
    class ChannelId {
      public:
        ChannelId() = default;
        ChannelId(std::string name) : _name(std::move(name)) {}
        ChannelId(const ChannelId& rhs) = default;
        ChannelId(ChannelId&& rhs)      = default;

        const std::string& name() const {
            return _name;
        }

        ChannelId& operator=(const ChannelId& rhs) = default;
        ChannelId& operator=(ChannelId&& rhs) = default;

        bool operator==(const ChannelId& rhs) const = default;

      private:
        std::string _name;
    };
} // namespace assfire::messenger

template<>
struct std::hash<assfire::messenger::ChannelId> {
    std::size_t operator()(const assfire::messenger::ChannelId& channel_id) const {
        return std::hash<std::string> {}(channel_id.name());
    }
};