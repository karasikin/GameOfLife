#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>

class World;

class WorldView : public QWidget
{

    Q_OBJECT

public:

    using index_t = unsigned long;

public:

    explicit WorldView(World &world, QWidget *parent = nullptr);

protected:

    void paintEvent(QPaintEvent *event) override;
    void drawGrid(QPainter &painter);
    void setCell(index_t row, index_t col, QPainter &painter);
    void dropCell(index_t row, index_t col, QPainter &painter);

private:

    double xScale() const;
    double yScale() const;
    QRectF cellRect(index_t row, index_t col) const;

private:

    World &_world;

};

#endif // WORLDVIEW_H
