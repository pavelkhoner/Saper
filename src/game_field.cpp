#include "game_field.h"
#include <iostream>

GameField::GameField(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	difficulties["Легкий"] = std::map<std::string, int> {
                                                         {"rowCellsNumber", 9}, {"colCellsNumber", 9},
                                                         {"bombsNumber", 9}, {"cellSize", 30}};

	difficulties["Средний"] = std::map<std::string, int> {
                                                          {"rowCellsNumber", 12},{"colCellsNumber", 12},
                                                          {"bombsNumber", 30}, {"cellSize", 30}};

	difficulties["Сложный"] = std::map<std::string, int> {
                                                          {"rowCellsNumber", 16}, {"colCellsNumber", 20},
                                                          {"bombsNumber", 80}, {"cellSize", 30}};

	currentDifficulty = difficulties["Легкий"];

    initDB();
	initGame();

    gameOverModal = new GameOver();
    saveModal = new SaveModal();
    downloadModal = new DownloadModal();
    connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(restart()));
    connect(ui.pushButton_3, SIGNAL(clicked(bool)), this, SLOT(openDownloadModal()));
    connect(ui.pushButton_2, SIGNAL(clicked(bool)), this, SLOT(showSaveModal()));
	connect(ui.comboBox, SIGNAL(currentIndexChanged(const QString&)), this,
            SLOT(processComboBoxChange(const QString &)));
    connect(gameOverModal, SIGNAL(restart()), this, SLOT(restart()));
    connect(gameOverModal, SIGNAL(save()), this, SLOT(processSave()));
    connect(gameOverModal, SIGNAL(download()), this, SLOT(processDownload()));
    connect(saveModal, SIGNAL(sendSave(QString)), this, SLOT(saveGame(QString)));
    connect(downloadModal, SIGNAL(sendDownload(QString)), this, SLOT(downloadGame(QString)));
}

void GameField::initDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    db.open();
}

void GameField::setupParams() {
    rowCellsNumber = currentDifficulty["rowCellsNumber"];
    colCellsNumber = currentDifficulty["colCellsNumber"];
    cellSize = currentDifficulty["cellSize"];
	width = cellSize * colCellsNumber + 50;
	height = cellSize * rowCellsNumber + 50;
	bombsNumber = currentDifficulty["bombsNumber"];
	flagsLeftNumber = bombsNumber;
    ui.label->setText(QString::number(flagsLeftNumber));
    this->resize(width + 200, height + 200);
    ui.graphicsView->setMinimumSize(width, height);
    if (isSave) {
        QSqlQuery query;
        query.prepare(R"(SELECT MAX(number) from Moves WHERE game_id=?)");
        query.addBindValue(gameId);
        query.exec();
        query.first();

        stepNumber = query.value(0).toInt()+1;
    } else {
        stepNumber = 0;
    }
}

int getRandomInt(int limit) {
	return rand() % limit;
}

void GameField::generateField() {
    scene = new QGraphicsScene(this);
    ui.graphicsView->setScene(scene);

	fieldCells = QVector<QVector<FieldCell*>>(rowCellsNumber, QVector<FieldCell*>(colCellsNumber));

	for (int rowId = 0; rowId < rowCellsNumber; rowId++) {
		for (int colId = 0; colId < colCellsNumber; colId++) {
			FieldCell* fieldCell = new FieldCell(rowId, colId, cellSize, cellSize);
			scene-> addItem(fieldCell);
			connect(fieldCell, &FieldCell::leftCellClick, this, &GameField::processLeftCellClick);
			connect(fieldCell, &FieldCell::rightCellClick, this, &GameField::processRightCellClick);
			fieldCells[rowId][colId] = fieldCell;
		}
	}

    if (isRepeat || isSave) {
        QSqlQuery query;
        query.prepare(R"(SELECT row_id, col_id from Bombs WHERE game_id=?)");
        query.addBindValue(gameId);
        query.exec();

        while (query.next()) {
            fieldCells[query.value(0).toInt()][query.value(1).toInt()]->setBomb();
        }
    } else {
        for (int bombId = 0; bombId < bombsNumber; bombId++) {
            int bombRowId = getRandomInt(rowCellsNumber);
            int bombColId = getRandomInt(colCellsNumber);
            if (!fieldCells[bombRowId][bombColId]->getIsBomb()) {
                fieldCells[bombRowId][bombColId]->setBomb();
                storeBomb(bombRowId, bombColId);
            } else {
                bombId--;
            }
        }
    }

	calculateCellNumbers();

    if (isSave) {
        QSqlQuery query;
        query.prepare(R"(
        SELECT row_id, col_id, opened, flagged FROM Moves
        INNER JOIN Saves on Moves.id = Saves.move_id
        WHERE name=?
        ORDER BY number)");
        query.addBindValue(saveName);
        query.exec();

        while(query.next()) {
            qDebug()<< "qe";
            makeSavedMove(query);
        }
    }
}

void GameField::initGame() {
    if (!(isRepeat || isSave)) {
        QSqlQuery query;
        query.prepare(R"(INSERT INTO Games(difficulty) VALUES(:difficulty))");
        query.bindValue(":difficulty", ui.comboBox->currentText());
        query.exec();
        gameId = query.lastInsertId().toInt();
    }

    setupParams();
    generateField();
}

void GameField::calculateCellNumbers() {
	for (int rowId = 0; rowId < rowCellsNumber; rowId++) {
		for (int colId = 0; colId < colCellsNumber; colId++) {
			int cellNumber = 0;
			std::vector<FieldCell *> neighbors = getNeighbors(rowId, colId);
			for (FieldCell* neighbor : neighbors) {
				if (neighbor->getIsBomb()) cellNumber++;
			}
			fieldCells[rowId][colId]->setCellNumber(cellNumber);
		}
	}
}

std::vector<FieldCell *> GameField::getNeighbors(int rowId, int colId) {
	std::vector<FieldCell *> neighbors;
	if (rowId > 0) {
		neighbors.push_back(fieldCells[rowId - 1][colId]);

		if (colId > 0) neighbors.push_back(fieldCells[rowId - 1][colId - 1]);

		if (colId < colCellsNumber - 1) neighbors.push_back(fieldCells[rowId - 1][colId + 1]);
	}

	if (rowId < rowCellsNumber - 1) {
		neighbors.push_back(fieldCells[rowId + 1][colId]);

		if (colId > 0) neighbors.push_back(fieldCells[rowId + 1][colId - 1]);

		if (colId < colCellsNumber - 1) neighbors.push_back(fieldCells[rowId + 1][colId + 1]);
	}

	if (colId > 0) neighbors.push_back(fieldCells[rowId][colId - 1]);

	if (colId < colCellsNumber - 1) neighbors.push_back(fieldCells[rowId][colId + 1]);

	return neighbors;
}

void GameField::processLeftCellClick(int rowId, int colId) {
    if (isRepeat) {
        QSqlQuery query;
        query.prepare(R"(SELECT row_id, col_id, opened, flagged FROM Moves WHERE game_id=:gameId AND number=:number)");
        query.bindValue(":gameId", gameId);
        query.bindValue(":number", stepNumber);
        query.exec();
        query.first();

        makeSavedMove(query);

        stepNumber++;
    } else {
        if (fieldCells[rowId][colId]->getIsFlagged() || fieldCells[rowId][colId]->getIsOpened()) {
            return;
        }

        storeStep(rowId, colId, 1);

        if (fieldCells[rowId][colId]->getIsBomb()) {
            fieldCells[rowId][colId]->explode();
            openAll();
            gameOver();
        } else {
            openArea(rowId, colId);
            if (checkWin()) {
                win();
            }
        }
    }
}

void GameField::makeSavedMove(QSqlQuery query) {
    int stepRowId = query.value(0).toInt();
    int stepColId = query.value(1).toInt();
    qDebug() << stepRowId;
    if (query.value(2).toBool()) {
        // opened
        if (fieldCells[stepRowId][stepColId]->getIsBomb()) {
            fieldCells[stepRowId][stepColId]->explode();
            openAll();
            gameOver();
        } else {
            qDebug() << "open";
            openArea(stepRowId, stepColId);
            if (checkWin()) {
                win();
            }
        }
    } else {
        // flagged
        fieldCells[stepRowId][stepColId]->setFlag();
        if (fieldCells[stepRowId][stepColId]->getIsFlagged()) {
            flagsLeftNumber--;
        } else {
            flagsLeftNumber++;
        }
        ui.label->setText(QString::number(flagsLeftNumber));
    }
}

void GameField::storeStep(int rowId, int colId, bool opened) {
    QSqlQuery query;
    query.prepare(R"(INSERT INTO Moves(game_id, row_id, col_id, number, opened, flagged) VALUES(:gameId, :rowId, :colId, :number, :opened, :flagged))");
    query.bindValue(":gameId", gameId);
    query.bindValue(":rowId", rowId);
    query.bindValue(":colId", colId);
    query.bindValue(":number", stepNumber);
    query.bindValue(":opened", int(opened));
    query.bindValue(":flagged", int(!opened));
    query.exec();

    stepNumber++;
}

void GameField::storeBomb(int rowId, int colId) {
    QSqlQuery query;
    query.prepare(R"(INSERT INTO Bombs(game_id, row_id, col_id) VALUES(:gameId, :rowId, :colId))");
    query.bindValue(":gameId", gameId);
    query.bindValue(":rowId", rowId);
    query.bindValue(":colId", colId);
    query.exec();
}

void GameField::openAll() {
	for (int rowId = 0; rowId < rowCellsNumber; rowId++) {
		for (int colId = 0; colId < colCellsNumber; colId++) {
            if (fieldCells[rowId][colId]->getIsBomb()) {
                fieldCells[rowId][colId]->open();
                if (fieldCells[rowId][colId]->getIsFlagged()) fieldCells[rowId][colId]->setFlag();
            }
		}
	}
}

void GameField::processRightCellClick(int rowId, int colId) {
    if (isRepeat) return;

    if (!fieldCells[rowId][colId]->getIsOpened()) {
        fieldCells[rowId][colId]->setFlag();

        storeStep(rowId, colId, 0);

        if (fieldCells[rowId][colId]->getIsFlagged()) {
            flagsLeftNumber--;
        } else {
            flagsLeftNumber++;
        }

        ui.label->setText(QString::number(flagsLeftNumber));
    }
}

void GameField::openArea(int rowId, int colId) {
	fieldCells[rowId][colId]->open();
	std::vector<FieldCell *> neighbors = getNeighbors(rowId, colId);
	for (FieldCell* neighbor : neighbors) {
		if (!neighbor->getIsOpened() && !neighbor->getIsBomb()) {
            if (fieldCells[neighbor->getRowId()][neighbor->getColId()]->getIsFlagged()) {
                fieldCells[neighbor->getRowId()][neighbor->getColId()]->setFlag();
                flagsLeftNumber++;
                ui.label->setText(QString::number(flagsLeftNumber));
            }
			if (neighbor->getCellNumber() == 0) {
				openArea(neighbor->getRowId(), neighbor->getColId());
			} else {
				neighbor->open();
			}
		}
	}
}

bool GameField::checkWin() {
	for (int rowId = 0; rowId < rowCellsNumber; rowId++) {
		for (int colId = 0; colId < colCellsNumber; colId++) {
			if (!fieldCells[rowId][colId]->getIsOpened() && !fieldCells[rowId][colId]->getIsBomb()) {
				return false;
			}
		}
	}

	return true;
}

void GameField::processComboBoxChange(const QString &) {
    isRepeat = false;
    isSave = false;
    currentDifficulty = difficulties[ui.comboBox->currentText().toStdString()];
    initGame();
}

void GameField::restart() {
    isRepeat = false;
    isSave = false;
    initGame();
    gameOverModal->hide();
}

void GameField::showRepeat() {
    isRepeat = true;
    isSave = false;
    initGame();
    gameOverModal->hide();
}

void GameField::showSaveModal() {
    saveModal->show();
}

void GameField::saveGame(QString gameName) {
    QSqlQuery query;
    query.prepare(R"(
    INSERT INTO Saves(move_id, name)
    SELECT id, :name from Moves where game_id=:gameId
    )");
    query.bindValue(":gameId", gameId);
    query.bindValue(":name", gameName);
    query.exec();
    saveModal->hide();
}

void GameField::openDownloadModal() {
    downloadModal->show();
}

void GameField::downloadGame(QString saveName) {
    this->saveName = saveName;
    qDebug() << saveName;
    QSqlQuery query;
    query.prepare(R"(
    SELECT game_id, finished FROM Moves
    INNER JOIN Saves ON Moves.id=Saves.move_id
    INNER JOIN Games ON Moves.game_id=Games.id
    where name=?
    )");
    query.addBindValue(saveName);
    query.exec();
    query.first();
    gameId = query.value(0).toInt();
    bool finished = query.value(1).toBool();
    isRepeat = finished;
    isSave = !finished;
    initGame();
    downloadModal->hide();
    gameOverModal->hide();
}

void GameField::win() {
    finishGame();
    gameOverModal->setMessage("ВЫ ПОБЕДИЛИ!");
    gameOverModal->show();
}

void GameField::gameOver() {
    finishGame();
    gameOverModal->setMessage("ВЫ ПРОИГРАЛИ(");
    gameOverModal->show();
}

void GameField::finishGame() {
    QSqlQuery query;
    query.prepare(R"(
    UPDATE Games SET finished=1 WHERE id=:gameId
    )");
    query.bindValue(":gameId", gameId);
    query.exec();
}

void GameField::processSave() {
    saveModal->show();
    saveModal->show();
}

void GameField::processDownload() {
    downloadModal->show();
}
