# -------------------------------------------------
# Project created by QtCreator 2009-05-04T00:39:58
# -------------------------------------------------
QT += xml \
    phonon
TARGET = ui
TEMPLATE = app
INCLUDEPATH += . \
    .. \
    "G:\lib\v2.0\v2.0.include"
LIBS += -L"G:\lib\v2.0\v2.0.lib" \
    -lwlog \
    -L"." \
    -lcore
SOURCES += main.cpp \
    uibuilder.cpp \
    connector.cpp \
    View.cpp \
    LyricLine.cpp \
    flowlayout.cpp
HEADERS += uibuilder.h \
    connector.h \
    View.h \
    LyricLine.h \
    flowlayout.h
RESOURCES += lingoures.qrc
RC_FILE = lingou.rc
