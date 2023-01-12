#include "save_modal.h"

SaveModal::SaveModal(QWidget *parent) :
        QDialog(parent) {
    ui.setupUi(this);
    this->setModal(true);
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void SaveModal::save() {
    emit sendSave(ui.lineEdit->text());
    ui.lineEdit->setText("");
}