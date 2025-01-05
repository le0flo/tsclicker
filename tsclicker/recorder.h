#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>

class Recorder {
private:
    HANDLE thread;
    static DWORD WINAPI recorder(LPVOID lpArg);

    bool running;
    std::vector<long long> intervals;

    long long current_ms();

public:
    Recorder();
    void forcestop();

    void enable_recorder(bool toggle);
    std::vector<long long> get_intervals();
    void clear_intervals();
};

#endif