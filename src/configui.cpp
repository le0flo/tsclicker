#include "configui.h"

ConfigUi::ConfigUi(Clicker* clicker, Recorder* recorder, QWidget* parent) : QWidget(parent) {
    this->clicker = clicker;
    this->recorder = recorder;

    tab_widget = new QTabWidget(this);
    clicker_tab = new QWidget(tab_widget);
    recorder_tab = new QWidget(tab_widget);

    // Clicker widgets

    clicker_toggle = new QCheckBox(clicker_tab);
    click_left = new QCheckBox(clicker_tab);
    click_right = new QCheckBox(clicker_tab);
    cps_slider = new QSlider(clicker_tab);
    cps_label = new QLabel(clicker_tab);
    select_cps = new QRadioButton(clicker_tab);
    select_recording = new QRadioButton(clicker_tab);

    // Recorder widgets

    record = new QPushButton(recorder_tab);
    recorder_save = new QPushButton(recorder_tab);
    intervals_path = new QLineEdit(recorder_tab);
    intervals_open = new QPushButton(recorder_tab);

    setup_window();
}

ConfigUi::~ConfigUi() {
    delete tab_widget;
    delete clicker_tab;
    delete recorder_tab;

    // Clicker widgets

    delete clicker_toggle;
    delete click_left;
    delete click_right;
    delete cps_slider;
    delete cps_label;
    delete select_cps;
    delete select_recording;

    // Recorder widgets

    delete record;
    delete recorder_save;
    delete intervals_path;
    delete intervals_open;
}

// Setups

void ConfigUi::setup_window() {
    setWindowTitle("TS clicker settings");
    resize(1000, 550);

    tab_widget->setGeometry(QRect(0, 0, 1000, 550));
    tab_widget->addTab(clicker_tab, "Clicker");
    tab_widget->addTab(recorder_tab, "Recorder");

    setup_clicker_tab();
    setup_recorder_tab();
}

void ConfigUi::setup_clicker_tab() {
    clicker_toggle->setText("Enabled");
    clicker_toggle->setCheckState(Qt::CheckState::Unchecked);
    clicker_toggle->setGeometry(QRect(450, 350, 100, 20));
    connect(clicker_toggle, &QCheckBox::clicked, this, &ConfigUi::on_change);

    click_left->setText("Left");
    click_left->setCheckState(Qt::CheckState::Checked);
    click_left->setGeometry(QRect(300, 100, 100, 20));
    connect(click_left, &QCheckBox::clicked, this, &ConfigUi::on_change);

    click_right->setText("Right");
    click_right->setCheckState(Qt::CheckState::Unchecked);
    click_right->setGeometry(QRect(700, 100, 100, 20));
    connect(click_right, &QCheckBox::clicked, this, &ConfigUi::on_change);

    cps_slider->setMinimum(6);
    cps_slider->setMaximum(60);
    cps_slider->setOrientation(Qt::Horizontal);
    cps_slider->setGeometry(QRect(450, 250, 100, 20));
    connect(cps_slider, &QSlider::valueChanged, this, &ConfigUi::on_changeslider);
    cps_slider->setValue(16);
    cps_label->setGeometry(QRect(450, 230, 100, 20));

    select_cps->setText("Static CPS");
    select_cps->setChecked(true);
    select_cps->setGeometry(QRect(60, 220, 200, 20));
    connect(select_cps, &QRadioButton::clicked, this, &ConfigUi::on_change);
    
    select_recording->setText("Recorded clicks");
    select_recording->setGeometry(QRect(60, 240, 200, 20));
    connect(select_recording, &QRadioButton::clicked, this, &ConfigUi::on_change);
}

void ConfigUi::setup_recorder_tab() {
    record->setText("Start recording");
    record->setGeometry(QRect(0, 0, 200, 40));
    connect(record, &QPushButton::clicked, this, &ConfigUi::start_recording);

    recorder_save->setText("Save the recording");
    recorder_save->setGeometry(QRect(0, 50, 200, 40));
    connect(recorder_save, &QPushButton::clicked, this, &ConfigUi::save_intervals);

    intervals_path->setPlaceholderText("Recording file path");
    intervals_path->setGeometry(QRect(0, 100, 300, 20));
    connect(intervals_path, &QLineEdit::textChanged, this, &ConfigUi::on_change);
    
    intervals_open->setText("Open");
    intervals_open->setGeometry(QRect(350, 100, 100, 20));
    connect(intervals_open, &QPushButton::clicked, this, &ConfigUi::open_intervals);
}

// Events

void ConfigUi::on_change() {
    std::tuple<std::vector<int>,std::vector<int>> intervals = read_intervals(intervals_path->text().toStdString());

    if (std::get<0>(intervals).empty()) {
        select_cps->setChecked(true);        
        select_recording->setChecked(false);
        select_recording->setCheckable(false);
    } else {
        select_recording->setCheckable(true);
    }

    clicker->enable_clicker(clicker_toggle->isChecked());
    clicker->enable_intervals(select_recording->isChecked());
    clicker->enable_click_left(click_left->isChecked());
    clicker->enable_click_right(click_right->isChecked());
    clicker->set_interval(cps_slider->value());
    
    if (select_recording->isChecked()) {
        clicker->set_intervals(std::get<0>(intervals), std::get<1>(intervals));
    }
}

void ConfigUi::on_changeslider() {
    std::string text = "CPS: " + std::to_string(cps_slider->value());
    cps_label->setText(text.c_str());

    on_change();
}

// Button callbacks

void ConfigUi::start_recording() {
    clicker->forcestop();
    clicker_toggle->setCheckState(Qt::CheckState::Unchecked);

    bool status = recorder->toggle_recorder();

    if (status) {
        record->setText("Stop recording");
    } else {
        record->setText("Start recording");
    }
}

void ConfigUi::save_intervals() {
    std::vector<long long> intervals = recorder->get_intervals();

    if (intervals.size() <= 0) {
        MessageBoxA(NULL, "No intervals found in memory, try again.", "Error", MB_ICONERROR);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save the recording", tsclicker_plugin_data_folder().c_str(), PLUGIN_FILEFORMAT_FILTER);

    if (filename.isEmpty()) {
        MessageBoxA(NULL, "Invalid file name.", "Error", MB_ICONERROR);
        return;
    }

    if (!filename.endsWith(PLUGIN_FILEFORMAT)) {
        filename.append(PLUGIN_FILEFORMAT);
    }

    write_intervals(filename.toStdString(), intervals);
    intervals_path->setText(filename);
}

void ConfigUi::open_intervals() {
    QString filename = QFileDialog::getOpenFileName(this, "Select the recording", tsclicker_plugin_data_folder().c_str(), PLUGIN_FILEFORMAT_FILTER);

    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(PLUGIN_FILEFORMAT)) {
        filename.append(PLUGIN_FILEFORMAT);
    }

    intervals_path->setText(filename);
}

// File operations

std::tuple<std::vector<int>,std::vector<int>> ConfigUi::read_intervals(std::string filename) {
    std::fstream input_file;
    input_file.open(filename, std::ios::in);
    
    std::tuple<std::vector<int>,std::vector<int>> result;

    if (input_file.fail()) {
        return result;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        size_t split_sym = line.find_first_of(':');
        if (split_sym != std::string::npos) {
            std::get<0>(result).push_back(atoi(line.substr(0, split_sym).c_str()));
            std::get<1>(result).push_back(atoi(line.substr(split_sym+1).c_str()));
        }
    }

    input_file.close();

    return result;
}

void ConfigUi::write_intervals(std::string filename, std::vector<long long> intervals) {
    std::fstream output_file;
    output_file.open(filename, std::ios::out);

    for (int i=1; i<intervals.size()-1; i+=2) {
        output_file << intervals.at(i) - intervals.at(i - 1) << ":";
        output_file << intervals.at(i + 1) - intervals.at(i) << "\n";
    }

    output_file.close();
}

// Setters

void ConfigUi::toggle_clicker() {
    if (clicker_toggle->isChecked()) {
        clicker_toggle->setCheckState(Qt::CheckState::Unchecked);
    } else {
        clicker_toggle->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}

void ConfigUi::toggle_click_left() {
    if (click_left->isChecked()) {
        click_left->setCheckState(Qt::CheckState::Unchecked);
    } else {
        click_left->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}

void ConfigUi::toggle_click_right() {
    if (click_right->isChecked()) {
        click_right->setCheckState(Qt::CheckState::Unchecked);
    } else {
        click_right->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}