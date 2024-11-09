#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>

class Recorder {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI recorder(LPVOID lpArg);
    long long current_ms();

    bool running;
    std::vector<long long> intervals;

public:
    Recorder();
    void forcestop();
    bool toggle_recorder();
    std::vector<long long> get_intervals();
};

#endif