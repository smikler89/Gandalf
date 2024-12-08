#pragma once

#include "user_profile.pb.h"
#include "config.pb.h"

#include <ctime>

namespace Gandalf {
    namespace Util {

        struct TSingleEvent {
            Gandalf::EEventType EventType;
            uint64_t ItemId;
            uint32_t Timestamp = std::time(0);
            float Value = 1.0;
        };

        void MergeSingleEvent(Gandalf::TUserProfile& toProfile, const TSingleEvent& event, const Gandalf::TConfig& config);

        // returns: TRUE (profile is non empty after clean up) or FALSE (otherwise)
        bool CleanUp(Gandalf::TUserProfile& profile, const Gandalf::TConfig& config, uint32_t timestamp = std::time(0));
        
    };
};
