#include <QDebug>
#include "download_modal.h"

DownloadModal::DownloadModal(QWidget *parent) :
        QDialog(parent) {
    ui.setupUi(this);
    this->setModal(true);

    listModel = new QStringListModel();
    ui.listView->setModel(listModel);
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(download()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DownloadModal::showEvent( QShowEvent* event ) {
    QWidget::showEvent(event);
    setSavings();
}

void DownloadModal::setSavings() {
    QSqlQuery query;
    query.prepare(R"(SELECT DISTINCT name from Saves)");
    query.exec();

    QStringList list;
    while (query.next()) {
        list<<query.value(0).toString();
    }
    listModel->setStringList(list);
}

void DownloadModal::download() {
    emit sendDownload(ui.listView->currentIndex().data(Qt::DisplayRole).toString());
}