#include "util/Log.hpp"

LogEntry* Log::entry = new LogEntry();

LogEntry::LogEntry() {
    data.reserve(10);
}

void LogEntry::startTimer(string key) {
    double startTime = Log::now();
    data[key] = startTime;
}

double LogEntry::stopTimer(string key) {
    double startTime = data[key];
    double stopTime = Log::now();
    double period = stopTime - startTime;
    data[key] = period;
    return period;
}

void LogEntry::finalize() {
    for ( auto it = data.begin(); it != data.end(); ++it ) {
        string key = it->first;
        double value = it->second;
        printf("(%s, %g), ", key.c_str(), value);
    }
    printf("\n");
}
