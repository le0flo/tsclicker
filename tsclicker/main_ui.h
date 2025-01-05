#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <iostream>
#include <string>

#include <QtCore/Qt>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

#include "clicker_ui.h"
#include "recorder_ui.h"
#include "injector_ui.h"

class MainUi : public QWidget {
private:
    QTabWidget* tab_widget = nullptr;
    ClickerUi* clicker_tab = nullptr;
    RecorderUi* recorder_tab = nullptr;
    InjectorUi* injector_tab = nullptr;

    void setup();

public:
    MainUi(Clicker* clicker, Recorder* recorder, QWidget* parent = 0);
    ~MainUi();

    ClickerUi* get_clicker();
    RecorderUi* get_recorder();
};

#endif