TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Client.cpp \
        main.cpp \
        message.cpp

HEADERS += \
    Client.h \
    Network_Interface.h \
    message.h
