#include "field_cell.h"
#include <iostream>
#include <QDebug>

FieldCell::FieldCell(int rowId_, int colId_, qreal width_, qreal height_, QGraphicsItem *parent)
: QGraphicsRectItem(colId_ * width_, rowId_ * height_, width_, height_, parent) {
	rowId = rowId_;
	colId = colId_;
	size = width_;
}

void FieldCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
	if (isOpened) {
		QPen pen;
		pen.setColor(QColor(131, 131, 131));
		pen.setWidth(4);
		painter->setPen(pen);
		if (isBomb) {
            QImage image;
            if (isExploded) {
                image = QImage("./assets/exploded_bomb.png");
            } else {
                image = QImage("./assets/bomb.png");
            }
			painter->drawRect(colId * size, rowId * size, size, size);
			painter->drawImage(QRect(colId * size + 2, rowId * size + 2, size - 4, size - 4), image);
		} else {
			painter->setBrush(QBrush(QColor(198, 198, 198)));
			painter->drawRect(colId * size, rowId * size, size, size);
		}
	} else {
        if (isFlagged) {
            QImage image("./assets/flag.png");
            painter->drawImage(QRect(colId * size, rowId * size, size, size), image);
            return;
        } else {
            painter->setPen(Qt::NoPen);

            QPolygon polygon1;
            polygon1 << QPoint(colId * size, rowId * size)
                     << QPoint((colId + 1) * size, rowId * size)
                     << QPoint(colId * size, (rowId + 1) * size);
            painter->setBrush(QBrush(QColor(255, 255, 255)));
            painter->drawPolygon(polygon1);

            QPolygon polygon2;
            painter->setBrush(QBrush(QColor(128, 128, 128)));
            polygon2 << QPoint((colId + 1) * size, (rowId + 1) * size)
                     << QPoint((colId + 1) * size, rowId * size)
                     << QPoint(colId * size, (rowId + 1) * size);
            painter->drawPolygon(polygon2);

            painter->setBrush(QBrush(QColor(198, 198, 198)));
            painter->drawRect(colId * size + size/8, rowId * size + size/8, size - size/4, size - size/4);
        }
	}
}

void FieldCell::setBomb() {
	isBomb = true;
	this->setBrush(QBrush(Qt::red));
}

void FieldCell::setFlag() {
	isFlagged = !isFlagged;
    update();
}

void FieldCell::explode() {
    isExploded = true;
}

void FieldCell::open() {
	isOpened = true;
	if (!isBomb && cellNumber > 0) {
		cellNumberText = new QGraphicsTextItem(QString::number(cellNumber), this);
        QFont font("MS Shell Dlg 2", 12);
        cellNumberText->setFont(font);
		cellNumberText->setPos(colId * size, rowId * size);
	}
	update();
}

void FieldCell::setCellNumber(int cellNumberVal) {
	cellNumber = cellNumberVal;

}

void FieldCell::mousePressEvent (QGraphicsSceneMouseEvent *event) {
	if (event->buttons() & Qt::LeftButton)
		{
            emit leftCellClick(rowId, colId);
		}
	else if (event->buttons() & Qt::RightButton)
		{
            emit rightCellClick(rowId, colId);
		}
}


