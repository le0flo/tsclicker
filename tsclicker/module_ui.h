#ifndef MODULE_UI_H
#define MODULE_UI_H

#include <iostream>
#include <Windows.h>

#include <QtCore/Qt>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qpushbutton.h>

#include "injector_ui.h"

class ModuleUi : public QWidget {
private:
    QLabel* label = nullptr;
    QCheckBox* toggle = nullptr;
    QPushButton* remove = nullptr;

    void setup();

    void enable_module();
    void remove_module();
    bool inject_module();
    bool eject_module();

    DWORD get_process_id();

public:
    std::string filename;

    ModuleUi(std::string filename, QWidget* parent = 0);
    ~ModuleUi();
};

#endif
