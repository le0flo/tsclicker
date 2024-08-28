#include "configui.h"

ConfigUi::ConfigUi(Clicker* clicker, Recorder* recorder, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;
    this->recorder = recorder;

    settings = new QSettings("Bestemmie", "TS clicker");
    tabWidget = new QTabWidget(this);
    clicker_tab = new QWidget(tabWidget);
    recorder_tab = new QWidget(tabWidget);

    // Clicker widgets

    clicker_toggle = new QCheckBox(clicker_tab);
    click_left = new QCheckBox(clicker_tab);
    click_right = new QCheckBox(clicker_tab);
    cps_slider = new QSlider(clicker_tab);
    cps_label = new QLabel(clicker_tab);
    save = new QPushButton(clicker_tab);

    // Recorder widgets

    record = new QPushButton(recorder_tab);

    setup_window();
}

ConfigUi::~ConfigUi() {
    delete settings;
    delete tabWidget;
    delete clicker_tab;
    delete recorder_tab;

    // Clicker widgets

    delete clicker_toggle;
    delete click_left;
    delete click_right;
    delete cps_slider;
    delete cps_label;
    delete save;

    // Recorder widgets

    delete record;
}

void ConfigUi::setup_window() {
    setWindowTitle("TS clicker settings");
    resize(1000, 550);

    tabWidget->setGeometry(QRect(0, 0, 1000, 550));
    tabWidget->addTab(clicker_tab, "Clicker");
    tabWidget->addTab(recorder_tab, "Recorder");

    setup_clicker_tab();
    setup_recorder_tab();
}

void ConfigUi::setup_clicker_tab() {
    clicker_toggle->setText("Enabled");
    clicker_toggle->setCheckState(Qt::CheckState::Unchecked);
    clicker_toggle->setGeometry(QRect(450, 350, 100, 20));

    click_left->setText("Left");
    click_left->setCheckState(Qt::CheckState::Unchecked);
    click_left->setGeometry(QRect(300, 100, 100, 20));

    click_right->setText("Right");
    click_right->setCheckState(Qt::CheckState::Unchecked);
    click_right->setGeometry(QRect(700, 100, 100, 20));

    cps_slider->setMinimum(6);
    cps_slider->setMaximum(60);
    cps_slider->setValue(16);
    cps_slider->setOrientation(Qt::Horizontal);
    cps_slider->setGeometry(QRect(450, 250, 100, 20));

    cps_label->setText("CPS");
    cps_label->setGeometry(QRect(450, 230, 100, 20));

    save->setText("Save");
    save->setGeometry(QRect(0, 0, 100, 20));
    connect(save, &QPushButton::released, this, &ConfigUi::save_settings);
}

void ConfigUi::setup_recorder_tab() {
    record->setText("Start recording");
    record->setGeometry(QRect(0, 0, 100, 20));
    connect(save, &QPushButton::released, this, &ConfigUi::start_recording);
}

// Button callbacks

void ConfigUi::save_settings() {
    settings->setValue("clicker_toggle", clicker_toggle->isChecked());
    settings->setValue("click_left", click_left->isChecked());
    settings->setValue("click_right", click_right->isChecked());
    settings->setValue("cps", cps_slider->value());
    settings->sync();
    clicker->sync(settings);
}

void ConfigUi::start_recording() {
    clicker->forcestop();
    bool status = recorder->toggle_recorder();

    if (status) {
        record->setText("Stop recording");
    } else {
        record->setText("Start recording");
    }
}

// Setters

void ConfigUi::toggle_clicker() {
    if (clicker_toggle->isChecked()) {
        clicker_toggle->setCheckState(Qt::CheckState::Unchecked);
    } else {
        clicker_toggle->setCheckState(Qt::CheckState::Checked);
    }

    save_settings();
}

void ConfigUi::toggle_click_left() {
    if (click_left->isChecked()) {
        click_left->setCheckState(Qt::CheckState::Unchecked);
    } else {
        click_left->setCheckState(Qt::CheckState::Checked);
    }

    save_settings();
}

void ConfigUi::toggle_click_right() {
    if (click_right->isChecked()) {
        click_right->setCheckState(Qt::CheckState::Unchecked);
    } else {
        click_right->setCheckState(Qt::CheckState::Checked);
    }

    save_settings();
}