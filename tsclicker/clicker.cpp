#include "clicker.h"

Clicker::Clicker() {
    thread = nullptr;

    enable_clicker(false);
    enable_intervals(false);
    enable_click_left(true);
    enable_click_right(false);
    set_interval(16);
}

DWORD WINAPI Clicker::clicker(LPVOID lpArg) {
    Clicker* instance = static_cast<Clicker*>(lpArg);

    if (instance == nullptr) {
        return -1;
    }

    HWND foreground_window;
    bool is_window_minecraft;

    unsigned long long iteration = 0;

    while (instance->running) {
        foreground_window = GetForegroundWindow();
        is_window_minecraft = FindWindowA(("LWJGL"), nullptr) == foreground_window || FindWindowA(("GLFW30"), nullptr) == foreground_window;

        if (is_window_minecraft && GetAsyncKeyState(VK_LBUTTON) && instance->click_left) {
            SendMessageA(foreground_window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            instance->pressed_sleep(iteration);
            SendMessageA(foreground_window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
        }

        if (is_window_minecraft && GetAsyncKeyState(VK_RBUTTON) && instance->click_right) {
            SendMessageA(foreground_window, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
            instance->pressed_sleep(iteration);
            SendMessageA(foreground_window, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
        }

        instance->between_sleep(iteration);
        iteration++;
    }

    return 0;
}

void Clicker::pressed_sleep(int iteration) {
    if (using_intervals) {
        std::this_thread::sleep_for(std::chrono::milliseconds(pressed_intervals.at(iteration % pressed_intervals.size())));
    }
}

void Clicker::between_sleep(int iteration) {
    if (using_intervals) {
        std::this_thread::sleep_for(std::chrono::milliseconds(between_intervals.at(iteration % between_intervals.size())));
    } else {
        std::this_thread::sleep_for(std::chrono::nanoseconds(interval));
    }
}

void Clicker::forcestop() {
    if (running) {
        running = false;
        WaitForSingleObject(thread, INFINITE);
    }
}

// Setters

void Clicker::enable_clicker(bool toggle) {
    if (running == toggle) {
        return;
    }

    running = toggle;

    if (running) {
        thread = CreateThread(nullptr, 0, clicker, this, 0, nullptr);
        if (thread == nullptr) {
            running = false;
        }
    } else {
        WaitForSingleObject(thread, INFINITE);
    }
}

void Clicker::enable_intervals(bool toggle) {
    using_intervals = toggle;
}

void Clicker::enable_click_left(bool toggle) {
    click_left = toggle;
}

void Clicker::enable_click_right(bool toggle) {
    click_right = toggle;
}

void Clicker::set_interval(int cps) {
    this->interval = 1000000000 / cps;
}

void Clicker::set_intervals(std::vector<int> pressed, std::vector<int> between) {
    this->pressed_intervals = std::vector<int>(pressed);
    this->between_intervals = std::vector<int>(between);
}