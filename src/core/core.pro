# #####################################################################
# Automatically generated by qmake (2.01a) ??? ?? 3 19:43:50 2009
# #####################################################################
TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . \
    "G:\lib\v2.0\v2.0.include" \
    "G:\lib\taglib-1.5-mingw-bin\include\taglib"
LIBS += -L"G:\lib\v2.0\v2.0.lib" \
    -lwlog \
    -L"G:\lib\taglib-1.5-mingw-bin\lib" \
    -ltag
QT += phonon
QT += xml

# Input
HEADERS += SkinLoader.h \
    SoundCore.h \
    ComponentAttr.h \
    LGSkinLoader.h \
    playlistmodel.h \
    musictag.h \
    Log.h \
    config.h \
    BuffOperator.h \
    StringUtil.h \
    LyricParser.h \
    Lrc.h \
    lyricmanager.h
SOURCES += SoundCore.cpp \
    SkinLoader.cpp \
    ComponentAttr.cpp \
    LGSkinLoader.cpp \
    playlistmodel.cpp \
    musictag.cpp \
    config.cpp \
    Lrc.cpp \
    BuffOperator.cpp \
    StringUtil.cpp \
    LyricParser.cpp \
    lyricmanager.cpp