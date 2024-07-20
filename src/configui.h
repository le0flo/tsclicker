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
#include <QSound>

#include "plugin.h"
#include "clicker.h"

class ConfigUi : public QWidget {

private:
    Clicker* clicker;

    QSettings* settings;
    QVBoxLayout* layout;

    QLabel* label_clicker;
    QCheckBox* toggle_clicker;

    QLabel* label_cps;
    QSpinBox* selector_cps;

    QLabel* label_click_left;
    QCheckBox* toggle_click_left;
    QLabel* label_click_right;
    QCheckBox* toggle_click_right;

    QLabel* label_recorded;
    QCheckBox* toggle_recorded;
    QPushButton* reload_recorded;

    QPushButton* save;

    void setup_window();

public:
    ConfigUi(Clicker* clicker, QWidget* parent = 0);
    ~ConfigUi();

    void save_settings();
    
    bool get_clicker();
    int get_cps();
    bool get_click_left();
    bool get_click_right();
    bool get_recorded();

    void set_clicker(bool value);
    void set_click_left(bool value);
    void set_click_right(bool value);
    void set_recorded(bool value);
    
    void save_settings_button();
    void reload_recorded_button();
};

#endif