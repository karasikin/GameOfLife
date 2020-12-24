#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "world.h"

class WorldView;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

public slots:

    void startGame();
    void stopGame();

private slots:

    void makeStep();

private:

    QToolBar *createToolBar() const;

private:

    std::unique_ptr<World> _world;
    WorldView *_world_view;

    std::unique_ptr<QTimer> _timer;

};

#endif // MAINWINDOW_H
