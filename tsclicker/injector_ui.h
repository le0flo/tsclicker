#ifndef INJECTOR_UI_H
#define INJECTOR_UI_H

#include <iostream>
#include <string>

#include <QtCore/Qt>
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>

#include "plugin.h"
#include "injector.h"

class InjectorUi : public QWidget {
private:
    Injector* injector = nullptr;

    QCheckBox* toggle = nullptr;
    QLineEdit* module_path = nullptr;
    QPushButton* module_open = nullptr;

    void setup();
    void on_change();

    void open_module();
    void inject_module();

public:
    InjectorUi(Injector* injector, QWidget* parent = 0);
    ~InjectorUi();
};

#endif
