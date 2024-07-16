/*
 * TeamSpeak 3 demo plugin | | TS clicker (alpha)
 *
 * Copyright (c) TeamSpeak Systems GmbH
 */

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#pragma warning(disable : 4100)
#include <Windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "teamspeak/public_definitions.h"
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_rare_definitions.h"
#include "ts3_functions.h"

#include "plugin.h"

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src)                                                                                                                                                                                                                           \
    {                                                                                                                                                                                                                                                          \
        strncpy(dest, src, destSize - 1);                                                                                                                                                                                                                      \
        (dest)[destSize - 1] = '\0';                                                                                                                                                                                                                           \
    }
#endif

#define PLUGIN_API_VERSION 26

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

#ifdef _WIN32

// Helper function to convert wchar_T to Utf-8 encoded strings on Windows
static int wcharToUtf8(const wchar_t* str, char** result) {
    int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
    *result    = (char*)malloc(outlen);
    if (WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
        *result = NULL;
        return -1;
    }
    return 0;
}

#endif

/*********************************** Required functions ************************************/

// Unique name identifying this plugin
const char* ts3plugin_name() {
#ifdef _WIN32
    static char* result = NULL;
    if (!result) {
        const wchar_t* name = L"TS clicker (alpha)";
        if (wcharToUtf8(name, &result) == -1) {
            result = "TS clicker (alpha)";
        }
    }
    return result;
#else
    return "TS clicker (alpha)";
#endif
}

// Plugin version
const char* ts3plugin_version() {
    return "0.1";
}

// Plugin API version. Must be the same as the clients API major version, else the plugin fails to load.
int ts3plugin_apiVersion() {
    return PLUGIN_API_VERSION;
}

// Plugin author
const char* ts3plugin_author() {
#ifdef _WIN32
    static char* result = NULL;
    if (!result) {
        const wchar_t* name = L"Bestemmie";
        if (wcharToUtf8(name, &result) == -1) {
            result = "Bestemmie";
        }
    }
    return result;
#else
    return "Bestemmie";
#endif
}

// Plugin description
const char* ts3plugin_description() {
#ifdef _WIN32
    static char* result = NULL;
    if (!result) {
        const wchar_t* name = L"Questo plugin è un simpatico autoclicker per minecraft 1.8.9.";
        if (wcharToUtf8(name, &result) == -1) {
            result = "Questo plugin è un simpatico autoclicker per minecraft 1.8.9.";
        }
    }
    return result;
#else
    return "Questo plugin è un simpatico autoclicker per minecraft 1.8.9.";
#endif
}

// Set TeamSpeak 3 callback functions
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

// Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
// If the function returns 1 on failure, the plugin will be unloaded again.
int ts3plugin_init() {
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
    char pluginPath[PATH_BUFSIZE];

    printf("PLUGIN: init\n");

    ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
    ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE, pluginID);

    printf("PLUGIN: App path: %s\nResources path: %s\nConfig path: %s\nPlugin path: %s\n", appPath, resourcesPath, configPath, pluginPath);

    return 0;
}

// Custom code called right before the plugin is unloaded
void ts3plugin_shutdown() {
    /* Your plugin cleanup code here */
    printf("PLUGIN: shutdown\n");

    if (pluginID) {
        free(pluginID);
        pluginID = NULL;
    }
}

/************************** Optionals ***************************/

// Enables the configuration page
int ts3plugin_offersConfigure() {
    printf("PLUGIN: offersConfigure\n");
    
    return PLUGIN_OFFERS_NO_CONFIGURE;
}

// Register id for menu items and hotkeys
void ts3plugin_registerPluginID(const char* id) {
    const size_t sz = strlen(id) + 1;
    pluginID        = (char*)malloc(sz * sizeof(char));
    _strcpy(pluginID, sz, id); /* The id buffer will invalidate after exiting this function */
    printf("PLUGIN: registerPluginID: %s\n", pluginID);
}

// Necessary for menu items
void ts3plugin_freeMemory(void* data) {
    free(data);
}

/************************** TeamSpeak menus ***************************/

// Helper function to create a menu item
static struct PluginMenuItem* createMenuItem(enum PluginMenuType type, int id, const char* text, const char* icon) {
    struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
    menuItem->type = type;
    menuItem->id = id;
    _strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
    _strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
    return menuItem;
}

// Some macros to make the code to create menu items a bit more readable
#define BEGIN_CREATE_MENUS(x)                                                                                                                                                                                                                                  \
    const size_t sz = x + 1;                                                                                                                                                                                                                                   \
    size_t       n  = 0;                                                                                                                                                                                                                                       \
    *menuItems      = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS                                                                                                                                                                                                                                       \
    (*menuItems)[n++] = NULL;                                                                                                                                                                                                                                  \
    assert(n == sz);

// Menu ids
enum {
    TSCLICKER_MENU_TOGGLE = 1,
    TSCLICKER_MENU_RELOAD
};

void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon) {

    BEGIN_CREATE_MENUS(2);
    CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, TSCLICKER_MENU_TOGGLE, "Toggle", "1.png");
    CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, TSCLICKER_MENU_RELOAD, "Reload click config", "2.png");
    END_CREATE_MENUS;

    *menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
    _strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "t.png");
}

/************************** TeamSpeak hotkeys ***************************/

// Helper function to create a hotkey
static struct PluginHotkey* createHotkey(const char* keyword, const char* description) {
    struct PluginHotkey* hotkey = (struct PluginHotkey*)malloc(sizeof(struct PluginHotkey));
    _strcpy(hotkey->keyword, PLUGIN_HOTKEY_BUFSZ, keyword);
    _strcpy(hotkey->description, PLUGIN_HOTKEY_BUFSZ, description);
    return hotkey;
}

// Some macros to make the code to create hotkeys a bit more readable
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
    CREATE_HOTKEY("tsclicker_reload", "Reload click config");
    END_CREATE_HOTKEYS;
}

/************************** TeamSpeak callbacks ***************************/

void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
    printf("PLUGIN: onMenuItemEvent: serverConnectionHandlerID=%llu, type=%d, menuItemID=%d, selectedItemID=%llu\n", (long long unsigned int)serverConnectionHandlerID, type, menuItemID, (long long unsigned int)selectedItemID);
    switch (type) {
        case PLUGIN_MENU_TYPE_GLOBAL:
            switch (menuItemID) {
                case TSCLICKER_MENU_TOGGLE: {
                    // TODO
                }
                break;
                case TSCLICKER_MENU_RELOAD: {
                    // TODO
                }
                break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void ts3plugin_onHotkeyEvent(const char* keyword) {
    printf("PLUGIN: Hotkey event: %s\n", keyword);

    if (keyword == "tsclicker_toggle") {
        // TODO
    } else if (keyword == "tsclicker_reload") {
        // TODO
    }
}