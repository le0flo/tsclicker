#ifndef MODULE_UI_H
#define MODULE_UI_H

#include <iostream>
#include <Windows.h>

#include <QtCore/Qt>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>

class ModuleUi : public QWidget {
private:
    QHBoxLayout* layout = nullptr;

    QLabel* label = nullptr;
    QPushButton* inject = nullptr;
    QPushButton* eject = nullptr;

    void setup();

    bool inject_module();
    bool eject_module();

    DWORD get_process_id();

public:
    std::string filename;

    ModuleUi(std::string filename, QWidget* parent = 0);
    ~ModuleUi();

    void enable_module();
    void remove_module();
};

#endif
