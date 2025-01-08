#include "injector_ui.h"

InjectorUi::InjectorUi(QWidget* parent) : QWidget(parent) { 
    label = new QLabel(this);
    open = new QPushButton(this);
    remove = new QPushButton(this);
    path = new QLineEdit(this);
    list = new QListWidget(this);
    
    this->setup();
}

InjectorUi::~InjectorUi() {
    delete label;
    delete open;
    delete remove;
    delete path;
    delete list;
}

void InjectorUi::setup() {
    label->setText("Inject");
    label->setGeometry(QRect(20, 20, 200, 20));

    open->setText("Open\nmodule");
    open->setGeometry(QRect(220, 20, 100, 50));
    connect(open, &QPushButton::clicked, this, &InjectorUi::open_module);

    remove->setText("Remove\nmodule");
    remove->setGeometry(QRect(330, 20, 100, 50));
    connect(remove, &QPushButton::clicked, this, &InjectorUi::remove_module);

    path->setPlaceholderText("Module path");
    path->setGeometry(QRect(20, 80, 400, 20));

    list->setGeometry(QRect(20, 120, 460, 140));
    list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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