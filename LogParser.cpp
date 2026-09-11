#include "LogParser.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <set>

LogParser::LogParser(const std::string& filename) : filename_(filename)
{
}

time_t LogParser::toSeconds(const std::string& timeText)
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

std::vector<LogEntry> LogParser::parse()
{
    std::ifstream file(filename_);

    std::vector<LogEntry> entries;

    if (!file.is_open())
    {
        std::cerr << "File not found" << std::endl;
        return entries;                          // EDIT 2: was return 1
    }

    std::string line;
    int failCount = 0;
    int readLines = 0;
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

    std::cout << "Lines read: " << readLines << "\n" << "Failure lines: "
            << failCount << "\n" << "Unique attacker IPs: " << uniqueIps.size() << std::endl;

    return entries;
}