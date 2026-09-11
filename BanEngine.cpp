#include "BanEngine.h"
#include <unordered_map>
#include <set>

BanEngine::BanEngine(int threshold, int windowMinutes)
    : threshold_(threshold), windowSeconds_(windowMinutes * 60)
{
}

std::map<std::string, int> BanEngine::findBanned(const std::vector<LogEntry>& entries)
{
    std::unordered_map<std::string, std::vector<time_t>> failTimes;
    std::set<std::string> banned;

    for (const auto& e : entries)
    {
        auto& times = failTimes[e.ip];
        times.push_back(e.seconds);

        while (!times.empty() && times.front() < e.seconds - windowSeconds_)
        {
            times.erase(times.begin());
        }

        if (times.size() >= threshold_)
        {
            banned.insert(e.ip);
        }
    }

    std::map<std::string, int> result;
    for (const auto& ip : banned)
    {
        result[ip] = failTimes[ip].size();
    }
    return result;
}