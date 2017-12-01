#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QDesktopServices>
#include <QFileInfo>
#include "config.h"

Config* Config::m_pInstance = 0;

Config::Config()        
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, workDir());
    m_pSetting = new QSettings(QSettings::IniFormat,QSettings::UserScope, "config", "lingou");
}

Config* Config::instance()
{
    if(!m_pInstance)
    {
        m_pInstance = new Config;
    }
    return m_pInstance;
}

QSettings* Config::setting() const
{
    return m_pSetting;
}

QStringList Config::musicFiltersList() const
{
    QStringList ret;
    ret << "*.wma" << "*.mp3" << "*.wav" <<"*.rm";
    return ret;
}

QString Config::musicFilters() const
{
    return QString("Musics (*.wma *.mp3 *.wav *.rm);; All Files(*)");
}

QString Config::musicDir() const
{
    setting()->beginGroup("Path");
    QString strRet = setting()->value("MusicDir", this->workDir()+"/musics").toString();
    setting()->endGroup();
    return strRet;
}

void Config::setMusicDir(const QString & strPath) const
{
    setting()->beginGroup("Path");
    setting()->setValue("MusicDir", strPath);
    setting()->endGroup();
}

QString Config::lyricDir() const
{
    setting()->beginGroup("Path");
    QString strRet = setting()->value("LyricDir", this->workDir()+"/lyrics").toString();
    setting()->endGroup();
    return strRet;
}

void Config::setLyricDir(const QString & strPath) const
{
    setting()->beginGroup("Path");
    setting()->setValue("LyricDir", strPath);
    setting()->endGroup();
}

QString Config::getAvaibleLyricPath(QString artist, QString title, QString filePath, QString ext/*="lrc"*/)
{
#define EXIST_RET     checkExist.setFile(strRet);\    
    if(checkExist.exists())\
    {\
     logtrace("check lyric file:(%s) exists", strRet.toUtf8().data());\
     return strRet;\
 }\
else\
{\
 logtrace("check lyric file:(%s) not exists", strRet.toUtf8().data());\
}

#define  CHECK_DIR(checkedDir)  strRet = checkedDir + "/" + fileNameExt;\
    EXIST_RET\
            strRet = checkedDir + "/" + artist + "-" + title + "." + ext;\
                     EXIST_RET\
                     strRet = checkedDir + "/" + artist + " - " + title + "." + ext;\
                              EXIST_RET\
                              strRet = checkedDir + "/" + artist + "/" + title + "." + ext;\
                                       EXIST_RET

                                       QFileInfo info(filePath);
    QString fileCurDir = info.absolutePath();
    QString lyricDir = this->lyricDir();
    QString musicDir = this->musicDir();
    QString fileNameNoExt = info.baseName();
    QString fileNameExt = fileNameNoExt + "." + ext;

    QString strRet;
    QFileInfo checkExist;
    //1. current dir, the same file name with different ext; artist-title.ext; ; artist - title.ext; artist/title.EXT;
    CHECK_DIR(fileCurDir);
    CHECK_DIR(lyricDir);
    CHECK_DIR(musicDir);

    return "";
}
QString Config::lastOpenDir() const
{
    setting()->beginGroup("Path");
    QString strRet = setting()->value("LastOpenDir", musicDir()).toString();
    setting()->endGroup();
    return strRet;
}

void Config::setLastOpenDir(const QString & str) const
{
    setting()->beginGroup("Path");
    setting()->setValue("LastOpenDir", str);
    setting()->endGroup();
}

QString Config::workDir() const
{
    return QDir::currentPath();
}

QString Config::currentSkinPath() const
{
    setting()->beginGroup("Path");
    QString strRet = setting()->value("CurrentSkinPath", "default.skin").toString();
    setting()->endGroup();
    return workDir()+"/"+strRet;
}

void Config::setCurrentSkinPath(const QString & strPath) const
{
    setting()->beginGroup("Path");
    setting()->setValue("CurrentSkinPath", strPath);
    setting()->endGroup();
}

QString Config::showTitleFormat() const
{
    setting()->beginGroup("UserDefine");
    QString strRet = setting()->value("TitleFormat", "%a - %t").toString();
    setting()->endGroup();
    return strRet;
}

void Config::setShowTitleFormat(const QString & str) const
{
    setting()->beginGroup("UserDefine");
    setting()->setValue("TitleFormat", str);
    setting()->endGroup();
}

bool Config::mute() const
{
    setting()->beginGroup("PlayerSettings");
    bool bRet = setting()->value("Mute", false).toBool();
    setting()->endGroup();
    return bRet;    
}

void Config::setMuted(bool bMuted /*= false*/) const
{
    setting()->beginGroup("PlayerSettings");
    setting()->setValue("Mute", bMuted);
    setting()->endGroup();
}

bool Config::webFormVisible() const
{
    setting()->beginGroup("PlayerSettings");
    bool bRet = setting()->value("WebFormVisible", false).toBool();
    setting()->endGroup();
    return bRet;
}

void Config::setWebFormVisible(bool bVisible/*= false*/) const
{
    setting()->beginGroup("PlayerSettings");
    setting()->setValue("WebFormVisible", bVisible);
    setting()->endGroup();
}

int Config::volume() const
{
    setting()->beginGroup("PlayerSettings");
    int iRet = setting()->value("Volume", 88).toInt();
    setting()->endGroup();
    return iRet;     
}

void Config::setVolume(int iVolume) const
{
    setting()->beginGroup("PlayerSettings");
    setting()->setValue("Volume", iVolume);
    setting()->endGroup();
}

