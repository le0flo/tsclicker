#include <iostream>
#include <time.h>
#include <Windows.h>

#include "clicker.h"
#include "utils.h"

bool running = false, left_click = true, right_click = false, fixed = true;

int cps = 16;
int intervals[N_INTERVALS] = { 0 };

HANDLE clickerth;
DWORD clickerth_id;

DWORD WINAPI tsclicker_thread(LPVOID lpArg) {
    HWND foreground_window;
    
    int i = 0;

    while (running) {
        foreground_window = GetForegroundWindow();
        if ((FindWindowA(("LWJGL"), NULL) == foreground_window || FindWindowA(("GLFW30"), NULL) == foreground_window)) {
            if (GetAsyncKeyState(VK_LBUTTON) && left_click) {
                SendMessageA(foreground_window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
            }

            if (GetAsyncKeyState(VK_RBUTTON) && right_click) {
                SendMessageA(foreground_window, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
                SendMessageA(foreground_window, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
            }
        }
        
        if (fixed) {
            Sleep(1000 / cps);
        } else {
            Sleep(intervals[i % N_INTERVALS]);
        }
        
        i++;
    }

    return 0;
}

bool tsclicker_toggle() {
    if (running) {
        running = false;
        WaitForSingleObject(clickerth, INFINITE);
    } else {
        running = true;
        clickerth = CreateThread(NULL, 0, tsclicker_thread, NULL, 0, &clickerth_id);
        if (clickerth == NULL) {
            running = false;
        }
    }
    
    return running;
}

void tsclicker_intervals_setmode(bool isfixed) {
    fixed = isfixed;
}

void tsclicker_intervals_fixed_update(int updatedcps) {
    cps = updatedcps;
}

bool tsclicker_intervals_config_update() {
    char filepath[MAX_PATH] = "";
    tsclicker_utils_getlocalfilepath(filepath, "clicks.txt");

    FILE* file_p;
    char line[4];

    file_p = fopen(filepath, "r");

    if (file_p == NULL) {
        return false;
    }

    for (unsigned int i=0; i < N_INTERVALS && fgets(line, sizeof(line) / sizeof(char), file_p) != NULL; i++) {
        intervals[i] = atoi(line);
    }

    fclose(file_p);

    return true;
}