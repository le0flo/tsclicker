#include "injector_ui.h"

InjectorUi::InjectorUi(QWidget* parent) : QWidget(parent) {
    main_layout = new QVBoxLayout(this);
    top_row = new QWidget(this);
    top_layout = new QHBoxLayout(top_row);
    label = new QLabel(top_row);
    open = new QPushButton(top_row);
    remove = new QPushButton(top_row);
    path = new QLineEdit(this);
    list = new QListWidget(this);
    
    this->setup();
}

InjectorUi::~InjectorUi() {
    delete main_layout;
    delete top_row;
    delete top_layout;
    delete label;
    delete open;
    delete remove;
    delete path;
    delete list;
}

void InjectorUi::setup() {
    label->setText("Inject");
    top_layout->addWidget(label);

    open->setText("Open\nmodule");
    connect(open, &QPushButton::clicked, this, &InjectorUi::open_module);
    top_layout->addWidget(open);

    remove->setText("Remove\nmodule");
    connect(remove, &QPushButton::clicked, this, &InjectorUi::remove_module);
    top_layout->addWidget(remove);

    top_row->setLayout(top_layout);
    main_layout->addWidget(top_row);

    path->setPlaceholderText("Module path");
    main_layout->addWidget(path);

    list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    main_layout->addWidget(list);

    setLayout(main_layout);
}

void InjectorUi::open_module() {
    QString filename = path->text();

    if (filename.isEmpty()) {
        filename = QFileDialog::getOpenFileName(this, "Select the addon", plugin::desktop_folder().c_str(), PLUGIN_FILTER_ADDON);

        if (filename.isEmpty()) return;

        if (!filename.endsWith(PLUGIN_SUFFIX_ADDON)) {
            filename.append(PLUGIN_SUFFIX_ADDON);
        }
    }

    append_module(filename.toStdString());
}

void InjectorUi::append_module(std::string filename) {
    path->setText(filename.c_str());

    QListWidgetItem* item;
    ModuleUi* widget;

    for (int i = 0; i < list->count(); i++) {
        item = list->item(i);
        widget = dynamic_cast<ModuleUi*>(list->itemWidget(item));

        if (widget->filename == filename) {
            return;
        }
    }

    item = new QListWidgetItem();
    widget = new ModuleUi(filename, this);
    
    item->setSizeHint(widget->sizeHint());

    list->addItem(item);
    list->setItemWidget(item, widget);
}

void InjectorUi::remove_module() {
    QListWidgetItem* item = list->currentItem();
    
    if (item == nullptr) return;

    ModuleUi* widget = dynamic_cast<ModuleUi*>(list->itemWidget(item));

    widget->remove_module();
    delete item;
}