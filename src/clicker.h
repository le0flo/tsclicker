#ifndef CLICKER_H
#define CLICKER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <QtCore\QSettings>

#include "plugin.h"

class Clicker {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI clicker(LPVOID lpArg);
    void sleep(unsigned int* iteration);

    bool running;
    bool using_intervals;
    bool click_left;
    bool click_right;
    int cps;
    std::vector<int> intervals;

public:
    Clicker();
    void forcestop();
    void sync(QSettings* settings);

    void enable_clicker(bool toggle);
    void enable_intervals(bool toggle);
    void enable_click_left(bool toggle);
    void enable_click_right(bool toggle);
    void set_cps(int cps);
    void set_intervals(std::vector<int> intervals);
};

#endif