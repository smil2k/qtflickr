

SOURCES += \
    test.cpp

HEADERS +=

TEMPLATE = app
TARGET = tst
CONFIG += warn_on qmltestcase
CONFIG += qml_debug

DISTFILES += \
    tst_test1.qml

VPATH += ../app ../app/oauth .
INCLUDEPATH += ../app  .
include (../app/oauth/oauth.pri)
