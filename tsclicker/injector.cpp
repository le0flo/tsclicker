#include "injector.h"

Injector::Injector() {
	addons.clear();
}

DWORD Injector::get_process_id() {
	HWND window_lwjgl = FindWindowA("LWJGL", nullptr);
	HWND window_glfw30 = FindWindowA("GLFW30", nullptr);
	DWORD process_id = 0;

	GetWindowThreadProcessId(window_lwjgl, &process_id);
	if (process_id == 0) GetWindowThreadProcessId(window_glfw30, &process_id);

	return process_id;
}

void Injector::enable_addon(addon addon) {
    if (std::get<1>(addon)) inject(addon);
    else deject(addon);
}

bool Injector::inject(addon addon) {
    std::string addon_path = std::get<0>(addon);
    bool enabled = std::get<1>(addon);

    if (addon_path.empty() || !enabled) return false;

    // TODO

    /*
	DWORD pid = get_process_id();
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (process == INVALID_HANDLE_VALUE) return false;

    LPVOID location = VirtualAllocEx(process, 0, addon_path.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (location == nullptr) return false;

    if (WriteProcessMemory(process, location, addon_path.c_str(), addon_path.size() + 1, 0)) {
        CloseHandle(process);
        return false;
    }

    HANDLE thread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), location, 0, 0);
    WaitForSingleObject(thread, INFINITE);
    VirtualFreeEx(process, location, addon_path.size() + 1, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(process);
    */

	return true;
}

bool Injector::deject(addon addon) {
	DWORD pid = get_process_id();

    // TODO

	return true;
}

void Injector::enable_addons(bool toggle) {
    addon addon;
    
    for (int i = 0; i < addons.size(); i++) {
        addon = addons.at(i);
        std::get<1>(addon) = toggle;
        enable_addon(addon);
    }
}

void Injector::enable_addon(std::string addon_path, bool toggle) {
    addon addon;

    for (int i = 0; i < addons.size(); i++) {
        addon = addons.at(i);
        if (std::get<0>(addon).compare(addon_path) == 0) {
            std::get<1>(addon) = toggle;
            enable_addon(addon);
            break;
        }
    }
}

bool Injector::add_addon(std::string addon_path) {
    addon addon = std::tuple<std::string, bool>(addon_path, true);

    for (int i = 0; i < addons.size(); i++) {
        addon = addons.at(i);
        if (std::get<0>(addon).compare(addon_path) == 0) {
            return false;
        }
    }

    addons.push_back(addon);
    return true;
}

bool Injector::del_addon(std::string addon_path) {
    addon addon;

    for (int i = 0; i < addons.size(); i++) {
        addon = addons.at(i);
        if (std::get<0>(addon).compare(addon_path) == 0) {
            std::get<1>(addon) = false;
            enable_addon(addon);
            addons.erase(std::find(addons.begin(), addons.end(), addon));
            return true;
        }
    }

    return false;
}