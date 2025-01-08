#ifndef RECORDER_UI_H
#define RECORDER_UI_H

#include <iostream>
#include <string>
#include <fstream>

#include <QtCore/Qt>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qfiledialog.h>

#include "plugin.h"
#include "recorder.h"

class RecorderUi : public QWidget {
private:
    Recorder* recorder = nullptr;

    QCheckBox* toggle = nullptr;
    QPushButton* save = nullptr;
    QPushButton* open = nullptr;
    QLineEdit* path = nullptr;

    void setup();
    void on_change();

    void open_intervals();
    void save_intervals();
    void write_intervals(std::string filename, std::vector<long long> intervals);
    void update_intervals_path(QString filename);

public:
    RecorderUi(Recorder* recorder, QWidget* parent = 0);
    ~RecorderUi();

    void toggle_recorder();
};

#endif