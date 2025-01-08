#include "recorder_ui.h"

RecorderUi::RecorderUi(Recorder* recorder, QWidget* parent) : QWidget(parent) {
    this->recorder = recorder;

    main_layout = new QVBoxLayout(this);
    top_row = new QWidget(this);
    top_layout = new QHBoxLayout(top_row);
    toggle = new QCheckBox(top_row);
    save = new QPushButton(top_row);
    open = new QPushButton(top_row);
    path = new QLineEdit(this);

    this->setup();
}

RecorderUi::~RecorderUi() {
    delete main_layout;
    delete top_row;
    delete top_layout;
    delete toggle;
    delete save;
    delete open;
    delete path;
}

void RecorderUi::setup() {
    toggle->setText("Record");
    toggle->setCheckState(Qt::CheckState::Unchecked);
    connect(toggle, &QCheckBox::clicked, this, &RecorderUi::on_change);
    top_layout->addWidget(toggle);

    save->setText("Save\nintervals");
    connect(save, &QPushButton::clicked, this, &RecorderUi::save_intervals);
    top_layout->addWidget(save);

    open->setText("Open\nintervals");
    connect(open, &QPushButton::clicked, this, &RecorderUi::open_intervals);
    top_layout->addWidget(open);

    top_row->setLayout(top_layout);
    main_layout->addWidget(top_row);

    path->setPlaceholderText("Intervals path");
    main_layout->addWidget(path);

    setLayout(main_layout);
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
        MessageBoxA(nullptr, "No intervals found in memory.", "Error", MB_ICONERROR);
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
