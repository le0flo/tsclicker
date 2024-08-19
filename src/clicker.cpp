#include "clicker.h"

Clicker::Clicker() {
    running = false;

    cps = 16;
    click_left = true;
    click_right = false;

    using_recorded_clicks = false;
}

DWORD WINAPI Clicker::clicker(LPVOID lpArg) {
    Clicker* instance = static_cast<Clicker*>(lpArg);

    if (instance == NULL) {
        return -1;
    }

    HWND foreground_window;
    unsigned int iteration = 0;
    bool is_window_minecraft;

    while (instance->running) {
        foreground_window = GetForegroundWindow();
        is_window_minecraft = FindWindowA(("LWJGL"), NULL) == foreground_window || FindWindowA(("GLFW30"), NULL) == foreground_window;

        if (is_window_minecraft) {
            if (GetAsyncKeyState(VK_LBUTTON) && instance->click_left) {
                SendMessageA(foreground_window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
            }

            if (GetAsyncKeyState(VK_RBUTTON) && instance->click_right) {
                SendMessageA(foreground_window, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
            }
        }
        
        instance->sleep(&iteration);
    }

    return 0;
}

void Clicker::sleep(unsigned int* iteration) {
    if (using_recorded_clicks) {
        std::this_thread::sleep_for(std::chrono::milliseconds(intervals[*iteration % intervals.size()]));
        iteration++;
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / cps));
    }
}

void Clicker::forcestop() {
    if (running) {
        running = false;
        WaitForSingleObject(thread, INFINITE);
    }
}

bool Clicker::update_recorded_clicks() {
    std::string clicks_file_path = tsclicker_plugin_data_folder().append("\\clicks.txt");

    std::fstream clicks_file;
    clicks_file.open(clicks_file_path, std::ios::in);

    if (clicks_file.is_open()) {
        intervals.clear();
        std::string line;

        while (std::getline(clicks_file, line)) {
            int interval = atoi(line.c_str());
            intervals.push_back(interval);
        }

        return true;
    }

    return false;
}

// Getters and setters

int Clicker::enable_clicker(bool toggle) {
    if (running == toggle) {
        return 0;
    }

    running = toggle;

    if (running) {
        thread = CreateThread(NULL, 0, clicker, this, 0, &thread_id);
        if (thread == NULL) {
            running = false;
            return -1;
        }
    } else {
        WaitForSingleObject(thread, INFINITE);
    }

    return 1;
}

void Clicker::set_cps(int cps) {
    this->cps = cps;
}

void Clicker::enable_click_left(bool toggle) {
    click_left = toggle;
}

void Clicker::enable_click_right(bool toggle) {
    click_right = toggle;
}

void Clicker::enable_recorded_clicks(bool toggle) {
    using_recorded_clicks = toggle;

    if (using_recorded_clicks) {
        update_recorded_clicks();
    }
}