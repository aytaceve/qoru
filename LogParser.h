#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "LogEntry.h"

class LogParser
{
public:
    LogParser(const std::string& filename);
    std::vector<LogEntry> parse();

private:
    std::string filename_;
    time_t toSeconds(const std::string& timeText);
};