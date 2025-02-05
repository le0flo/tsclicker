/*
 * TeamSpeak 3 demo plugin | | TS clicker (alpha)
 *
 * Copyright (c) TeamSpeak Systems GmbH
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <Windows.h>

#include "teamspeak/public_definitions.h"
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_rare_definitions.h"
#include "ts3_functions.h"

#include "plugin.h"
#include "main_ui.h"

static struct TS3Functions ts3Functions;

#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = nullptr;
Clicker* clicker = nullptr;
Recorder* recorder = nullptr;
MainUi* main_ui = nullptr;

const char* ts3plugin_name() {
    return PLUGIN_NAME;
}

const char* ts3plugin_version() {
    return PLUGIN_VERSION;
}

int ts3plugin_apiVersion() {
    return PLUGIN_API_VERSION;
}

const char* ts3plugin_author() {
    return PLUGIN_AUTHOR;
}

const char* ts3plugin_description() {
    return PLUGIN_DESCRIPTION;
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

int ts3plugin_init() {
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
    char pluginPath[PATH_BUFSIZE];

    ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
    ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE, pluginID);

    plugin::intervals_path = std::string();

    clicker = new Clicker();
    recorder = new Recorder();
    main_ui = new MainUi(clicker, recorder);

    return 0;
}

void ts3plugin_shutdown() {
    main_ui->deleteLater();
    delete main_ui;

    clicker->forcestop();
    delete clicker;

    recorder->forcestop();
    delete recorder;

    if (pluginID) {
        free(pluginID);
        pluginID = nullptr;
    }
}

int ts3plugin_offersConfigure() {
    return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;
}

void ts3plugin_configure(void* handle, void* qParentWidget) {
    main_ui->show();
}

void ts3plugin_registerPluginID(const char* id) {
    const size_t sz = strlen(id) + 1;
    pluginID        = (char*)malloc(sz * sizeof(char));
    _strcpy(pluginID, sz, id);
}

void ts3plugin_freeMemory(void* data) {
    free(data);
}

static struct PluginHotkey* createHotkey(const char* keyword, const char* description) {
    struct PluginHotkey* hotkey = (struct PluginHotkey*)malloc(sizeof(struct PluginHotkey));
    _strcpy(hotkey->keyword, PLUGIN_HOTKEY_BUFSZ, keyword);
    _strcpy(hotkey->description, PLUGIN_HOTKEY_BUFSZ, description);
    return hotkey;
}

#define BEGIN_CREATE_HOTKEYS(x)                                                                                                                                                                                                                                \
    const size_t sz = x + 1;                                                                                                                                                                                                                                   \
    size_t       n  = 0;                                                                                                                                                                                                                                       \
    *hotkeys        = (struct PluginHotkey**)malloc(sizeof(struct PluginHotkey*) * sz);
#define CREATE_HOTKEY(a, b) (*hotkeys)[n++] = createHotkey(a, b);
#define END_CREATE_HOTKEYS                                                                                                                                                                                                                                     \
    (*hotkeys)[n++] = nullptr;                                                                                                                                                                                                                                    \
    assert(n == sz);

void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys) {
    BEGIN_CREATE_HOTKEYS(5);
    CREATE_HOTKEY("tsclicker_open_settings", "Open settings");
    CREATE_HOTKEY("tsclicker_toggle_clicker", "Toggle clicker");
    CREATE_HOTKEY("tsclicker_toggle_left_click", "Toggle left clicker");
    CREATE_HOTKEY("tsclicker_toggle_right_click", "Toggle right clicker");
    CREATE_HOTKEY("tsclicker_toggle_recorder", "Toggle recorder");
    END_CREATE_HOTKEYS;
}

// Event handlers

void ts3plugin_onHotkeyEvent(const char* keyword) {
    std::string hotkey = std::string(keyword);

    if (hotkey == "tsclicker_open_settings") {
        main_ui->show();
    } else if (hotkey == "tsclicker_toggle_clicker") {
        main_ui->get_clicker()->toggle_clicker();
    } else if (hotkey == "tsclicker_toggle_left_click") {
        main_ui->get_clicker()->toggle_click_left();
    } else if (hotkey == "tsclicker_toggle_right_click") {
        main_ui->get_clicker()->toggle_click_right();
    } else if (hotkey == "tsclicker_toggle_recorder") {
        main_ui->get_recorder()->toggle_recorder();
    }
}

// Utility functions

std::string plugin::data_folder() {
    char current_path[MAX_PATH] = { 0 };
    
    HMODULE module = nullptr;

    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) &ts3plugin_init, &module) == 0) return nullptr;

    int length = GetModuleFileNameA(module, current_path, MAX_PATH);

    if (length == 0) return nullptr;
    std::string result = std::string(current_path);

    int pos = result.find_last_of(".dll");
    
    return result.substr(0, pos - 3);
}

std::string plugin::desktop_folder() {
    char current_path[MAX_PATH] = { 0 };
    char current_file[MAX_PATH] = { 0 };

    HMODULE module = nullptr;

    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&ts3plugin_init, &module) == 0) return nullptr;

    int length = GetModuleFileNameA(module, current_path, MAX_PATH);

    if (length == 0) return nullptr;
    std::string result = std::string(current_path);
    
    std::string temp = "AppData\\Roaming\\TS3Client\\plugins\\";
    int pos = result.find_last_of("\\") - temp.size();

    return result.substr(0, pos).append("\\Desktop");
}