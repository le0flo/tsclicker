#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#include "clicker.h"
#include "utils.h"

#define NINTERVALS 1000

bool running = false;
HANDLE clickerth = NULL;
DWORD clickerth_id;

int intervals[NINTERVALS] = { 0 };

DWORD WINAPI tsclicker_thread(LPVOID lpArg) {
    HWND foregroundWindow;
    
    int i = 0;

    while (running) {
        foregroundWindow = GetForegroundWindow();
        if(GetAsyncKeyState(VK_LBUTTON) && (FindWindowA(("LWJGL"), NULL) == foregroundWindow || FindWindowA(("GLFW30"), NULL) == foregroundWindow)){
            SendMessageW(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessageW(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
        }

        Sleep(intervals[i%NINTERVALS]);
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

 void tsclicker_intervals_fixed(int cps) {
    for (unsigned int i=0; i<NINTERVALS; i++) {
        intervals[i] = 1000 / cps;
    }
 }

 bool tsclicker_intervals_config() {
    char filepath[MAX_PATH] = "";
    tsclicker_utils_getlocalfilepath(filepath, "clicks.txt");

    FILE* file_p;
    char line[4];

    file_p = fopen(filepath, "r");

    if (file_p == NULL) {
        return false;
    }

    for (unsigned int i=0; i<NINTERVALS && fgets(line, sizeof(line) / sizeof(char), file_p) != NULL; i++) {
        intervals[i] = atoi(line) * 1.5;
    }

    fclose(file_p);

    return true;
 }