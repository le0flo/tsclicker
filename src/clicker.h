#ifndef CLICKER_H
#define CLICKER_H

#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>

#include "plugin.h"

class Clicker {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI clicker(LPVOID lpArg);
    void pressed_sleep(int iteration);
    void between_sleep(int iteration);

    bool running;
    bool using_intervals;
    bool click_left;
    bool click_right;
    int interval;
    std::vector<int> pressed_intervals;
    std::vector<int> between_intervals;

public:
    Clicker();
    void forcestop();

    void enable_clicker(bool toggle);
    void enable_intervals(bool toggle);
    void enable_click_left(bool toggle);
    void enable_click_right(bool toggle);
    void set_interval(int cps);
    void set_intervals(std::vector<int> pressed, std::vector<int> between);
};

#endif