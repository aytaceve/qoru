#pragma once
#include <string>
#include <ctime>

struct LogEntry
{
    std::string ip;
    std::string timeText;
    time_t seconds;
};
