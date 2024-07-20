#ifndef CLICKER_H
#define CLICKER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h>

#include "plugin.h"

class Clicker {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI clicker(LPVOID lpArg);

    bool running;

    int cps;
    bool click_left;
    bool click_right;

    bool using_recorded_clicks;
    std::vector<int> intervals;

public:
    Clicker();

    void forcestop();

    int enable_clicker(bool toggle);
    void set_cps(int cps);
    void enable_click_left(bool toggle);
    void enable_click_right(bool toggle);

    void enable_recorded_clicks(bool toggle);
    bool update_recorded_clicks();
};

#endif