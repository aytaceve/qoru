#include <iostream>
#include <fstream>
#include "parser.h"
#include <vector>
#include <set>

struct LogEntry { std::string ip; std::string timeText; };

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

            uniqueIps.insert(ip);
            entries.push_back(entry);
        }
    }

    std::cout << "Lines read: " << readLines << "\n" << "Failure lines: "
            << failCount << "\n" << "Unique attacker IPs: " << uniqueIps.size() << std::endl;
    std::cout << entries.size() << std::endl;

    for (const auto& e: entries)
    {
        std::cout << e.timeText << "  " << e.ip << "\n";
    }

    Parse();
}