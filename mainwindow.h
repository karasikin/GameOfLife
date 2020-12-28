#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "worldeditor.h"

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
    void onChangeTimerInterval();

    void onSaveWorld();
    void onLoadWorld();

    void onOpenEditor();

private:

    void initActions();
    void initWidgets();
    QToolBar *createSettingsToolBar();
    QToolBar *createControlToolBar();
    QMenuBar *createMenuBar();
    void restoreRowColEdit();
    std::unique_ptr<World> initDefaultWorld() const;

private:

    std::unique_ptr<World> _world;
    std::unique_ptr<World> _previous_world;

    std::unique_ptr<QTimer> _timer;
    int _timer_interval;

    WorldView *_world_view;

    std::unique_ptr<WorldEditor> _world_editor;

    QLineEdit *_world_row_line_edit;
    QLineEdit *_world_col_line_edit;
    QLineEdit *_timer_line_edit;
    QPushButton *_set_world_size_btn;
    QPushButton *_set_timer_interval_btn;

    QAction *_settings_tool_bar_view_action;
    QAction *_restore_action;
    QAction *_show_grid_action;
    QAction *_border_policy_action;
};

#endif // MAINWINDOW_H
