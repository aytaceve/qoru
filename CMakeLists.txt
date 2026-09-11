cmake_minimum_required(VERSION 4.3)
project(qoru)

set(CMAKE_CXX_STANDARD 23)

add_executable(qoru
        main.cpp
        LogParser.cpp
        LogParser.h
        LogEntry.cpp
        LogEntry.h
        BanEngine.cpp
        BanEngine.h
)
