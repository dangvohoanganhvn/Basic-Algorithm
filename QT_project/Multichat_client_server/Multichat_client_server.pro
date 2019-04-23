TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Server.cpp \
        main.cpp \
        message.cpp

HEADERS += \
    Network_Interface.h \
    Server.h \
    message.h
