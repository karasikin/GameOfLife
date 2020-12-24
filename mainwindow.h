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
    void onClearWorld();

private slots:

    void onMakeStep();
    void onWorldViewLeftMouseButtonClicked(unsigned long row, unsigned long col);
    void onChangeWorldSize();

private:

    QToolBar *createToolBar();

private:

    std::unique_ptr<World> _world;
    WorldView *_world_view;

    std::unique_ptr<QTimer> _timer;

    QLineEdit *_world_row_line_edit;
    QLineEdit *_world_col_line_edit;
    QPushButton *_set_world_size_btn;
};

#endif // MAINWINDOW_H
