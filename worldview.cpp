#include "worldview.h"

#include <QPainter>
#include <QMouseEvent>

#include "world.h"

WorldView::WorldView(World &world, QWidget *parent)
    : QWidget(parent),
      _world(world)
{
    // Когда добавлю настройки тут все буду брать из них

    setPalette({ Qt::white });
    setAutoFillBackground(true);

    pen.setColor(Qt::black);

    aliveBrush.setColor(Qt::black);
    aliveBrush.setStyle(Qt::SolidPattern);
    deadBrush.setColor(Qt::white);
    deadBrush.setStyle(Qt::SolidPattern);
}

void WorldView::paintEvent(QPaintEvent *event) {

    QPainter painter{ this };

    painter.setPen(pen);

    drawGrid(painter);
    displayWorld(painter);

    QWidget::paintEvent(event);
}

void WorldView::mousePressEvent(QMouseEvent *event) {
    auto point{ fromPixelToIndex(event->pos()) };

    _world.inverse({point.x(), point.y()});

    update();

    QWidget::mousePressEvent(event);
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
}

void WorldView::displayWorld(QPainter &painter) {
    for(auto i{ 0ul }; i < _world.row(); ++i) {
        for(auto j{ 0ul }; j < _world.col(); ++j) {
            if(_world.test({i, j})) {
                setCell(i, j, painter);
            } else {
                dropCell(i, j, painter);
            }
        } // end_for
    } // end_for
}

void WorldView::setCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush(aliveBrush);
    painter.drawRect(cellRect(row, col));
}

void WorldView::dropCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush(deadBrush);
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

QPoint WorldView::fromPixelToIndex(const QPoint &index) const {
    return { int(index.y() / yScale()), int(index.x() / xScale()) };
}
