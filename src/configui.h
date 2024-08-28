#ifndef CONFIGUI_H
#define CONFIGUI_H

#include <QtCore\Qt>
#include <QtCore\QSettings>
#include <QtWidgets\QTabWidget>
#include <QtWidgets\QWidget>
#include <QtWidgets\QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets\QSlider>
#include <QtWidgets\QLabel>
#include <QtWidgets\QPushButton>

#include "plugin.h"
#include "clicker.h"
#include "recorder.h"

class ConfigUi : public QWidget {
private:
    Clicker* clicker;
    Recorder* recorder;

    QSettings* settings;
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
    QPushButton* clicker_save;

    // Recorder widgets

    QPushButton* record;
    QPushButton* recorder_save;

    void setup_window();
    void setup_clicker_tab();
    void setup_recorder_tab();

    void save_settings();
    void start_recording();
    void save_intervals();

public:
    ConfigUi(Clicker* clicker, Recorder* recorder, QWidget* parent = 0);
    ~ConfigUi();

    void toggle_clicker();
    void toggle_click_left();
    void toggle_click_right();
};

#endif