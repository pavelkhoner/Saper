#ifndef SAPER_FIELD_CELL_H
#define SAPER_FIELD_CELL_H

#include <QObject>
#include <QtWidgets>
#include <QtSvg>

/// Класс игровой ячейки, наследуемый от QGraphicsRectItem
class FieldCell : public QObject, public QGraphicsRectItem {
	Q_OBJECT
public:
	FieldCell(int rowId_, int colId_, qreal width_, qreal height_, QGraphicsItem *parent = nullptr);
    /// Переопределенная функция рисования ячейки
	void FieldCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
    /// Геттер переменной rowId
	int getRowId() { return rowId; }
    /// Геттер переменной ColId
	int getColId() { return colId; }
    /// Геттер переменной isBomb
	bool getIsBomb() { return isBomb; };
    /// Геттер переменной isOpened
	bool getIsOpened() { return isOpened; }
    /// Геттер переменной isFlagged
	bool getIsFlagged() { return isFlagged; }
    /// Геттер переменной cellNumber
	int getCellNumber() { return cellNumber; }

    /// Сеттер переменной cellNumber
	void setCellNumber(int cellNumber);
    /// Сеттер переменной isBomb
	void setBomb();
    /// Сеттер переменной isExploded
    void explode();
    /// Сеттер переменной isFlagged
	void setFlag();
    /// Сеттер переменной isOpened
	void open();
    /// Переопределенная функция нажатия на ячейку
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
	int rowId;
	int colId;
	int size;
	QGraphicsTextItem* cellNumberText;

	bool isBomb = false;
    bool isExploded = false;
	bool isOpened = false;
	bool isFlagged = false;
	bool isOpenedNumber = false;
	int cellNumber = 0;
signals:
    /// Сигнал о нажатии левой кнопкой мыши по ячейке
	void leftCellClick(int rowId, int colId);
    /// Сигнал о нажатии правой кнопкой мыши по ячейке
	void rightCellClick(int rowId, int colId);
};


#endif //SAPER_FIELD_CELL_H
