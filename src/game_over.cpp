#include "game_over.h"

GameOver::GameOver(QWidget *parent) :
        QDialog(parent) {
    ui.setupUi(this);
    this->setModal(true);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(sendSave()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(sendDownload()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(sendRestart()));

//    this->setWindowFlags(Qt::Dialog | Qt::Desktop);
}

void GameOver::sendRestart() {
    emit restart();
}

void GameOver::sendSave() {
    emit save();
}

void GameOver::sendDownload() {
    emit download();
}


void GameOver::setMessage(QString messageVal) {
    message = messageVal;
    ui.label->setText(message);
}
