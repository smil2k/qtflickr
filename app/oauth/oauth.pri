VPATH += $PWD
INCLUDEPATH += $PWD

QT += network

HEADERS += \
    oauth/oauth.h \
    oauth/oauth_types.h \
    oauth/oauth_p.h \
    oauth/oauth_helper.h

SOURCES += \
    oauth/oauth.cpp \
    oauth/oauth_p.cpp
