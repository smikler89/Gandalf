#include "util.h"

#include <cmath>

namespace Gandalf {
    namespace Util {
        void MergeSingleEvent(Gandalf::TUserProfile& toProfile, const TSingleEvent& event, const Gandalf::TConfig& config)
        {
            auto& events = *toProfile.mutable_events();
            auto& curEvents = *events[uint32_t(event.EventType)].mutable_events();

            const auto iter = curEvents.find(event.ItemId);
            if (iter != curEvents.end()) {
                // item is already presented
                Gandalf::TEvent& old = iter->second;
                const auto oldTs = old.timestamp();
                const auto newTs = event.Timestamp;
                if (oldTs > newTs) {
                    old.set_weight1(old.weight1() + event.Value * std::exp(config.alpha1() * (oldTs - newTs)));
                    old.set_weight2(old.weight2() + event.Value * std::exp(config.alpha2() * (oldTs - newTs)));
                } else {
                    old.set_weight1(old.weight1() * std::exp(config.alpha1() * (newTs - oldTs)) + event.Value);
                    old.set_weight2(old.weight2() * std::exp(config.alpha2() * (newTs - oldTs)) + event.Value);
                    old.set_timestamp(newTs);
                }
            } else {
                // item is not presented
                Gandalf::TEvent ne;
                ne.set_timestamp(event.Timestamp);
                ne.set_weight1(event.Value);
                ne.set_weight2(event.Value);
                curEvents[event.ItemId] = std::move(ne);
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
    }
};
