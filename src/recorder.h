#ifndef RECORDER_H
#define RECORDER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <QtCore\Qt>
#include <QtWidgets\QWidget>
#include <QtWidgets\QFileDialog>

class Recorder {
private:
    HANDLE thread;
    DWORD thread_id;

    static DWORD WINAPI recorder(LPVOID lpArg);
    bool is_cursor_visible();
    long long current_ms();

    bool running;
    std::vector<long long> intervals;

public:
    Recorder();
    void forcestop();
    bool toggle_recorder();
    void save_intervals(QWidget* parent, QString path);
};

#endif