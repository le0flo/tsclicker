#include "clicker.h"

Clicker::Clicker() {
    this->running = false;

    this->using_recorded_clicks = false;

    this->cps = 16;
    this->left_click = true;
    this->right_click = false;

    this->intervals.push_back(1000 / cps);
}

DWORD WINAPI Clicker::clicker(LPVOID lpArg) {
    Clicker* instance = static_cast<Clicker*>(lpArg);

    if (instance == NULL) {
        return -1;
    }

    HWND foreground_window;
    int iteration = 0;

    while (instance->running) {
        foreground_window = GetForegroundWindow();

        if ((FindWindowA(("LWJGL"), NULL) == foreground_window || FindWindowA(("GLFW30"), NULL) == foreground_window)) {
            if (GetAsyncKeyState(VK_LBUTTON) && instance->left_click) {
                SendMessageA(foreground_window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
            }

            if (GetAsyncKeyState(VK_RBUTTON) && instance->right_click) {
                SendMessageA(foreground_window, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
            }
        }
        
        if (instance->using_recorded_clicks) {
            Sleep(instance->intervals[iteration % instance->intervals.size()]);
            iteration++;
        } else {
            Sleep(1000 / instance->cps);
        }
    }

    return 0;
}

bool Clicker::toggle() {
    if (running) {
        running = false;
        WaitForSingleObject(thread, INFINITE);
    } else {
        running = true;
        thread = CreateThread(NULL, 0, clicker, this, 0, &thread_id);
        if (thread == NULL) {
            running = false;
        }
    }
    
    return running;
}

void Clicker::use_recorded_clicks(bool using_recorded_clicks) {
    this->using_recorded_clicks = using_recorded_clicks;

    if (this->using_recorded_clicks) {
        update_recorded_clicks();
    }
}

bool Clicker::update_recorded_clicks() {
    char current_path[MAX_PATH] = { 0 };
    
    HMODULE module = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) &dummy_function, &module) == 0) {
        return false;
    }

    int length = GetModuleFileNameA(module, current_path, MAX_PATH);

    if (length == 0) {
        return false;
    }

    std::string::size_type pos = std::string(current_path).find_last_of("\\/");
    std::string clicks_file_path = std::string(current_path).substr(0, pos).append("\\clicks.txt");

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

void Clicker::set_cps(int cps) {
    this->cps = cps;
}

void Clicker::set_left_click(bool left_click) {
    this->left_click = left_click;
}

void Clicker::set_right_click(bool right_click) {
    this->right_click = right_click;
}

void dummy_function() {
    std::cout << "leo was here" << std::endl;
}