#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#include "clicker.h"

bool running = true;
bool status = true;

bool tsclicker_toggle() {
    status = !status;
    return status;
}

void *tsclicker_thread(void *args) {
    while (running) {
        if(GetAsyncKeyState(VK_LBUTTON) && status){
            if(FindWindowA(("LWJGL"), NULL) == GetForegroundWindow()) {
                
                SendMessageW(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageW(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));

                Sleep(20);
            }
        }
    }
}