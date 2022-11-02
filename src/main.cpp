#include <iostream>
#include <QApplication>
#include "game_field.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	GameField gameField;
	gameField.show();
	return app.exec();
}
