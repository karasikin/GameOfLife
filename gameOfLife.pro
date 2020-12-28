TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

LIBS += -pthread

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        world.cpp \
        worldeditor.cpp \
        worldview.cpp

HEADERS += \
    defaultsettings.h \
    mainwindow.h \
    world.h \
    worldeditor.h \
    worldview.h

QT += widgets

RESOURCES += \
    resources.qrc
