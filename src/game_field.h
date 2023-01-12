#ifndef SAPER_GAME_FIELD_H
#define SAPER_GAME_FIELD_H

#include <QMainWindow>
#include "../ui/ui_game_field.h"
#include "game_objects/field_cell.h"
#include "./game_over.h"
#include "./save_modal.h"
#include "./download_modal.h"
#include <QString>
#include <string>
#include <QtSql>

/// Класс игрового окна
class GameField : public QMainWindow {
Q_OBJECT
public:
	GameField(QWidget *parent = nullptr);
private:
	Ui_GameField ui;
	QGraphicsScene* scene;
	QVector<QVector<FieldCell*>> fieldCells;

	std::map<std::string, std::map<std::string, int>> difficulties;
	std::map<std::string, int> currentDifficulty;

	int width = 300;
	int height = 300;
	int rowCellsNumber = 9;
	int colCellsNumber  = 9;
	int cellSize = width / std::min(rowCellsNumber, colCellsNumber);
	int bombsNumber = 15;
	int flagsLeftNumber = bombsNumber;

    int gameId;
    bool isRepeat = false;
    QString saveName;
    bool isSave = false;
    int stepNumber = 0;

    GameOver* gameOverModal;
    SaveModal* saveModal;
    DownloadModal* downloadModal;
    /// Полностью перегенирирует игру
    void initGame();
    /// Подключается к базе данных
    void initDB();
    /// Заполняет параметры игры и игрового поля в зависимости от выбранной сложности
	void setupParams();
    /// Рандомно генерирует поле в зависимости от параметров
	void generateField();
    /// Возвращает список соседей клетки
	std::vector<FieldCell *> getNeighbors(int rowId, int colId);
    /// Рассчитывает количество бомб рядом с каждой клеткой на поле
	void calculateCellNumbers();
    /// Рекурсивно открывает клетки, которые не являются бомбами, вокруг изначальной
	void openArea(int rowId, int colId);
    /// Открывает все бомбы на поле
	void openAll();
    /// Проверяет, выйграта ли игра
	bool checkWin();
    /// Вызывает модальное окно победы
    void win();
    /// Вызывает модальное окно проигрыша
    void gameOver();
    /// Записывает в БД ход игрока
    void storeStep(int rowId, int colId, bool opened);
    /// Записывает в БД местоположение бомбы
    void storeBomb(int rowId, int colId);
    /// Ставит флаг завершения игры в БД
    void finishGame();
    /// Делает сохраненный ход на основании записи из БД
    void makeSavedMove(QSqlQuery query);
public slots:
    /// Отрабатывает нажатие левой кнопкой мыши на ячейку
	void processLeftCellClick(int rowId, int colId);
    /// Отрабатывает нажатие правой кнопкой мыши на ячейку
	void processRightCellClick(int rowId, int colId);
    /// Отрабатывает изменение сложности
    void processComboBoxChange(const QString&);
    /// Начинает игру заново
    void restart();
    /// Показывает повтор предыдущей игры
    void showRepeat();
    /// Открывает модальное окно для сохранения
    void showSaveModal();
    /// Сохраняет текущую игру
    void saveGame(QString saveName);
    /// Открывает модальное окно для загрузки
    void openDownloadModal();
    /// Загружает выбранную игры
    void downloadGame(QString saveName);
    /// Обрабатывает сигнал о сохранении после окончания игры
    void processSave();
    /// Обрабатывает сигнал о сохранении после окончания игры
    void processDownload();
};

#endif //SAPER_GAME_FIELD_H
