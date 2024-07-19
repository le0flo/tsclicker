/*
 * TeamSpeak 3 demo plugin | TS clicker (alpha)
 *
 * Copyright (c) TeamSpeak Systems GmbH
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#define PLUGINS_EXPORTDLL __declspec(dllexport)

extern "C" {

PLUGINS_EXPORTDLL const char* ts3plugin_name();
PLUGINS_EXPORTDLL const char* ts3plugin_version();
PLUGINS_EXPORTDLL int ts3plugin_apiVersion();
PLUGINS_EXPORTDLL const char* ts3plugin_author();
PLUGINS_EXPORTDLL const char* ts3plugin_description();
PLUGINS_EXPORTDLL void ts3plugin_setFunctionPointers(const struct TS3Functions funcs);
PLUGINS_EXPORTDLL int ts3plugin_init();
PLUGINS_EXPORTDLL void ts3plugin_shutdown();
PLUGINS_EXPORTDLL int ts3plugin_offersConfigure();
PLUGINS_EXPORTDLL void ts3plugin_configure(void* handle, void* qParentWidget);
PLUGINS_EXPORTDLL void ts3plugin_registerPluginID(const char* id);
PLUGINS_EXPORTDLL void ts3plugin_freeMemory(void* data);
PLUGINS_EXPORTDLL void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon);
PLUGINS_EXPORTDLL void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys);
PLUGINS_EXPORTDLL void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID);
PLUGINS_EXPORTDLL void ts3plugin_onHotkeyEvent(const char* keyword);

}

void tsclicker_toggle();
void tsclicker_reload();

#endif