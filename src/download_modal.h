#ifndef DOWNLOAD_MODAL_H
#define DOWNLOAD_MODAL_H

#include <QtSql>
#include <QDialog>
#include <QStringListModel>
#include "../ui/ui_download_modal.h"
/// Класс модального окна со списком сохранений для загрузки
class DownloadModal: public QDialog {
    Q_OBJECT
public:
    DownloadModal(QWidget *parent = nullptr);
private:
    Ui_DownloadModal ui;
    QStringListModel * listModel;
    /// Переопределенная функия открытия окна
    void showEvent( QShowEvent* event );
    /// Функция, заполняющая список сохранений
    void setSavings();
private slots:
    /// Слот загрузки игры
    void download();
signals:
    /// Сигнал загрузки игры
    void sendDownload(QString saveName);
};


#endif //DOWNLOAD_MODAL_H