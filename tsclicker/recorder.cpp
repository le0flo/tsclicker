#include "recorder.h"

Recorder::Recorder() { 
    thread = nullptr;

    enable_recorder(false);
    clear_intervals();
}

DWORD WINAPI Recorder::recorder(LPVOID lpArg) {
    Recorder* instance = static_cast<Recorder*>(lpArg);

    if (instance == nullptr) {
        return -1;
    }

    HWND foreground_window;
    bool is_window_minecraft;

    instance->clear_intervals();

    while (instance->running) {
        foreground_window = GetForegroundWindow();
        is_window_minecraft = FindWindowA(("LWJGL"), nullptr) == foreground_window || FindWindowA(("GLFW30"), nullptr) == foreground_window;

        if (GetAsyncKeyState(VK_LBUTTON) && is_window_minecraft) {
            instance->intervals.push_back(instance->current_ms());

            while (GetAsyncKeyState(VK_LBUTTON)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            instance->intervals.push_back(instance->current_ms());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    instance->intervals.erase(instance->intervals.begin(), instance->intervals.begin()+4);
    instance->intervals.erase(instance->intervals.end()-4, instance->intervals.end());

    return 0;
}

long long Recorder::current_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Recorder::forcestop() {
    if (running) {
        running = false;
        WaitForSingleObject(thread, INFINITE);
    }
}

void Recorder::enable_recorder(bool toggle) {
    if (running == toggle) {
        return;
    }

    running = toggle;

    if (running) {
        thread = CreateThread(nullptr, 0, recorder, this, 0, nullptr);
        if (thread == nullptr) {
            running = false;
        }
    }
    else {
        WaitForSingleObject(thread, INFINITE);
    }
}

// Getters

std::vector<long long> Recorder::get_intervals() {
    return intervals;
}

// Setters

void Recorder::clear_intervals() {
    intervals.clear();
}