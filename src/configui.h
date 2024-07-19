#ifndef CONFIGUI_H
#define CONFIGUI_H

#include <Qt>
#include <QWidget>
#include <QSettings>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>

#include "clicker.h"

class ConfigUi : public QWidget {

private:
    Clicker* clicker;

    QSettings* settings;
    QVBoxLayout* layout;

    QLabel* label_cps;
    QSpinBox* selector_cps;

    QLabel* label_use_recorded_clicks;
    QCheckBox* toggle_use_recorded_clicks;

    QLabel* label_click_left;
    QCheckBox* toggle_click_left; 
    QLabel* label_click_right;
    QCheckBox* toggle_click_right;

    QPushButton* save;

public:
    ConfigUi(Clicker* clicker, QWidget* parent = 0);
    ~ConfigUi();

    void save_settings();
    int current_cps();
    bool use_recorded_clicks();
    bool left_click();
    bool right_click();
};

#endif