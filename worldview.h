#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QBrush>
#include <QPen>

class World;

class WorldView : public QWidget
{

    Q_OBJECT

public:

    using index_t = unsigned long;

public:

    explicit WorldView(World &world, QWidget *parent = nullptr);

signals:

    void mouseLeftButtonClickedByIndex(index_t row, index_t col);

protected:

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:

    void drawGrid(QPainter &paiter);
    void displayWorld(QPainter &painter);
    void setCell(index_t row, index_t col, QPainter &painter);
    void dropCell(index_t row, index_t col, QPainter &paiter);

    double xScale() const;
    double yScale() const;
    QRectF cellRect(index_t row, index_t col) const;
    QPoint fromPixelToIndex(const QPoint &index) const;

private:

    World &_world;

    QBrush aliveBrush;
    QBrush deadBrush;
    QPen pen;

};

#endif // WORLDVIEW_H
