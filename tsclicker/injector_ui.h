#ifndef INJECTOR_UI_H
#define INJECTOR_UI_H

#include <iostream>
#include <string>

#include <QtCore/Qt>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlistwidget.h>

#include "plugin.h"
#include "module_ui.h"

class InjectorUi : public QWidget {
private:
    QLabel* label = nullptr;
    QPushButton* open = nullptr;
    QPushButton* remove = nullptr;
    QLineEdit* path = nullptr;
    QListWidget* list = nullptr;

    void setup();
    void open_module();
    void append_module(std::string filename);
    void remove_module();

public:
    InjectorUi(QWidget* parent = 0);
    ~InjectorUi();
};

#endif
