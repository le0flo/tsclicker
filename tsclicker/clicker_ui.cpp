#include "clicker_ui.h"

ClickerUi::ClickerUi(Clicker* clicker, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;

    toggle = new QCheckBox(this);
    click_left = new QCheckBox(this);
    click_right = new QCheckBox(this);
    cps_slider = new QSlider(this);
    cps_label = new QLabel(this);
    select_cps = new QRadioButton(this);
    select_recording = new QRadioButton(this);

    this->setup();
}

ClickerUi::~ClickerUi() {
    delete toggle;
    delete click_left;
    delete click_right;
    delete cps_slider;
    delete cps_label;
    delete select_cps;
    delete select_recording;
}

void ClickerUi::setup() {
    toggle->setText("Enable");
    toggle->setCheckState(Qt::CheckState::Unchecked);
    toggle->setGeometry(QRect(20, 20, 200, 20));
    connect(toggle, &QCheckBox::clicked, this, &ClickerUi::on_change);

    click_left->setText("Left");
    click_left->setCheckState(Qt::CheckState::Checked);
    click_left->setGeometry(QRect(20, 80, 100, 20));
    connect(click_left, &QCheckBox::clicked, this, &ClickerUi::on_change);

    click_right->setText("Right");
    click_right->setCheckState(Qt::CheckState::Unchecked);
    click_right->setGeometry(QRect(120, 80, 100, 20));
    connect(click_right, &QCheckBox::clicked, this, &ClickerUi::on_change);

    cps_slider->setMinimum(6);
    cps_slider->setMaximum(60);
    cps_slider->setValue(16);
    cps_slider->setOrientation(Qt::Horizontal);
    cps_slider->setGeometry(QRect(220, 40, 200, 20));
    connect(cps_slider, &QSlider::valueChanged, this, &ClickerUi::on_change);

    std::string text = "CPS: " + std::to_string(cps_slider->value());
    cps_label->setText(text.c_str());
    cps_label->setGeometry(QRect(220, 20, 200, 20));

    select_cps->setText("Static CPS");
    select_cps->setChecked(true);
    select_cps->setGeometry(QRect(220, 60, 200, 20));
    connect(select_cps, &QRadioButton::clicked, this, &ClickerUi::on_change);

    select_recording->setText("Recorded clicks");
    select_recording->setGeometry(QRect(220, 80, 200, 20));
    connect(select_recording, &QRadioButton::clicked, this, &ClickerUi::on_change);
}

void ClickerUi::on_change() {
    std::tuple<std::vector<int>, std::vector<int>> intervals = read_intervals(plugin::intervals_path);

    if (std::get<0>(intervals).empty()) {
        select_cps->setChecked(true);
        select_recording->setChecked(false);
        select_recording->setCheckable(false);
    }
    else {
        select_recording->setCheckable(true);
    }

    clicker->enable_clicker(toggle->isChecked());
    clicker->enable_click_left(click_left->isChecked());
    clicker->enable_click_right(click_right->isChecked());
    clicker->set_interval(cps_slider->value());
    clicker->enable_intervals(select_recording->isChecked());

    std::string text = "CPS: " + std::to_string(cps_slider->value());
    cps_label->setText(text.c_str());

    if (select_recording->isChecked()) {
        clicker->set_intervals(std::get<0>(intervals), std::get<1>(intervals));
    }
}

std::tuple<std::vector<int>, std::vector<int>> ClickerUi::read_intervals(std::string filename) {
    std::tuple<std::vector<int>, std::vector<int>> result;
    std::fstream input_file;

    if (filename.empty()) return result;

    input_file.open(filename, std::ios::in);

    if (input_file.fail()) return result;

    std::string line;
    while (std::getline(input_file, line)) {
        size_t split_sym = line.find_first_of(':');
        if (split_sym != std::string::npos) {
            std::get<0>(result).push_back(atoi(line.substr(0, split_sym).c_str()));
            std::get<1>(result).push_back(atoi(line.substr(split_sym + 1).c_str()));
        }
    }

    input_file.close();

    return result;
}

void ClickerUi::toggle_clicker() {
    if (toggle->isChecked()) {
        toggle->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        toggle->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}

void ClickerUi::toggle_click_left() {
    if (click_left->isChecked()) {
        click_left->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        click_left->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}

void ClickerUi::toggle_click_right() {
    if (click_right->isChecked()) {
        click_right->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        click_right->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}