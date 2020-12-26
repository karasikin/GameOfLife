#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "world.h"

class WorldView;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    bool isGameRunning() const;

public slots:

    void onStartGame();
    void onStopGame();
    void onStepGame();
    void onClearWorld();
    void onRestoreWorld();

    void onShowGridToggle();
    void onBorderPolicyToggle();

private:

    void keyPressEvent(QKeyEvent *event);

private slots:

    void onMakeStep();
    void onWorldViewLeftMouseButtonClicked(unsigned long row, unsigned long col);
    void onChangeWorldSize();

    void onSaveWorld();
    void onLoadWorld();

private:

    QToolBar *createSettingsToolBar();
    QToolBar *createControlToolBar();
    QMenuBar *createMenuBar();
    void restoreRowColEdit();

private:

    std::unique_ptr<World> _world;
    std::unique_ptr<World> _previous_world;

    std::unique_ptr<QTimer> _timer;

    WorldView *_world_view;

    QLineEdit *_world_row_line_edit;
    QLineEdit *_world_col_line_edit;
    QPushButton *_set_world_size_btn;

    QLineEdit *_timer_line_edit;
    QPushButton *_set_timer_interval_btn;

    QAction *_settings_tool_bar_view_action;
};

#endif // MAINWINDOW_H
