#ifndef CLICKER_H
#define CLICKER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h>

class Clicker {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI clicker(LPVOID lpArg);

    bool running;
    
    bool using_recorded_clicks;
    std::vector<int> intervals;

    int cps = 16;
    bool left_click;
    bool right_click;

public:
    Clicker();

    bool toggle();

    void use_recorded_clicks(bool using_recorded_clicks);
    bool update_recorded_clicks();

    void set_cps(int cps);
    void set_left_click(bool left_click);
    void set_right_click(bool right_click);
};

void dummy_function();

#endif