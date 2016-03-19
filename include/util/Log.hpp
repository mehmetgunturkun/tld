#ifndef LOG_H
#define LOG_H
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <chrono>

using namespace std;

class LogEntry {
private:
    unordered_map<string, double> data;
public:
    LogEntry();
    void startTimer(string key);
    double stopTimer(string key);
    void finalize();
};

class Log {
public:
    static LogEntry* entry;
    static bool init() {
        return true;
    }

    void static startTimer(string key) {
        entry->startTimer(key);
    }

    double static stopTimer(string key) {
        return entry->stopTimer(key);
    }

    void static finalize() {
        entry->finalize();
        free(entry);
        entry = new LogEntry();
    }

    static double now() {
        std::chrono::system_clock::time_point currentTime = chrono::system_clock::now();
        return double(currentTime.time_since_epoch().count()) / 1000;
    }
};
#endif
