#include "configui.h"

ConfigUi::ConfigUi(Clicker* clicker, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;

    this->settings = new QSettings("Bestemmie", "TS clicker");
    this->layout = new QVBoxLayout();

    this->label_cps = new QLabel();
    this->selector_cps = new QSpinBox();

    this->label_use_recorded_clicks = new QLabel();
    this->toggle_use_recorded_clicks = new QCheckBox();

    this->label_click_left = new QLabel();
    this->toggle_click_left = new QCheckBox(); 
    this->label_click_right = new QLabel();
    this->toggle_click_right = new QCheckBox();

    this->save = new QPushButton();
    connect(this->save, &QPushButton::released, this, &ConfigUi::save_settings);

    this->setWindowTitle("TS clicker settings");
    this->resize(300, 200);
    this->setLayout(this->layout);

    this->label_cps->setText("CPS:");
    this->layout->addWidget(this->label_cps);

    this->selector_cps->setMinimum(6);
    this->selector_cps->setMaximum(100);
    this->selector_cps->setValue(16);
    this->layout->addWidget(this->selector_cps);

    this->label_use_recorded_clicks->setText("Usare i click registrati:");
    this->layout->addWidget(this->label_use_recorded_clicks);

    this->toggle_use_recorded_clicks->setTristate(false);
    this->toggle_use_recorded_clicks->setCheckState(Qt::CheckState::Unchecked);
    this->layout->addWidget(this->toggle_use_recorded_clicks);

    this->label_click_left->setText("Tasto sinistro:");
    this->layout->addWidget(this->label_click_left);

    this->toggle_click_left->setTristate(false);
    this->toggle_click_left->setCheckState(Qt::CheckState::Checked);
    this->layout->addWidget(this->toggle_click_left);

    this->label_click_right->setText("Tasto destro:");
    this->layout->addWidget(this->label_click_right);

    this->toggle_click_right->setTristate(false);
    this->toggle_click_right->setCheckState(Qt::CheckState::Unchecked);
    this->layout->addWidget(this->toggle_click_right);

    this->save->setText("Salva");
    this->layout->addWidget(this->save);
}

ConfigUi::~ConfigUi() {
    delete this->settings;
    delete this->layout;

    delete this->label_cps;
    delete this->selector_cps;

    delete this->label_use_recorded_clicks;
    delete this->toggle_use_recorded_clicks;

    delete this->label_click_left;
    delete this->toggle_click_left;
    delete this->label_click_right;
    delete this->toggle_click_right;

    delete this->save;
}

void ConfigUi::save_settings() {
    this->settings->setValue("cps", this->selector_cps->value());
    this->settings->setValue("use_recorded_clicks", this->toggle_use_recorded_clicks->isChecked());
    this->settings->setValue("left_click", this->toggle_click_left->isChecked());
    this->settings->setValue("right_click", this->toggle_click_right->isChecked());
    this->settings->sync();

    this->clicker->set_cps(this->current_cps());
    this->clicker->use_recorded_clicks(this->use_recorded_clicks());
    this->clicker->set_left_click(this->left_click());
    this->clicker->set_right_click(this->right_click());
}

int ConfigUi::current_cps() {
    return this->settings->value("cps").toInt();
}

bool ConfigUi::use_recorded_clicks() {
    return this->settings->value("use_recorded_clicks").toBool();
}

bool ConfigUi::left_click() {
    return this->settings->value("left_click").toBool();
}

bool ConfigUi::right_click() {
    return this->settings->value("right_click").toBool();
}