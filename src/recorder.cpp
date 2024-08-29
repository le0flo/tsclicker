#include "recorder.h"

Recorder::Recorder() {
    running = false;
    intervals.clear();
}

DWORD WINAPI Recorder::recorder(LPVOID lpArg) {
    Recorder* instance = static_cast<Recorder*>(lpArg);

    if (instance == NULL) {
        return -1;
    }

    HWND foreground_window;
    bool is_window_minecraft;

    instance->intervals.clear();

    while (instance->running) {
        foreground_window = GetForegroundWindow();
        is_window_minecraft = FindWindowA(("LWJGL"), NULL) == foreground_window || FindWindowA(("GLFW30"), NULL) == foreground_window;

        if (GetAsyncKeyState(VK_LBUTTON) && is_window_minecraft && !instance->is_cursor_visible()) {
            instance->intervals.push_back(instance->current_ms());

            while (GetAsyncKeyState(VK_LBUTTON)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            instance->intervals.push_back(instance->current_ms());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}

bool Recorder::is_cursor_visible() {
    CURSORINFO ci = {sizeof(CURSORINFO)};

    if (GetCursorInfo(&ci)) {
        if (ci.flags == 0 || ci.flags == 2) {
            return true;
        }
    }

    return false;
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

bool Recorder::toggle_recorder() {
    running = !running;

    if (running) {
        thread = CreateThread(NULL, 0, recorder, this, 0, &thread_id);
        if (thread == NULL) {
            running = false;
        }
    } else {
        WaitForSingleObject(thread, INFINITE);
    }

    return running;
}

std::vector<long long> Recorder::get_intervals() {
    return intervals;
}