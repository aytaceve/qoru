#include <iostream>
#include <string>
#include "LogParser.h"
#include "BanEngine.h"
#include <fstream>
#include <ctime>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: qoru <logfile> [--threshold N] [--window MINUTES]\n";
        return 1;
    }

    std::string logfile = argv[1];
    int threshold = 5;
    int windowMinutes = 10;

    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--threshold" && i + 1 < argc)
        {
            threshold = std::stoi(argv[i + 1]);
            i++;
        }
        else if (arg == "--window" && i + 1 < argc)
        {
            windowMinutes = std::stoi(argv[i + 1]);
            i++;
        }
        else
        {
            std::cerr << "Unknown or incomplete option: " << arg << "\n";
            return 1;
        }
    }

    LogParser parser(logfile);
    std::vector<LogEntry> entries = parser.parse();

    BanEngine engine(threshold, windowMinutes);
    std::map<std::string, int> banned = engine.findBanned(entries);

    std::ofstream banLog("qoru.log", std::ios::app);

    time_t now = time(nullptr);
    char timeBuf[20];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    for (const auto& [ip, count] : banned)
    {
        std::cout << "BANNED " << ip << " (" << count << " failures)\n";
        banLog << timeBuf << " BANNED " << ip << " (" << count << " failures)\n";
    }
    return 0;
}