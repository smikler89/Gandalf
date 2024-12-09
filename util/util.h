#pragma once

#include "user_profile.pb.h"
#include "config.pb.h"

#include <ctime>

namespace Gandalf {
    namespace Util {

        struct TSingleEvent {
            EEventType EventType;
            uint64_t ItemId;
            uint32_t Timestamp;
            float Value;

            TSingleEvent(EEventType et = EEventType::None, uint64_t iid = 0, uint32_t ts = std::time(0), float v = 1.0)
                : EventType(et)
                , ItemId(iid)
                , Timestamp(ts)
                , Value(v)
            {
            }
        };

        void MergeSingleEvent(Gandalf::TUserProfile& toProfile, const TSingleEvent& event, const Gandalf::TConfig& config);

        // returns: TRUE (profile is non empty after clean up) or FALSE (otherwise)
        bool CleanUp(Gandalf::TUserProfile& profile, const Gandalf::TConfig& config, uint32_t timestamp = std::time(0));
        
    };
};
