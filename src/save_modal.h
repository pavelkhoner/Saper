#ifndef SAPER_SAVE_MODAL_H
#define SAPER_SAVE_MODAL_H

#include <QDialog>
#include "../ui/ui_save_modal.h"
/// Класс модального окна сохранения игры
class SaveModal: public QDialog {
    Q_OBJECT
public:
    SaveModal(QWidget *parent = nullptr);
private:
    Ui_SaveModal ui;
private slots:
    /// Слот сохранения игры
    void save();
signals:
    /// Сигнал сохранения игры с отправкой названия сохранения
    void sendSave(QString saveName);
};


#endif //SAPER_SAVE_MODAL_H
