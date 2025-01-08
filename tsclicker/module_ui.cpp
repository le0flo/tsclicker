#include "module_ui.h"

ModuleUi::ModuleUi(std::string filename, QWidget* parent) : QWidget(parent) {
	this->filename = filename;

	layout = new QHBoxLayout(this);
	label = new QLabel(this);
	inject = new QPushButton(this);
	eject = new QPushButton(this);

	this->setup();
}

ModuleUi::~ModuleUi() {
	delete layout;
	delete label;
	delete inject;
	delete eject;
}

void ModuleUi::setup() {
	setGeometry(0, 0, 400, 30);

	std::string text = filename.substr(filename.find_last_of('/') + 1);
	label->setText(text.c_str());
	layout->addWidget(label);

	inject->setText("Inject");
	connect(inject, &QPushButton::clicked, this, &ModuleUi::inject_module);
	layout->addWidget(inject);

	eject->setText("Eject");
	eject->setDisabled(true); // TODO sistema di eject
	connect(eject, &QPushButton::clicked, this, &ModuleUi::eject_module);
	layout->addWidget(eject);

	setLayout(layout);
}

void ModuleUi::remove_module() {
	eject_module();

	deleteLater();
}

bool ModuleUi::inject_module() {
	DWORD pid = get_process_id();
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (process == INVALID_HANDLE_VALUE) return false;

	LPVOID location = VirtualAllocEx(process, 0, filename.size() + 1, MEM_COMMIT, PAGE_READWRITE);
	if (location == nullptr) return false;

	WriteProcessMemory(process, location, filename.c_str(), filename.size() + 1, 0);
	HANDLE thread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, location, 0, 0);

	if (thread && thread != INVALID_HANDLE_VALUE) {
		CloseHandle(thread);
	}

	if (process && process != INVALID_HANDLE_VALUE) {
		CloseHandle(process);
	}

	return true;
}

bool ModuleUi::eject_module() {
	// TODO sistema di eject

	return true;
}

DWORD ModuleUi::get_process_id() {
	HWND window_lwjgl = FindWindowA("LWJGL", nullptr);
	HWND window_glfw30 = FindWindowA("GLFW30", nullptr);
	DWORD process_id = 0;

	GetWindowThreadProcessId(window_lwjgl, &process_id);
	if (process_id == 0) GetWindowThreadProcessId(window_glfw30, &process_id);

	return process_id;
}