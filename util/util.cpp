#include "util.h"

#include <cmath>
#include <algorithm>

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

        struct TKeyAndTime {
            uint64_t Key;
            uint32_t Timestamp;

            TKeyAndTime(uint64_t k = 0, uint32_t ts = 0)
                : Key(k)
                , Timestamp(ts)
            {
            }
        };

        // returns: TRUE (profile is non empty after clean up) or FALSE (otherwise)
        bool CleanUp(
            Gandalf::TUserProfile& profile,
            const Gandalf::TConfig& config,
            uint32_t now)
        {
            const auto tooLongAgo = now - config.maxitemage();

            std::vector<uint32_t> eventsToRemove;
            std::vector<uint64_t> keysForRemove;
            std::vector<TKeyAndTime> keysToCheck;

            auto& events = *profile.mutable_events();

            // Parse every eventType
            for (auto& p: events) {
                const uint32_t eventType = p.first;
                auto& curEvents = *p.second.mutable_events(); // map<uint64,TEvent>

                keysForRemove.clear();
                keysToCheck.clear();

                const bool needFindFresh = (curEvents.size() > config.maxitemspereventtype());

                // Parse every event within particular eventType
                for (auto& pp : curEvents) {
                    const uint64_t key = pp.first;
                    auto& event = pp.second;
                    
                    if (event.timestamp() < tooLongAgo) {
                        keysForRemove.push_back(key);
                    } else if (needFindFresh) {
                        keysToCheck.emplace_back(key, event.timestamp());
                    } else {
                        // all good
                    }
                }

                // Remove too old keys.
                for (const auto& key : keysForRemove) {
                    curEvents.erase(key);
                }

                // Max item limit reached. We need to keep N freshest ones.
                if (keysToCheck.size() > config.maxitemspereventtype()) {
                    // sort DESC
                    std::sort(keysToCheck.begin(), keysToCheck.end(), [](const auto& x, const auto& y) {
                        return x.Timestamp > y.Timestamp;
                    });
                    for (size_t i = config.maxitemspereventtype(); i < keysToCheck.size(); ++i) {
                        curEvents.erase(keysToCheck[i].Key);
                    }
                }

                // If entire EventType emerged to be empty - it shall be removed from profile.
                if (curEvents.empty()) {
                    eventsToRemove.push_back(eventType);
                }
            }


            for (const auto& et : eventsToRemove) {
                events.erase(et);
            }

            return !profile.events().empty();
        }
    }
};
