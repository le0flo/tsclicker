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
#include "configui.h"

static struct TS3Functions ts3Functions;

#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s

#define PLUGIN_API_VERSION 26
#define PLUGIN_NAME "TS clicker"
#define PLUGIN_VERSION "1.0"
#define PLUGIN_AUTHOR "Bestemmie"
#define PLUGIN_DESCRIPTION "Questo plugin si trasforma in un simpatico autoclicker per minecraft 1.8.9."

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

Clicker* clicker;
ConfigUi* config_ui;

enum {
    TSCLICKER_GLOBAL_TOGGLE = 1,
    TSCLICKER_GLOBAL_RELOAD
};

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
    config_ui = new ConfigUi(clicker);
    config_ui->save_settings();

    return 0;
}

void ts3plugin_shutdown() {
    config_ui->deleteLater();
    delete config_ui;

    clicker->forcestop();
    delete clicker;

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

static struct PluginMenuItem* createMenuItem(enum PluginMenuType type, int id, const char* text, const char* icon) {
    struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
    menuItem->type = type;
    menuItem->id = id;
    _strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
    _strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
    return menuItem;
}

#define BEGIN_CREATE_MENUS(x)                                                                                                                                                                                                                                  \
    const size_t sz = x + 1;                                                                                                                                                                                                                                   \
    size_t       n  = 0;                                                                                                                                                                                                                                       \
    *menuItems      = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS                                                                                                                                                                                                                                       \
    (*menuItems)[n++] = NULL;                                                                                                                                                                                                                                  \
    assert(n == sz);

void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon) {

    BEGIN_CREATE_MENUS(2);
    CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, TSCLICKER_GLOBAL_TOGGLE, "Toggle", "");
    CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, TSCLICKER_GLOBAL_RELOAD, "Reload", "");
    END_CREATE_MENUS;

    *menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
    _strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "");
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
    BEGIN_CREATE_HOTKEYS(2);
    CREATE_HOTKEY("tsclicker_toggle", "Toggle");
    CREATE_HOTKEY("tsclicker_reload", "Reload");
    END_CREATE_HOTKEYS;
}

/************************** Callbacks ***************************/

void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
    switch (type) {
        case PLUGIN_MENU_TYPE_GLOBAL:
            switch (menuItemID) {
                case TSCLICKER_GLOBAL_TOGGLE: {
                    tsclicker_toggle();
                    break;
                }
                case TSCLICKER_GLOBAL_RELOAD: {
                    tsclicker_reload();
                    break;
                }
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void ts3plugin_onHotkeyEvent(const char* keyword) {
    std::string hotkey = std::string(keyword);

    if (hotkey == "tsclicker_toggle") {
        tsclicker_toggle();
    } else if (hotkey == "tsclicker_reload") {
        tsclicker_reload();
    }
}

/************************** Wrappers ***************************/

void tsclicker_toggle() {
    bool status = clicker->toggle();
    
    std::string message = "TS clicker | ";
    message += (status ? "on" : "off");
    ts3Functions.printMessageToCurrentTab(message.c_str());
}

void tsclicker_reload() {
    bool status = clicker->update_recorded_clicks();
    
    std::string message = "TS clicker | ";
    message += (status ? "caricato" : "non caricato");
    ts3Functions.printMessageToCurrentTab(message.c_str());
}