

SOURCES += \
    test.cpp \
    cryptohelper.cpp

HEADERS += \
    cryptohelper.h

TEMPLATE = app
TARGET = tst
CONFIG += warn_on qmltestcase

DISTFILES += \
    tst_test1.qml

VPATH += ../app ../app/oauth .
include (../app/oauth/oauth.pri)
