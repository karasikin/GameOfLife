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

    const QBrush &aliveBrush() const;
    const QBrush &deadBrush() const;
    const QPen &pen() const;
    bool grid() const;

    void setAliveBrush(const QBrush &brush);
    void setDeadBrush(const QBrush &brush);
    void setPen(const QPen &pen);
    void setGrid(bool value);

signals:

    void mouseLeftButtonClickedByIndex(index_t row, index_t col);

protected:

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:

    void displayWorld(QPainter &painter);
    void setCell(index_t row, index_t col, QPainter &painter);
    void dropCell(index_t row, index_t col, QPainter &paiter);

    double xScale() const;
    double yScale() const;
    QRectF cellRect(index_t row, index_t col) const;
    QPoint fromPixelToIndex(const QPoint &index) const;

private:

    World &_world;

    QBrush _aliveBrush;
    QBrush _deadBrush;
    QPen _pen;
    bool _grid;

};

#endif // WORLDVIEW_H
