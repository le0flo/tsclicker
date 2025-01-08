#ifndef CLICKER_UI_H
#define CLICKER_UI_H

#include <iostream>
#include <string>
#include <fstream>

#include <QtCore/Qt>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qradiobutton.h>

#include "plugin.h"
#include "clicker.h"

class ClickerUi : public QWidget {
private:
	Clicker* clicker = nullptr;

    QHBoxLayout* main_layout = nullptr;
    QWidget* left_col = nullptr;
    QVBoxLayout* left_layout = nullptr;
    QWidget* clicks_row = nullptr;
    QHBoxLayout* clicks_layout = nullptr;
    QWidget* right_col = nullptr;
    QVBoxLayout* right_layout = nullptr;

    QCheckBox* toggle = nullptr;
    QCheckBox* click_left = nullptr;
    QCheckBox* click_right = nullptr;
    QSlider* cps_slider = nullptr;
    QLabel* cps_label = nullptr;
    QRadioButton* select_cps = nullptr;
    QRadioButton* select_recording = nullptr;

    void setup();
    void on_change();

    std::tuple<std::vector<int>, std::vector<int>> read_intervals(std::string filename);

public:
	ClickerUi(Clicker* clicker, QWidget* parent = 0);
    ~ClickerUi();

    void toggle_clicker();
    void toggle_click_left();
    void toggle_click_right();
};

#endif
