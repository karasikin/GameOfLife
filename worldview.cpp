#include "worldview.h"

#include <QPainter>

#include "world.h"

WorldView::WorldView(World &world, QWidget *parent)
    : QWidget(parent),
      _world(world)
{
    setPalette({ Qt::white });
    setAutoFillBackground(true);
}

void WorldView::paintEvent(QPaintEvent *event) {

    QPainter painter{ this };
    QPen pen{ Qt::black };

    painter.setPen(pen);

    drawGrid(painter);

    QWidget::paintEvent(event);
}

void WorldView::drawGrid(QPainter &painter) {
    auto scale_x{ xScale() };

    for(auto i{ 0ul }; i < _world.col(); ++i) {
        painter.drawLine(scale_x * i, 0, scale_x * i, height());
    }

    auto scale_y{ yScale() };

    for(auto i{ 0ul }; i < _world.row(); ++i) {
        painter.drawLine(0, scale_y * i, width(), scale_y * i);
    }

    setCell(0, 0, painter);
    setCell(2, 2, painter);
    dropCell(2, 2, painter);
}

void WorldView::setCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush({ Qt::black });
    painter.drawRect(cellRect(row, col));
}

void WorldView::dropCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush({ Qt::white });
    painter.drawRect(cellRect(row, col));
}

double WorldView::xScale() const {
    return double(width()) / _world.col();
}

double WorldView::yScale() const {
    return double(height()) / _world.row();
}

QRectF WorldView::cellRect(index_t row, index_t col) const {
    return { col * xScale(), row * yScale(), xScale(), yScale() };
}



