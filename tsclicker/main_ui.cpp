#include "main_ui.h"

MainUi::MainUi(Clicker* clicker, Recorder* recorder, QWidget* parent) : QWidget(parent) {
    tab_widget = new QTabWidget(this);
    clicker_tab = new ClickerUi(clicker, tab_widget);
    recorder_tab = new RecorderUi(recorder, tab_widget);
    injector_tab = new InjectorUi(tab_widget);

    this->setup();
}

MainUi::~MainUi() {
    delete tab_widget;
    delete clicker_tab;
    delete recorder_tab;
    delete injector_tab;
}

void MainUi::setup() {
    setWindowTitle("TS clicker settings");
    setFixedSize(500, 300);

    tab_widget->setGeometry(this->geometry());
    tab_widget->addTab(clicker_tab, "Clicker");
    tab_widget->addTab(recorder_tab, "Recorder");
    tab_widget->addTab(injector_tab, "Addons");
}

ClickerUi* MainUi::get_clicker() {
    return this->clicker_tab;
}

RecorderUi* MainUi::get_recorder() {
    return this->recorder_tab;
}
