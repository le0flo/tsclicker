#ifndef RECORDER_H
#define RECORDER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>

class Recorder {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI recorder(LPVOID lpArg);
    bool is_cursor_visible();
    long long get_current_ms();

    bool running;
    std::vector<long long> intervals;

public:
    Recorder();

    void forcestop();

    int enable_recorder(bool toggle);
    void save_recorded_clicks();
};

#endif