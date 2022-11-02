#include "game_over.h"

GameOver::GameOver(QWidget *parent) :
        QDialog(parent) {
    ui.setupUi(this);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(sendShowRepeat()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(sendRestart()));

//    this->setWindowFlags(Qt::Dialog | Qt::Desktop);
}

void GameOver::sendRestart() {
    emit restart();
}

void GameOver::sendShowRepeat() {
    emit showRepeat();
}

void GameOver::setMessage(QString messageVal) {
    message = messageVal;
    ui.label->setText(message);
}
