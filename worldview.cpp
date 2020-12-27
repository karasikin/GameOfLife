#include "worldview.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include "world.h"

WorldView::WorldView(World *world, QWidget *parent)
    : QWidget(parent),
      _world(world),
      _grid(true)
{
    // Когда добавлю настройки тут все буду брать из них

    _pen.setColor({0xbf, 0xbf, 0xbf});
    _pen.setWidth(2);

    _aliveBrush.setColor(Qt::black);
    _aliveBrush.setStyle(Qt::SolidPattern);
    _deadBrush.setColor(Qt::white);
    _deadBrush.setStyle(Qt::SolidPattern);
}

const QBrush &WorldView::aliveBrush() const { return _aliveBrush; }
const QBrush &WorldView::deadBrush() const { return _deadBrush; }
const QPen &WorldView::pen() const { return _pen; }
bool WorldView::grid() const { return _grid; }

void WorldView::setAliveBrush(const QBrush &brush) { _aliveBrush = brush; }
void WorldView::setDeadBrush(const QBrush &brush) { _deadBrush = brush; }
void WorldView::setPen(const QPen &pen) { _pen = pen; }

void WorldView::setGrid(bool value) {

    /////////////////// Настройки !!!!!!!!!!

    _grid = value;
    if(value) {
        _pen.setWidth(2);
        _pen.setColor({0xbf, 0xbf, 0xbf});
    } else {
        _pen.setWidth(0);
        _pen.setColor({0xff, 0xff, 0xff});
    }
}

void WorldView::setWorld(World *world) {
    _world = world;
}

void WorldView::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };

    painter.setPen(_pen);

    displayWorld(painter);

    QWidget::paintEvent(event);
}

void WorldView::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        auto point{ fromPixelToIndex(event->pos()) };
        emit mouseLeftButtonClickedByIndex(point.x(), point.y());
    }

    QWidget::mousePressEvent(event);
}

void WorldView::displayWorld(QPainter &painter) {
    for(auto i{ 0ul }; i < _world->row(); ++i) {
        for(auto j{ 0ul }; j < _world->col(); ++j) {
            if(_world->test({i, j})) {
                setCell(i, j, painter);
            } else {
                dropCell(i, j, painter);
            }
        } // end_for
    } // end_for
}

void WorldView::setCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush(_aliveBrush);
    painter.drawRect(cellRect(row, col));
}

void WorldView::dropCell(index_t row, index_t col, QPainter &painter) {
    painter.setBrush(_deadBrush);
    painter.drawRect(cellRect(row, col));
}

double WorldView::xScale() const {
    return double(width()) / _world->col();
}

double WorldView::yScale() const {
    return double(height()) / _world->row();
}

QRectF WorldView::cellRect(index_t row, index_t col) const {
    return { col * xScale(), row * yScale(), xScale(), yScale() };
}

QPoint WorldView::fromPixelToIndex(const QPoint &index) const {
    return { int(index.y() / yScale()), int(index.x() / xScale()) };
}
