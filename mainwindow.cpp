#include "mainwindow.h"

#include <fstream>

#include <QApplication>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
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
    const auto fontSize{ 11 };
    /* Настройки WorldView не задыть добавить должен ты!!! */
    // Конец настройки


    /* font setting */
    auto font{ this->font() };
    font.setPointSize(fontSize);
    setFont(font);
    //


    _world = std::make_unique<World>(size, saveChangeList, ncPolicy);
    _previous_world = std::make_unique<World>(size, saveChangeList, ncPolicy);
    _world_view = new WorldView(*_world);

    _timer = std::make_unique<QTimer>();
    _timer->setInterval(timerInterval);

    addToolBar(Qt::TopToolBarArea, createToolBar());
    setMenuBar(createMenuBar());

    connect(_timer.get(), &QTimer::timeout, this, &MainWindow::onMakeStep);
    connect(_world_view, &WorldView::mouseLeftButtonClickedByIndex, this, &MainWindow::onWorldViewLeftMouseButtonClicked);
    connect(_set_world_size_btn, &QPushButton::clicked, this, &MainWindow::onChangeWorldSize);
    connect(_world_row_line_edit, &QLineEdit::returnPressed, this, &MainWindow::onChangeWorldSize);
    connect(_world_col_line_edit, &QLineEdit::returnPressed, this, &MainWindow::onChangeWorldSize);

    setCentralWidget(_world_view);

    _world_view->setFocus();
}

bool MainWindow::isGameRunning() const {
    return _timer->isActive();
}


void MainWindow::onStartGame() {
    qDebug() << "<startGame> slot colled";
    if(!isGameRunning()) {
        _previous_world = std::make_unique<World>(*_world);
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
    onStopGame();
    _world->clear();
    _world_view->update();
}

void MainWindow::onRestoreWorld() {
    qDebug() << "<restoreWorld> slot colled";
    onStopGame();
    *_world = *_previous_world;                // Подумать над обменом указателей для этого надо world & -> world * в WorldView
    _world_view->update();
}

void MainWindow::onShowGridToggle() {
    qDebug() << "<onShowGridToggle> slot colled";
    _world_view->setGrid(!_world_view->grid());
    _world_view->update();
}

void MainWindow::onBorderPolicyToggle() {
    qDebug() << "<onBorderPolicyToggle> slot colled";
    if(isGameRunning()) {
        onStopGame();
    }

    if(_world->neighborCountingPolicy() == World::WITHOUT_BORDER) {
        _world->setNeighborCountingPolicy(World::WITH_BORDER);
    } else {
        _world->setNeighborCountingPolicy(World::WITHOUT_BORDER);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Space) {
        if(isGameRunning()) {
            onStopGame();
        } else {
            onStartGame();
        }
        return;
    }

    if(event->matches(QKeySequence::Undo)) {
        onRestoreWorld();
        return;
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
    qDebug() << "<onChangeWorldSize> slot colled";

    bool row_ok{}, col_ok{};

    _world_view->setFocus();
    auto row_cout{ _world_row_line_edit->text().toULong(&row_ok) };
    auto col_cout{ _world_col_line_edit->text().toULong(&col_ok) };

    if(isGameRunning() || !row_ok || !col_ok) {
        restoreRowColEdit();
        return;
    }

    if(_world->row() == row_cout && _world->col() == col_cout) {
        return;
    }

    /// Читать конфигурацию из настроек!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    *_world = World{ {row_cout, col_cout}, false, World::WITHOUT_BORDER };
    _world_view->update();
}

void MainWindow::onSaveWorld() {
    qDebug() << "<onSaveWorld> slot colled()";

    const QString extansion{ ".wrld" }; // В настройки!!!

    if(isGameRunning()) {
        return;
    }

    auto fileName{ QFileDialog::getSaveFileName(this, "Save file", "", "*" + extansion) };
    if(!fileName.endsWith(extansion)) {
        fileName.append(extansion);
    }

    std::ofstream out{ fileName.toStdString() };

    if(out) {
        out << *_world;
        out.close();
    }
}

void MainWindow::onLoadWorld() {
    qDebug() << "<onLoadWorld> slot colled()";

    const QString extansion{ ".wrld" }; // В настройки!!!!!!!!

    if(isGameRunning()) {
        return;
    }

    auto fileName{ QFileDialog::getOpenFileName(this, "Open file", "", "*" + extansion) };
    std::ifstream in{ fileName.toStdString() };
    if(in) {
        in >> *_world;
        _world_view->update();
        restoreRowColEdit();
        in.close();
    }
}

QToolBar *MainWindow::createToolBar() {
    auto tool_bar{ new QToolBar{"Tool Bar"} };
    tool_bar->setFloatable(false);
    tool_bar->setMovable(false);
    tool_bar->setMinimumHeight(40);

    _world_row_line_edit = new QLineEdit{ QString::number(_world->row()), this };
    _world_row_line_edit->setMaximumWidth(70);
    tool_bar->addWidget(_world_row_line_edit);

    _world_col_line_edit = new QLineEdit{ QString::number(_world->col()), this };
    _world_col_line_edit->setMaximumWidth(70);
    tool_bar->addWidget(_world_col_line_edit);

    _set_world_size_btn = new QPushButton{ "Set new size", this };
    tool_bar->addWidget(_set_world_size_btn);

    tool_bar->addAction(QIcon(":images/resources/start_48.png"), "Start", this, &MainWindow::onStartGame);
    tool_bar->addAction(QIcon(":images/resources/stop_48.png"), "Stop", this, &MainWindow::onStopGame);

    tool_bar->addAction(QIcon(":images/resources/clear_48.png"), "Clear", this, &MainWindow::onClearWorld);
    tool_bar->addAction("Restore", this, &MainWindow::onRestoreWorld);

    return tool_bar;
}

QMenuBar *MainWindow::createMenuBar() {
    auto menu_bar{ new QMenuBar{} };

    auto file_menu{ new QMenu{"File", this} };
    file_menu->addAction("Save", this, &MainWindow::onSaveWorld, QKeySequence::Save);
    file_menu->addAction("Open", this, &MainWindow::onLoadWorld, QKeySequence::Open);
    file_menu->addAction("Quit", qApp, &QApplication::quit);

    auto world_menu{ new QMenu{"World", this} };
    world_menu->addAction("Step", this, &MainWindow::onMakeStep);
    world_menu->addAction("Start", this, &MainWindow::onStartGame);
    world_menu->addAction("Stop", this, &MainWindow::onStopGame);
    world_menu->addAction("Clear", this, &MainWindow::onClearWorld);
    world_menu->addAction("Restore", this, &MainWindow::onRestoreWorld);

    auto settings_menu{ new QMenu{"Settings", this} };

    auto showGridAction{ new QAction{"Show grid", this} };
    showGridAction->setCheckable(true);
    showGridAction->setChecked(_world_view->grid());
    connect(showGridAction, &QAction::triggered, this, &MainWindow::onShowGridToggle);
    settings_menu->addAction(showGridAction);


    auto borderPolicyAction{ new QAction{"World with borders"} };
    borderPolicyAction->setCheckable(true);
    borderPolicyAction->setChecked(_world->neighborCountingPolicy() == World::WITH_BORDER);
    connect(borderPolicyAction, &QAction::triggered, this, &MainWindow::onBorderPolicyToggle);
    settings_menu->addAction(borderPolicyAction);


    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(world_menu);
    menu_bar->addMenu(settings_menu);

    return menu_bar;
}

void MainWindow::restoreRowColEdit() {
    _world_col_line_edit->setText(QString::number(_world->col()));
    _world_row_line_edit->setText(QString::number(_world->row()));
}
