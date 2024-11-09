#ifndef CONFIGUI_H
#define CONFIGUI_H

#include <iostream>
#include <string>
#include <fstream>

#include <QtCore/Qt>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>

#include "plugin.h"
#include "clicker.h"
#include "recorder.h"

class ConfigUi : public QWidget {
private:
    Clicker* clicker;
    Recorder* recorder;

    QTabWidget* tab_widget;
    QWidget* clicker_tab;
    QWidget* recorder_tab;

    // Clicker widgets

    QCheckBox* clicker_toggle;
    QCheckBox* click_left;
    QCheckBox* click_right;
    QSlider* cps_slider;
    QLabel* cps_label;
    QRadioButton* select_cps;
    QRadioButton* select_recording;

    // Recorder widgets

    QPushButton* record;
    QPushButton* recorder_save;
    QLineEdit* intervals_path;
    QPushButton* intervals_open;

    // Setups

    void setup_window();
    void setup_clicker_tab();
    void setup_recorder_tab();

    // Events

    void on_change();
    void on_changeslider();

    // Button callbacks

    void start_recording();
    void save_intervals();
    void open_intervals();

    // File operations

    std::tuple<std::vector<int>,std::vector<int>> read_intervals(std::string filename);
    void write_intervals(std::string filename, std::vector<long long> intervals);

public:
    ConfigUi(Clicker* clicker, Recorder* recorder, QWidget* parent = 0);
    ~ConfigUi();

    // Setters

    void toggle_clicker();
    void toggle_click_left();
    void toggle_click_right();
};

#endif