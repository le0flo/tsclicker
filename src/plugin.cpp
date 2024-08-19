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
    config_ui->save_settings();

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
    BEGIN_CREATE_HOTKEYS(5);
    CREATE_HOTKEY("tsclicker_toggle", "Toggle");
    CREATE_HOTKEY("tsclicker_toggle_left_click", "Toggle left click");
    CREATE_HOTKEY("tsclicker_toggle_right_click", "Toggle right click");
    CREATE_HOTKEY("tsclicker_toggle_recorded_clicks", "Toggle recorded clicks");
    CREATE_HOTKEY("tsclicker_reload", "Reload recorded clicks");
    END_CREATE_HOTKEYS;
}

// Event handlers

void ts3plugin_onHotkeyEvent(const char* keyword) {
    std::string hotkey = std::string(keyword);

    if (hotkey == "tsclicker_toggle") {
        tsclicker_toggle();
    } else if (hotkey == "tsclicker_toggle_left_click") {
        tsclicker_toggle_left_click();
    } else if (hotkey == "tsclicker_toggle_right_click") {
        tsclicker_toggle_right_click();
    } else if (hotkey == "tsclicker_toggle_recorded_clicks") {
        tsclicker_toggle_recorded_clicks();
    } else if (hotkey == "tsclicker_reload") {
        tsclicker_reload_recorded_clicks();
    }
}

// Hotkeys callbacks

std::string tsclicker_plugin_data_folder() {
    char current_path[MAX_PATH] = { 0 };
    
    HMODULE module = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) &ts3plugin_init, &module) == 0) {
        return false;
    }

    int length = GetModuleFileNameA(module, current_path, MAX_PATH);

    if (length == 0) {
        return false;
    }

    std::string::size_type pos = std::string(current_path).find_last_of(".dll");
    std::string result = std::string(current_path).substr(0, pos-3);
    
    return result;
}

void tsclicker_toggle() {
    bool status = config_ui->get_clicker();
    status = !status;
    config_ui->set_clicker(status);
}

void tsclicker_toggle_left_click() {
    bool status = config_ui->get_click_left();
    status = !status;
    config_ui->set_click_left(status);
}

void tsclicker_toggle_right_click() {
    bool status = config_ui->get_click_right();
    status = !status;
    config_ui->set_click_right(status);
}

void tsclicker_toggle_recorded_clicks() {
    bool status = config_ui->get_recorded();
    status = !status;
    config_ui->set_recorded(status);
}

void tsclicker_reload_recorded_clicks() {
    clicker->update_recorded_clicks();
}