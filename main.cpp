#include <iostream>
#include "LogParser.h"
#include "BanEngine.h"

const int threshold = 5;
const int windowMinutes = 10;

int main()
{
    LogParser parser("sample_auth.log");
    std::vector<LogEntry> entries = parser.parse();

    BanEngine engine(threshold, windowMinutes);
    std::map<std::string, int> banned = engine.findBanned(entries);

    for (const auto& [ip, count] : banned)
    {
        std::cout << "BANNED " << ip << " (" << count << " failures)\n";
    }
    return 0;
}