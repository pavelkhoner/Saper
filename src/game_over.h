#ifndef SAPER_GAME_OVER_H
#define SAPER_GAME_OVER_H

#include <QDialog>
#include "../ui/ui_game_over.h"

/// Класс модального окна с сообщением о результате игры
class GameOver : public QDialog {
    Q_OBJECT
public:
    GameOver(QWidget *parent = nullptr);
    /// Позволяет изменить сообщение окна извне
    void setMessage(QString messageVal);
private:
    Ui_GameOver ui;
    QString message;
private slots:
    /// Отправляет сигнал о начале новой игры
    void sendRestart();
    /// Отправляет сигнал о показе повтора игры
    void sendShowRepeat();

signals:
    /// Сигнал о начале новой игры
    void restart();
    /// Сигнал о показе повтора игры
    void showRepeat();
};

#endif //SAPER_GAME_OVER_H
