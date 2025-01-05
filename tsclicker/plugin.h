/*
 * TeamSpeak 3 demo plugin | TS clicker (alpha)
 *
 * Copyright (c) TeamSpeak Systems GmbH
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <iostream>

#define PLUGINS_EXPORTDLL __declspec(dllexport)

#define PLUGIN_API_VERSION 26
#define PLUGIN_NAME "TS clicker"
#define PLUGIN_VERSION "2.0"
#define PLUGIN_AUTHOR "Bestemmie"
#define PLUGIN_DESCRIPTION "Skibidi sigma clicker"
#define PLUGIN_FILEFORMAT ".tsc"
#define PLUGIN_FILEFORMAT_FILTER "TS clicker intervals (*.tsc)"

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
PLUGINS_EXPORTDLL void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys);
PLUGINS_EXPORTDLL void ts3plugin_onHotkeyEvent(const char* keyword);

}

namespace plugin {
	inline std::string intervals_path;

	std::string data_folder();
}

#endif