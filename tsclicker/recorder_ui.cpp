#include "recorder_ui.h"

RecorderUi::RecorderUi(Recorder* recorder, QWidget* parent) : QWidget(parent) {
    this->recorder = recorder;

    toggle = new QCheckBox(this);
    save = new QPushButton(this);
    open = new QPushButton(this);
    path = new QLineEdit(this);

    this->setup();
}

RecorderUi::~RecorderUi() {
    delete toggle;
    delete save;
    delete open;
    delete path;
}

void RecorderUi::setup() {
    toggle->setText("Record");
    toggle->setCheckState(Qt::CheckState::Unchecked);
    toggle->setGeometry(QRect(20, 20, 200, 20));
    connect(toggle, &QCheckBox::clicked, this, &RecorderUi::on_change);

    save->setText("Save");
    save->setGeometry(QRect(220, 20, 100, 20));
    connect(save, &QPushButton::clicked, this, &RecorderUi::save_intervals);

    open->setText("Open");
    open->setGeometry(QRect(220, 50, 100, 20));
    connect(open, &QPushButton::clicked, this, &RecorderUi::open_intervals);

    path->setPlaceholderText("File path");
    path->setGeometry(QRect(20, 80, 300, 20));
}

void RecorderUi::on_change() {
    recorder->enable_recorder(toggle->isChecked());
}

void RecorderUi::open_intervals() {
    QString filename = path->text();

    if (filename.isEmpty()) {
        filename = QFileDialog::getOpenFileName(this, "Select the recording", plugin::data_folder().c_str(), PLUGIN_FILTER_FILE);

        if (filename.isEmpty()) return;

        if (!filename.endsWith(PLUGIN_SUFFIX_FILE)) {
            filename.append(PLUGIN_SUFFIX_FILE);
        }
    }

    update_intervals_path(filename);
}

void RecorderUi::save_intervals() {
    std::vector<long long> intervals = recorder->get_intervals();

    if (intervals.size() <= 0) {
        MessageBoxA(nullptr, "No intervals found in memory, try again.", "Error", MB_ICONERROR);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save the recording", plugin::data_folder().c_str(), PLUGIN_FILTER_FILE);

    if (filename.isEmpty()) {
        MessageBoxA(nullptr, "Invalid file name.", "Error", MB_ICONERROR);
        return;
    }

    if (!filename.endsWith(PLUGIN_SUFFIX_FILE)) {
        filename.append(PLUGIN_SUFFIX_FILE);
    }

    write_intervals(filename.toStdString(), intervals);
    update_intervals_path(filename);
}

void RecorderUi::write_intervals(std::string filename, std::vector<long long> intervals) {
    std::fstream output_file;
    output_file.open(filename, std::ios::out);

    for (int i = 1; i < intervals.size() - 1; i += 2) {
        output_file << intervals.at(i) - intervals.at(i - 1) << ":";
        output_file << intervals.at(i + 1) - intervals.at(i) << "\n";
    }

    output_file.close();
}

void RecorderUi::update_intervals_path(QString filename) {
    plugin::intervals_path = filename.toStdString();
    path->setText(filename);
}

void RecorderUi::toggle_recorder() {
    if (toggle->isChecked()) {
        toggle->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        toggle->setCheckState(Qt::CheckState::Checked);
    }

    on_change();
}
