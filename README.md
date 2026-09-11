# qoru

qoru means "protect!" in Azerbaijani. It is a small tool that reads SSH
auth logs, detects brute force attacks and bans the attacker IPs.


## Why

This is my first C++ project. I built it from scratch to learn the
language, after four years as a software engineer in other languages.
The idea comes from fail2ban.


## Build and run

Requires a C++23 compiler and CMake 3.28+.

    cmake -B build
    cmake --build build
    ./build/qoru sample_auth.log --threshold 5 --window 10

`--threshold` is the number of failures that triggers a ban. Default is 5. 
`--window` is the time window in minutes. Default is 10.

## Example output

`
Lines read: 32
Failure lines: 23
Unique attacker IPs: 6
BANNED 203.0.113.45 (7 failures)
BANNED 203.0.113.66 (6 failures)
`

## How it works

qoru reads the log file and takes the IP and the time from every failed
login line. For every IP it keeps a list of the failure times. When a new
failure comes, it drops the stored times that are older than the window.
If the times that are left reach the threshold, the IP is banned.

So 5 failures in 4 minutes is a ban, but 5 failures spread over 3 hours is normal and won't banned.

## Limitations for now

- The log format has no year, so qoru assumes the current year.
- Bans are detected and logged. Not a real BAN.
- Qoru does not yet remember past bans between runs.

