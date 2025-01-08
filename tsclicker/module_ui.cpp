#include "module_ui.h"

ModuleUi::ModuleUi(std::string filename, QWidget* parent) : QWidget(parent) {
	this->filename = filename;

	label = new QLabel(this);
	toggle = new QCheckBox(this);

	this->setup();
}

ModuleUi::~ModuleUi() {
	delete label;
	delete toggle;
}

void ModuleUi::setup() {
	setGeometry(0, 0, 400, 30);

	std::string text = filename.substr(filename.find_last_of('/') + 1);
	label->setText(text.c_str());
	label->setGeometry(QRect(0, 0, 200, 30));

	toggle->setText("Inject");
	toggle->setCheckState(Qt::CheckState::Unchecked);
	toggle->setGeometry(QRect(200, 0, 100, 30));
	connect(toggle, &QCheckBox::clicked, this, &ModuleUi::enable_module);
}

void ModuleUi::enable_module() {
	if (toggle->isChecked()) inject_module();
	else eject_module();
}

void ModuleUi::remove_module() {
	if (toggle->isChecked()) eject_module();

	deleteLater();
}

bool ModuleUi::inject_module() {
	// TODO

	/*
	DWORD pid = get_process_id();
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (process == INVALID_HANDLE_VALUE) return false;

	LPVOID location = VirtualAllocEx(process, 0, addon_path.size() + 1, MEM_COMMIT, PAGE_READWRITE);
	if (location == nullptr) return false;

	WriteProcessMemory(process, location, addon_path.c_str(), addon_path.size() + 1, 0);
	HANDLE thread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, location, 0, 0);

	if (!thread) {
		CloseHandle(thread);
	}

	if (!process) {
		CloseHandle(process);
	}
	*/

	return true;
}

bool ModuleUi::eject_module() {
	// TODO

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