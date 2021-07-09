TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    testLockBuff.cpp

LIBS += -lpthread

HEADERS += \
    linklistqueue.h \
    LockBuffer.h \
    Matrix.h \
    Writer.h \
    Reader1.h \
    Reader2.h \
    Reader3.h

## opencv  4
LIBS += `pkg-config --libs opencv4`
INCLUDEPATH += /usr/local/include/opencv4
#============ pthread
LIBS += -lpthread
