#include <iostream>
#include <fstream>
#include "parser.h"
#include <vector>
#include <set>
#include <sstream>
#include <unordered_map>
#include <ctime>
#include <iomanip>

struct LogEntry { std::string ip; std::string timeText; time_t seconds; };
const int threshold = 5;
const int windowMinutes = 10;
const int windowSeconds = windowMinutes * 60;

time_t toSeconds(const std::string& timeText)
{
    std::istringstream ss(timeText);
    std::tm t{};
    ss >> std::get_time(&t, "%b %d %H:%M:%S");
    if (ss.fail())
    {
        return -1;
    }
    t.tm_year = 2026 - 1900;
    t.tm_isdst = -1;
    return std::mktime(&t);
}

int main()
{
    std::ifstream file("sample_auth.log");

    if (!file.is_open())
    {
        std::cerr << "File not found" << std::endl;
        return 1;
    }

    std::string line;
    int failCount = 0;
    int readLines = 0;

    std::vector<LogEntry> entries;
    std::set<std::string> uniqueIps;

    while (std::getline(file, line))
    {
        readLines++;
        if (line.contains("Failed password"))
        {
            failCount++;

            size_t fromPos = line.find("from ");
            if (fromPos == std::string::npos)
            {
                continue;
            }
            size_t ipStart = fromPos + 5;
            size_t spaceAfter = line.find(' ', ipStart);
            std::string ip = line.substr(ipStart, spaceAfter - ipStart);

            LogEntry entry;
            entry.ip = ip;
            entry.timeText = line.substr(0, 15);
            entry.seconds = toSeconds(entry.timeText);

            uniqueIps.insert(ip);
            entries.push_back(entry);
        }
    }

    std::unordered_map<std::string, std::vector<time_t>> failTimes;
    std::set<std::string> banned;


    std::cout << "Lines read: " << readLines << "\n" << "Failure lines: "
            << failCount << "\n" << "Unique attacker IPs: " << uniqueIps.size() << std::endl;

    for (const auto& e: entries)
    {
        auto& times = failTimes[e.ip];
        times.push_back(e.seconds);

        while (!times.empty() &&  times.front() < e.seconds - windowSeconds )
        {
            times.erase(times.begin());
        }

        if (times.size() >= threshold)
        {
            banned.insert(e.ip);
        }
    }

    for (const auto& ban : banned)
    {
        std::cout << "BANNED " << ban << " (" << failTimes[ban].size() << " failures)\n";
    }
    Parse();

    std::cout << "Gap test 1: " << toSeconds(entries[1].timeText) - toSeconds(entries[0].timeText) << "\n";
}