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
#include "defaultsettings.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _world(initDefaultWorld()),
      _previous_world(initDefaultWorld()),
      _timer(std::make_unique<QTimer>()),
      _timer_interval(DefaultSettings::timerIntervaMs),
      _world_view(new WorldView{_world.get()}),
      _world_editor(nullptr)

{
    auto font{ this->font() };
    font.setPointSize(DefaultSettings::fontSize);
    setFont(font);

    _timer->setInterval(_timer_interval);

    initActions();
    initWidgets();

    addToolBar(Qt::LeftToolBarArea, createSettingsToolBar());
    addToolBar(Qt::TopToolBarArea, createControlToolBar());
    setMenuBar(createMenuBar());

    connect(_timer.get(), &QTimer::timeout, this, &MainWindow::onMakeStep);
    connect(_world_view, &WorldView::mouseLeftButtonClickedByIndex, this, &MainWindow::onWorldViewLeftMouseButtonClicked);
    connect(_set_world_size_btn, &QPushButton::clicked, this, &MainWindow::onChangeWorldSize);
    connect(_world_row_line_edit, &QLineEdit::returnPressed, this, &MainWindow::onChangeWorldSize);
    connect(_world_col_line_edit, &QLineEdit::returnPressed, this, &MainWindow::onChangeWorldSize);
    connect(_set_timer_interval_btn, &QPushButton::clicked, this, &MainWindow::onChangeTimerInterval);
    connect(_timer_line_edit, &QLineEdit::returnPressed, this, &MainWindow::onChangeTimerInterval);

    setCentralWidget(_world_view);

    _world_view->setFocus();

    //onOpenEditor();   // Временно Отладка!!!!
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

void MainWindow::onStepGame() {
    qDebug() << "<onStepGame> slot colled";

    if(!isGameRunning()) {
        onMakeStep();
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

    auto tmp{ std::move(_world) };                 // Самое тупое место в программе
    _world = std::move(_previous_world);           // Если что-то посыпится, сразу смотреть сюда
    _previous_world = std::move(tmp);              // Скорее всего проблемма будет здесь
    _world_view->setWorld(_world.get());           // Истправление World* сменить на World&, и юзать присваивание вместо свопа
//    if(_world_editor) _world_editor->setWorld(_world.get());

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
    *_world = World{{row_cout, col_cout}, DefaultSettings::isSaveLastStep, DefaultSettings::neighborCountingPolicy};
    _world_view->update();
}

void MainWindow::onChangeTimerInterval() {
    qDebug() << "<onChangeTimerInterval> slot colled";

    bool interval_ok;

    _world_view->setFocus();
    auto interval{ _timer_line_edit->text().toUInt(&interval_ok) };

    if(isGameRunning() || !interval_ok) {
        _timer_line_edit->setText(QString::number(_timer_interval));
        return;
    }

    if(_timer_interval == int(interval)) {
        return;
    }

    _timer_interval = interval;
    _timer->setInterval(std::chrono::milliseconds(_timer_interval));
}

void MainWindow::onSaveWorld() {
    qDebug() << "<onSaveWorld> slot colled()";
    if(isGameRunning()) {
        return;
    }

    QString filter{QString{"*%1"}.arg(QString::fromStdString(DefaultSettings::fileExtension))};
    auto fileName{ QFileDialog::getSaveFileName(this, "Save file", "", filter) };

    if(!fileName.endsWith(QString::fromStdString(DefaultSettings::fileExtension))) {
        fileName.append(QString::fromStdString(DefaultSettings::fileExtension));
    }

    std::ofstream out{ fileName.toStdString() };

    if(out) {
        out << *_world;
        out.close();
    }
}

void MainWindow::onLoadWorld() {
    qDebug() << "<onLoadWorld> slot colled()";

    if(isGameRunning()) {
        return;
    }

    QString filter{QString{"*%1"}.arg(QString::fromStdString(DefaultSettings::fileExtension))};
    auto fileName{ QFileDialog::getOpenFileName(this, "Open file", "", filter) };

    std::ifstream in{ fileName.toStdString() };
    if(in) {
        in >> *_world;
        _world_view->update();
        restoreRowColEdit();
        in.close();
    }
}

void MainWindow::onOpenEditor() {
    qDebug() << "<onWorldEditor> slot colled";

    onStopGame();

    if(!_world_editor) {
        _world_editor = std::make_unique<WorldEditor>();
        connect(_world_editor.get(), &WorldEditor::becomeHidden, this, &MainWindow::show);
        connect(_world_editor.get(), &WorldEditor::createdWorld, this, &MainWindow::onCreatedWorld);
    }

    this->hide();
    _world_editor->show();
}

void MainWindow::onCreatedWorld(const World &world) {
    qDebug() << "<onCreatedWorld> slot colled";
    _world = std::make_unique<World>(world);
    _world_view->setWorld(_world.get());
    restoreRowColEdit();
}

void MainWindow::initActions() {
    _restore_action = new QAction{QIcon(":images/resources/undo_64.png"), "Restore", this};
    _restore_action->setShortcut(QKeySequence::Refresh);
    connect(_restore_action, &QAction::triggered, this, &MainWindow::onRestoreWorld);

    _show_grid_action = new QAction{"Show grid", this};
    _show_grid_action->setCheckable(true);
    _show_grid_action->setChecked(_world_view->grid());
    connect(_show_grid_action, &QAction::triggered, this, &MainWindow::onShowGridToggle);

    _border_policy_action = new QAction{"World with borders", this};
    _border_policy_action->setCheckable(true);
    _border_policy_action->setChecked(_world->neighborCountingPolicy() == World::WITH_BORDER);
    connect(_border_policy_action, &QAction::triggered, this, &MainWindow::onBorderPolicyToggle);
}

void MainWindow::initWidgets() {
    _world_row_line_edit = new QLineEdit{ QString::number(_world->row()), this };
    _world_row_line_edit->setMaximumWidth(50);
    _world_col_line_edit = new QLineEdit{ QString::number(_world->col()), this };
    _world_col_line_edit->setMaximumWidth(50);
    _set_world_size_btn = new QPushButton{ "Resize", this };

    _timer_line_edit = new QLineEdit{"100", this};   // Временные меры
    _timer_line_edit->setMaximumWidth(50);
    _set_timer_interval_btn = new QPushButton{"Set interval", this};
}

QToolBar *MainWindow::createSettingsToolBar() {
    auto tool_bar{ new QToolBar{"Settings Toolbar"} };

    tool_bar->addWidget(new QLabel{ "Size: ", this });
    tool_bar->addWidget(_world_row_line_edit);
    tool_bar->addWidget(_world_col_line_edit);
    tool_bar->addWidget(_set_world_size_btn);
    tool_bar->addSeparator();

    tool_bar->addWidget(new QLabel{"Timer: ", this});
    tool_bar->addWidget(_timer_line_edit);
    tool_bar->addWidget(_set_timer_interval_btn);

    _settings_tool_bar_view_action = tool_bar->toggleViewAction();
    tool_bar->hide();

    return tool_bar;
}


QToolBar *MainWindow::createControlToolBar() {
    auto tool_bar{ new QToolBar{"Control Toolbar"} };
    tool_bar->setFloatable(false);
    tool_bar->setMovable(false);
    tool_bar->setMinimumHeight(40);

    _settings_tool_bar_view_action->setIcon(QIcon(":images/resources/view_64.png"));
    _settings_tool_bar_view_action->setShortcut(QKeySequence::NextChild);

    tool_bar->addAction(_settings_tool_bar_view_action);
    tool_bar->addSeparator();
    tool_bar->addAction(QIcon(":images/resources/step_64.png"), "Step", this, &MainWindow::onStepGame);
    tool_bar->addAction(QIcon(":images/resources/start_64.png"), "Start", this, &MainWindow::onStartGame);
    tool_bar->addAction(QIcon(":images/resources/stop_64.png"), "Stop", this, &MainWindow::onStopGame);
    tool_bar->addAction(_restore_action);
    tool_bar->addSeparator();
    tool_bar->addAction(QIcon(":images/resources/clear_64.png"), "Clear", this, &MainWindow::onClearWorld);
    tool_bar->addAction("Open editor", this, &MainWindow::onOpenEditor);

    return tool_bar;
}

QMenuBar *MainWindow::createMenuBar() {
    auto menu_bar{ new QMenuBar{} };

    auto file_menu{ new QMenu{"File", this} };
    file_menu->addAction("Save", this, &MainWindow::onSaveWorld, QKeySequence::Save);
    file_menu->addAction("Open", this, &MainWindow::onLoadWorld, QKeySequence::Open);
    file_menu->addAction("Quit", qApp, &QApplication::quit);
    menu_bar->addMenu(file_menu);

    auto world_menu{ new QMenu{"World", this} };
    world_menu->addAction("Step", this, &MainWindow::onStepGame);
    world_menu->addAction("Start", this, &MainWindow::onStartGame);
    world_menu->addAction("Stop", this, &MainWindow::onStopGame);
    world_menu->addAction("Clear", this, &MainWindow::onClearWorld);
    world_menu->addAction("Restore", this, &MainWindow::onRestoreWorld);
    menu_bar->addMenu(world_menu);

    auto settings_menu{ new QMenu{"Settings", this} };
    settings_menu->addAction(_show_grid_action);
    settings_menu->addAction(_border_policy_action);
    menu_bar->addMenu(settings_menu);

    return menu_bar;
}

void MainWindow::restoreRowColEdit() {
    _world_col_line_edit->setText(QString::number(_world->col()));
    _world_row_line_edit->setText(QString::number(_world->row()));
}

std::unique_ptr<World> MainWindow::initDefaultWorld() const {
    return std::make_unique<World>(
                std::tuple{DefaultSettings::worldRows, DefaultSettings::worldCols},
                DefaultSettings::isSaveLastStep,
                DefaultSettings::neighborCountingPolicy);
}
