#include "mainwindow.h"

#include <QToolBar>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDebug>


#include "worldview.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Настройки в будущем должны где-то  сохраняться и потом восстанавливаться
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

    addToolBar(Qt::TopToolBarArea, createToolBar());

    connect(_timer.get(), &QTimer::timeout, this, &MainWindow::onMakeStep);
    connect(_world_view, &WorldView::mouseLeftButtonClickedByIndex, this, &MainWindow::onWorldViewLeftMouseButtonClicked);
    connect(_set_world_size_btn, &QPushButton::clicked, this, &MainWindow::onChangeWorldSize);

    setCentralWidget(_world_view);
}

bool MainWindow::isGameRunning() const {
    return _timer->isActive();
}


void MainWindow::onStartGame() {
    qDebug() << "<startGame> slot colled";
    if(!isGameRunning()) {
        _timer->start();
    }
}

void MainWindow::onStopGame() {
    qDebug() << "<stopGame> slot colled";
    if(isGameRunning()) {
        _timer->stop();
    }
}

void MainWindow::onClearWorld() {
    qDebug() << "<clearWorld> slot colled";
    if(!isGameRunning()) {
        _world->clear();
        _world_view->update();
    }
}

void MainWindow::onMakeStep() {
    if(_world->step()) {
        _world_view->update();
    } else {
        onStopGame();
    }
}

void MainWindow::onWorldViewLeftMouseButtonClicked(unsigned long row, unsigned long col) {
    qDebug() << "<onWorldViewLeftMouseButtonClicked> slot colled";
    if(!isGameRunning()) {
        _world->inverse({row, col});
        _world_view->update();
    }
}

void MainWindow::onChangeWorldSize() {
    bool ok{};

    if(isGameRunning()) {
        _world_row_line_edit->setText(QString::number(_world->row()));
        _world_col_line_edit->setText(QString::number(_world->col()));
        return;
    }

    auto row_cout{ _world_row_line_edit->text().toULong(&ok) };
    if(!ok) {
        _world_row_line_edit->setText(QString::number(_world->row()));
        _world_col_line_edit->setText(QString::number(_world->col()));
        return;
    }

    auto col_cout{ _world_col_line_edit->text().toULong(&ok) };
    if(!ok) {
        _world_col_line_edit->setText(QString::number(_world->col()));
        _world_row_line_edit->setText(QString::number(_world->row()));
        return;
    }

    qDebug() << "<onChangeWorldSize> slot colled";

    /// Читать конфигурацию из настроек!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(_world->row() == row_cout && _world->col() == col_cout) {
        return;
    }

    *_world = World{ {row_cout, col_cout}, false, World::WITHOUT_BORDER };
    _world_view->update();
}

QToolBar *MainWindow::createToolBar() {
    auto tool_bar{ new QToolBar{"Tool Bar"} };
    tool_bar->setFloatable(false);
    tool_bar->setMovable(false);
    tool_bar->setMinimumHeight(40);

    _world_row_line_edit = new QLineEdit{ QString::number(_world->row()) };
    _world_row_line_edit->setMaximumWidth(70);
    tool_bar->addWidget(_world_row_line_edit);

    _world_col_line_edit = new QLineEdit{ QString::number(_world->col()) };
    _world_col_line_edit->setMaximumWidth(70);
    tool_bar->addWidget(_world_col_line_edit);

    _set_world_size_btn = new QPushButton{ "Set new size" };
    tool_bar->addWidget(_set_world_size_btn);

    tool_bar->addAction(QIcon(":images/resources/start_48.png"), "Start", this, &MainWindow::onStartGame);
    tool_bar->addAction(QIcon(":images/resources/stop_48.png"), "Stop", this, &MainWindow::onStopGame);

    tool_bar->addAction(QIcon(":images/resources/clear_48.png"), "Clear", this, &MainWindow::onClearWorld);

    return tool_bar;
}
