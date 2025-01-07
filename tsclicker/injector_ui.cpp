#include "injector_ui.h"

InjectorUi::InjectorUi(Injector* injector, QWidget* parent) : QWidget(parent) {
    this->injector = injector;
    
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
    toggle->setText("Inject");
    toggle->setCheckState(Qt::CheckState::Unchecked);
    toggle->setGeometry(QRect(20, 20, 200, 20));
    connect(toggle, &QCheckBox::clicked, this, &InjectorUi::on_change);

    module_open->setText("Open\nmodule");
    module_open->setGeometry(QRect(220, 20, 100, 50));
    connect(module_open, &QPushButton::clicked, this, &InjectorUi::open_module);

    module_path->setPlaceholderText("Module path");
    module_path->setGeometry(QRect(20, 80, 300, 20));
    connect(module_path, &QLineEdit::textChanged, this, &InjectorUi::on_change);
}

void InjectorUi::on_change() {
    injector->enable_addons(toggle->isChecked());
}

void InjectorUi::open_module() {
    QString filename = QFileDialog::getOpenFileName(this, "Select the addon", plugin::data_folder().c_str(), PLUGIN_FILTER_ADDON);

    if (filename.isEmpty()) return;

    if (!filename.endsWith(PLUGIN_SUFFIX_ADDON)) {
        filename.append(PLUGIN_SUFFIX_ADDON);
    }

    injector->add_addon(filename.toStdString());
}
