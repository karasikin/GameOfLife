#include "mainwindow.h"

#include <QToolBar>
#include <QDebug>


#include "worldview.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Настройки в будущем должны где-то  сожраняться и потом восстанавливаться
    const auto size{ std::tuple{10, 20} };
    const bool saveChangeList{ false };
    const World::NeighborCountingPolicy ncPolicy{ World::WITHOUT_BORDER };
    const auto timerInterval{ std::chrono::milliseconds{100} };
    /* Настройки WorldView не задыть добавить должен ты!!! */
    // Конец настройки

    _world = std::make_unique<World>(size, saveChangeList, ncPolicy);
    _world_view = new WorldView(*_world);

    _timer = std::make_unique<QTimer>();
    _timer->setInterval(timerInterval);
    connect(_timer.get(), &QTimer::timeout, this, &MainWindow::makeStep);

    addToolBar(Qt::TopToolBarArea, createToolBar());

    setCentralWidget(_world_view);
}


void MainWindow::startGame() {
    qDebug() << "start game slot colled";
    if(!_timer->isActive()) {
        _timer->start();
    }
}

void MainWindow::stopGame() {
    qDebug() << "stop game slot colled";
    if(_timer->isActive()) {
        _timer->stop();
    }
}

void MainWindow::makeStep() {
    if(_world->step()) {
        _world_view->update();
    } else {
        stopGame();
    }
}

QToolBar *MainWindow::createToolBar() const{
    auto tool_bar{ new QToolBar{"Tool Bar"} };

    tool_bar->addAction("Start", this, &MainWindow::startGame);
    tool_bar->addAction("Stop", this, &MainWindow::stopGame);

    return tool_bar;
}
