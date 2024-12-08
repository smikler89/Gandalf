#pragma once

#include "user_profile.pb.h"
#include "config.pb.h"

#include <ctime>

namespace Gandalf {
    void AddEvent(
        Gandalf::TUserProfile& toProfile,
        Gandalf::EEventType eventType,
        uint64_t itemId,
        const Gandalf::TConfig& config,
        uint32_t timestamp = std::time(0),
        float val = 1.0);

    // returns: TRUE (profile is non empty after clean up) or FALSE (otherwise)
    bool CleanUp(
        Gandalf::TUserProfile& profile,
        const Gandalf::TConfig& config,
        uint32_t timestamp = std::time(0)
    );
};