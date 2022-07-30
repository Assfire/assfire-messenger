#pragma once

#include <string>

namespace assfire::messenger {
    class ChannelId {
      public:
        using TopicId = std::string;

        ChannelId() = default;
        ChannelId(TopicId topic_id) : _topic_id(std::move(topic_id)) {}
        ChannelId(const ChannelId& rhs) = default;
        ChannelId(ChannelId&& rhs)      = default;

        ChannelId& operator=(const ChannelId& rhs) = default;
        ChannelId& operator=(ChannelId&& rhs) = default;

        bool operator==(const ChannelId& rhs) const = default;

      private:
        TopicId _topic_id;
    };
} // namespace assfire::messenger