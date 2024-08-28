#include "clicker.h"

Clicker::Clicker() {
    running = false;
    using_intervals = false;
    click_left = true;
    click_right = false;
    cps = 16;
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
        // TODO reimplementare la riproduzione dei click con l'intervallo tra un click e un altro + durata del tasto premuto

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
    if (using_intervals) {
        std::this_thread::sleep_for(std::chrono::milliseconds(intervals[*iteration % intervals.size()]));
        (*iteration)++;
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

void Clicker::sync(QSettings* settings) {
    enable_clicker(settings->value("clicker_toggle").toBool());
    enable_click_left(settings->value("click_left").toBool());
    enable_click_right(settings->value("click_right").toBool());
    set_cps(settings->value("cps").toInt());
}

// Setters

void Clicker::enable_clicker(bool toggle) {
    if (running == toggle) {
        return;
    }

    running = toggle;

    if (running) {
        thread = CreateThread(NULL, 0, clicker, this, 0, &thread_id);
        if (thread == NULL) {
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

void Clicker::set_cps(int cps) {
    this->cps = cps;
}

void Clicker::set_intervals(std::vector<int> intervals) {
    this->intervals = std::vector<int>(intervals);
}