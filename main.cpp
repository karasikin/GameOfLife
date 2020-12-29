#include <thread>
#include <QApplication>
#include <QDebug>

#include "mainwindow.h"


#include "worldeditor.h"


int main(int argc, char **argv) {

    QApplication app{ argc, argv };
    MainWindow mw{};
    mw.show();

//    WorldEditor we;
//    we.show();
    return app.exec();
}
