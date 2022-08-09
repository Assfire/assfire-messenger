#pragma once

#include "assfire/messenger/api/ChannelId.hpp"

#include <stdexcept>
#include <string>

namespace assfire::messenger {

    class ChannelNotDeclaredError : public std::runtime_error {
      public:
        ChannelNotDeclaredError(const ChannelId& channel_id) : std::runtime_error(std::string("Channel not declared: ") + channel_id.name()) {}
    };

    class ChannelRedeclarationAttemptError : public std::runtime_error {
      public:
        ChannelRedeclarationAttemptError(const ChannelId& channel_id)
            : std::runtime_error(std::string("Channel redeclaration with different settings: ") + channel_id.name()) {}
    };
} // namespace assfire::messenger