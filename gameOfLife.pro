TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

LIBS += -pthread

SOURCES += \
        main.cpp \
        world.cpp \
        worldview.cpp

HEADERS += \
    world.h \
    worldview.h

QT += widgets
