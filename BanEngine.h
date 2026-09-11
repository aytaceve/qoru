#pragma once
#include <string>
#include <vector>
#include <map>
#include "LogEntry.h"

class BanEngine
{
public:
    BanEngine(int threshold, int windowMinutes);
    std::map<std::string, int> findBanned(const std::vector<LogEntry>& entries);

private:
    int threshold_;
    int windowSeconds_;
};