#include "util.h"

#include <cmath>

namespace Gandalf {
    void AddEvent(
        Gandalf::TUserProfile& toProfile,
        Gandalf::EEventType eventType,
        uint64_t itemId,
        const Gandalf::TConfig& config,
        uint32_t ts,
        float val)
    {
        auto& events = *toProfile.mutable_events();
        auto& curEvents = *events[uint32_t(eventType)].mutable_events();

        const auto iter = curEvents.find(itemId);
        if (iter != curEvents.end()) {
            // item is already presented
            Gandalf::TEvent& old = iter->second;
            const auto oldTs = old.timestamp();
            if (oldTs > ts) {
                old.set_weight1(old.weight1() + val * std::exp(config.alpha1() * (oldTs - ts)));
                old.set_weight2(old.weight2() + val * std::exp(config.alpha2() * (oldTs - ts)));
            } else {
                old.set_weight1(old.weight1() * std::exp(config.alpha1() * (ts - oldTs)) + val);
                old.set_weight2(old.weight2() * std::exp(config.alpha2() * (ts - oldTs)) + val);
                old.set_timestamp(ts);
            }
        } else {
            // item is not presented
            Gandalf::TEvent ne;
            ne.set_timestamp(ts);
            ne.set_weight1(val);
            ne.set_weight2(val);
            curEvents[itemId] = std::move(ne);
        }
    }

    // returns: TRUE (profile is non empty after clean up) or FALSE (otherwise)
    bool CleanUp(
        Gandalf::TUserProfile& profile,
        const Gandalf::TConfig& config,
        uint32_t timestamp
    )
    {
        // WIP
        return false;
    }
};
