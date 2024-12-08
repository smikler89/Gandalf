#pragma once

#include "util.h"

#include "user_profile.pb.h"
#include "config.pb.h"

#include <memory>
#include <vector>

namespace Gandalf {
    // Manager is an abstraction that manages fetching & storing profiles.
    // Implementation could be: file-based, memory-based, SQL db, KV storage, etc.
    // Also it could be:
    // - all FetchProfile, StoreProfile and DeleteProfile supported (for RT updates)
    // - only FetchProfile supported (for serving)
    // - only StoreProfile supported (restoring from backup, or copying entire profiles from other place)
    // Cache logic could be achieved by implementing IProfileManager that receives anoter IProfileManager (basic one) + LRU cache settings.
    class IProfileManager {
    public:
        virtual std::shared_ptr<Gandalf::TUserProfile> FetchProfile(const std::string& userId) = 0;
        virtual void StoreProfile(const std::string& userId, const Gandalf::TUserProfile& profile) = 0;
        virtual void DeleteProfile(const std::string& userId) = 0;
        virtual ~IProfileManager() = default;
    };


    // example of RT updates based processing
    inline void RunUserUpdate(
        const Gandalf::TConfig& config,
        IProfileManager& manager,
        const std::string& userId,
        const std::vector<Util::TSingleEvent>& events,
        uint32_t ts = std::time(0))
    {
        // read profile
        auto profile = manager.FetchProfile(userId);

        // merge all events
        for (const auto& event : events) {
            Util::MergeSingleEvent(*profile, event, config);
        }
        
        // apply clean up policies
        if (Util::CleanUp(*profile, config, ts)) {
            // profile is non empty => update in Database
            manager.StoreProfile(userId, *profile);
        } else {
            // profile is empty => remove from Database
            manager.DeleteProfile(userId);
        }
    }
};
