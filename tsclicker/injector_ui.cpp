#include "injector_ui.h"

InjectorUi::InjectorUi(QWidget* parent) : QWidget(parent) {
    toggle = new QCheckBox(this);
    module_path = new QLineEdit(this);
    module_open = new QPushButton(this);

    this->setup();
}

InjectorUi::~InjectorUi() {
    delete toggle;
    delete module_path;
    delete module_open;
}

void InjectorUi::setup() {

}

void InjectorUi::on_change() {

}

void InjectorUi::open_module() {

}

void InjectorUi::inject_module() {

}