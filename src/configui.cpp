#include "configui.h"

ConfigUi::ConfigUi(Clicker* clicker, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;

    settings = new QSettings("Bestemmie", "TS clicker");
    layout = new QVBoxLayout();

    label_clicker = new QLabel();
    toggle_clicker = new QCheckBox();

    label_cps = new QLabel();
    selector_cps = new QSpinBox();

    label_click_left = new QLabel();
    toggle_click_left = new QCheckBox(); 
    label_click_right = new QLabel();
    toggle_click_right = new QCheckBox();

    label_recorded = new QLabel();
    toggle_recorded = new QCheckBox();
    reload_recorded = new QPushButton();
    connect(reload_recorded, &QPushButton::released, this, &ConfigUi::reload_recorded_button);

    save = new QPushButton();
    connect(save, &QPushButton::released, this, &ConfigUi::save_settings_button);

    setup_window();
}

ConfigUi::~ConfigUi() {
    delete settings;
    delete layout;

    delete label_clicker;
    delete toggle_clicker;

    delete label_cps;
    delete selector_cps;

    delete label_click_left;
    delete toggle_click_left;
    delete label_click_right;
    delete toggle_click_right;

    delete label_recorded;
    delete toggle_recorded;
    delete reload_recorded;

    delete save;
}

void ConfigUi::setup_window() {
    setWindowTitle("TS clicker settings");
    resize(500, 300);
    setLayout(layout);

    label_clicker->setText("Clicker:");
    layout->addWidget(label_clicker);

    toggle_clicker->setTristate(false);
    toggle_clicker->setCheckState(Qt::CheckState::Unchecked);
    layout->addWidget(toggle_clicker);

    label_cps->setText("CPS:");
    layout->addWidget(label_cps);

    selector_cps->setMinimum(6);
    selector_cps->setMaximum(100);
    selector_cps->setValue(16);
    layout->addWidget(selector_cps);

    label_click_left->setText("Tasto sinistro:");
    layout->addWidget(label_click_left);

    toggle_click_left->setTristate(false);
    toggle_click_left->setCheckState(Qt::CheckState::Checked);
    layout->addWidget(toggle_click_left);

    label_click_right->setText("Tasto destro:");
    layout->addWidget(label_click_right);

    toggle_click_right->setTristate(false);
    toggle_click_right->setCheckState(Qt::CheckState::Unchecked);
    layout->addWidget(toggle_click_right);

    label_recorded->setText("Usare i click registrati:");
    layout->addWidget(label_recorded);

    toggle_recorded->setTristate(false);
    toggle_recorded->setCheckState(Qt::CheckState::Unchecked);
    layout->addWidget(toggle_recorded);

    reload_recorded->setText("Carica la registrazione dei click");
    layout->addWidget(reload_recorded);

    save->setText("Salva");
    layout->addWidget(save);
}

void ConfigUi::save_settings() {
    settings->setValue("clicker", toggle_clicker->isChecked());
    settings->setValue("cps", selector_cps->value());
    settings->setValue("recorded", toggle_recorded->isChecked());
    settings->setValue("click_left", toggle_click_left->isChecked());
    settings->setValue("click_right", toggle_click_right->isChecked());
    settings->sync();

    clicker->enable_clicker(get_clicker());
    clicker->set_cps(get_cps());
    clicker->enable_recorded_clicks(get_recorded());
    clicker->enable_click_left(get_click_left());
    clicker->enable_click_right(get_click_right());
}

bool ConfigUi::get_clicker() {
    return settings->value("clicker").toBool();
}

int ConfigUi::get_cps() {
    return settings->value("cps").toInt();
}

bool ConfigUi::get_click_left() {
    return settings->value("click_left").toBool();
}

bool ConfigUi::get_click_right() {
    return settings->value("click_right").toBool();
}

bool ConfigUi::get_recorded() {
    return settings->value("recorded").toBool();
}

void ConfigUi::set_clicker(bool value) {
    if (value) {
        toggle_clicker->setCheckState(Qt::CheckState::Checked);
    } else {
        toggle_clicker->setCheckState(Qt::CheckState::Unchecked);
    }

    save_settings();
}

void ConfigUi::set_click_left(bool value) {
    if (value) {
        toggle_click_left->setCheckState(Qt::CheckState::Checked);
    } else {
        toggle_click_left->setCheckState(Qt::CheckState::Unchecked);
    }

    save_settings();
}

void ConfigUi::set_click_right(bool value) {
    if (value) {
        toggle_click_right->setCheckState(Qt::CheckState::Checked);
    } else {
        toggle_click_right->setCheckState(Qt::CheckState::Unchecked);
    }

    save_settings();
}

void ConfigUi::set_recorded(bool value) {
    if (value) {
        toggle_recorded->setCheckState(Qt::CheckState::Checked);
    } else {
        toggle_recorded->setCheckState(Qt::CheckState::Unchecked);
    }

    save_settings();
}

void ConfigUi::save_settings_button() {
    save_settings();

    MessageBoxA(0, "Impostazioni salvate", "Conferma", MB_OK);
}

void ConfigUi::reload_recorded_button() {
    bool status = clicker->update_recorded_clicks();

    if (status) {
        MessageBoxA(0, "Registrazione dei click caricata", "Conferma", MB_OK);
    } else {
        MessageBoxA(0, "Impossibile leggere la registrazione dei click", "Errore", MB_OK);
    }
}