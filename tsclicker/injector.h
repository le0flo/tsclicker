#ifndef INJECTOR_H
#define INJECTOR_H

#include <iostream>
#include <vector>
#include <Windows.h>

typedef std::tuple<std::string, bool> addon;

class Injector {
private:
	DWORD get_process_id();

	void enable_addon(addon addon);
	bool inject(addon addon);
	bool deject(addon addon);

public:
	std::vector<addon> addons;

	Injector();

	void enable_addons(bool toggle);
	void enable_addon(std::string addon_path, bool toggle);
	
	bool add_addon(std::string addon_path);
	bool del_addon(std::string addon_path);
};

#endif