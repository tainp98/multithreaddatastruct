TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -lpthread

HEADERS += \
    linklistqueue.h \
    LockBuffer.h
