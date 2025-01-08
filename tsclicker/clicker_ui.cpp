#include "clicker_ui.h"

ClickerUi::ClickerUi(Clicker* clicker, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;

    main_layout = new QHBoxLayout(this);
    left_col = new QWidget(this);
    left_layout = new QVBoxLayout(left_col);
    clicks_row = new QWidget(left_col);
    clicks_layout = new QHBoxLayout(clicks_row);
    right_col = new QWidget(this);
    right_layout = new QVBoxLayout(right_col);

    toggle = new QCheckBox(left_col);
    click_left = new QCheckBox(clicks_row);
    click_right = new QCheckBox(clicks_row);
    cps_slider = new QSlider(right_col);
    cps_label = new QLabel(right_col);
    select_cps = new QRadioButton(right_col);
    select_recording = new QRadioButton(right_col);

    this->setup();
}

ClickerUi::~ClickerUi() {
    delete main_layout;
    delete left_col;
    delete left_layout;
    delete clicks_row;
    delete clicks_layout;
    delete right_col;
    delete right_layout;
    delete toggle;
    delete click_left;
    delete click_right;
    delete cps_slider;
    delete cps_label;
    delete select_cps;
    delete select_recording;
}

void ClickerUi::setup() {
    toggle->setText("Enabled");
    toggle->setCheckState(Qt::CheckState::Unchecked);
    connect(toggle, &QCheckBox::clicked, this, &ClickerUi::on_change);
    left_layout->addWidget(toggle);

    click_left->setText("Left");
    click_left->setCheckState(Qt::CheckState::Checked);
    connect(click_left, &QCheckBox::clicked, this, &ClickerUi::on_change);
    clicks_layout->addWidget(click_left);

    click_right->setText("Right");
    click_right->setCheckState(Qt::CheckState::Unchecked);
    connect(click_right, &QCheckBox::clicked, this, &ClickerUi::on_change);
    clicks_layout->addWidget(click_right);

    clicks_row->setLayout(clicks_layout);
    left_layout->addWidget(clicks_row);
    left_col->setLayout(left_layout);

    cps_slider->setMinimum(6);
    cps_slider->setMaximum(60);
    cps_slider->setValue(16);
    cps_slider->setOrientation(Qt::Horizontal);
    connect(cps_slider, &QSlider::valueChanged, this, &ClickerUi::on_change);
    right_layout->addWidget(cps_slider);

    std::string text = "CPS: " + std::to_string(cps_slider->value());
    cps_label->setText(text.c_str());
    right_layout->addWidget(cps_label);

    select_cps->setText("Static CPS");
    select_cps->setChecked(true);
    connect(select_cps, &QRadioButton::clicked, this, &ClickerUi::on_change);
    right_layout->addWidget(select_cps);

    select_recording->setText("Recorded clicks");
    connect(select_recording, &QRadioButton::clicked, this, &ClickerUi::on_change);
    right_layout->addWidget(select_recording);
    
    right_col->setLayout(right_layout);
    
    main_layout->addWidget(left_col);
    main_layout->addWidget(right_col);
    setLayout(main_layout);
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