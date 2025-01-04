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
#include "clicker.h"
#include "recorder.h"
#include "configui.h"

static struct TS3Functions ts3Functions;

#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

Clicker* clicker;
Recorder* recorder;
ConfigUi* config_ui;

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

    clicker = new Clicker();
    recorder = new Recorder();
    config_ui = new ConfigUi(clicker, recorder);

    return 0;
}

void ts3plugin_shutdown() {
    config_ui->deleteLater();
    delete config_ui;

    clicker->forcestop();
    delete clicker;

    recorder->forcestop();
    delete recorder;

    if (pluginID) {
        free(pluginID);
        pluginID = NULL;
    }
}

int ts3plugin_offersConfigure() {
    return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;
}

void ts3plugin_configure(void* handle, void* qParentWidget) {
    config_ui->show();
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
    (*hotkeys)[n++] = NULL;                                                                                                                                                                                                                                    \
    assert(n == sz);

void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys) {
    BEGIN_CREATE_HOTKEYS(3);
    CREATE_HOTKEY("tsclicker_toggle", "Toggle clicker");
    CREATE_HOTKEY("tsclicker_enable_left_click", "Enable/Disable left clicker");
    CREATE_HOTKEY("tsclicker_enable_right_click", "Enable/Disable right clicker");
    END_CREATE_HOTKEYS;
}

// Event handlers

void ts3plugin_onHotkeyEvent(const char* keyword) {
    std::string hotkey = std::string(keyword);

    if (hotkey == "tsclicker_toggle") {
        config_ui->toggle_clicker();
    } else if (hotkey == "tsclicker_enable_left_click") {
        config_ui->toggle_click_left();
    } else if (hotkey == "tsclicker_enable_right_click") {
        config_ui->toggle_click_right();
    }
}

// Utility functions

std::string tsclicker_plugin_data_folder() {
    char current_path[MAX_PATH] = { 0 };
    
    HMODULE module = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) &ts3plugin_init, &module) == 0) {
        return nullptr;
    }

    int length = GetModuleFileNameA(module, current_path, MAX_PATH);

    if (length == 0) {
        return nullptr;
    }

    std::string::size_type pos = std::string(current_path).find_last_of(".dll");
    std::string result = std::string(current_path).substr(0, pos-3);
    
    return result;
}