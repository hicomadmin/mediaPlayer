#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

//extern Config* Config::instance;
#include "Log.h"
class QSettings;
class QStringList;
class Config : public QObject
{
private:
    Config();
    static Config* m_pInstance;

public:
    static Config* instance();

public:
    QStringList musicFiltersList() const;
    QString musicFilters() const;

    QString lastOpenDir() const;
    void setLastOpenDir(const QString & str) const;

    QString workDir() const;

    QString currentSkinPath() const;
    void setCurrentSkinPath(const QString & strPath) const;

    QString musicDir() const;
    void setMusicDir(const QString & strPath) const;

    QString lyricDir() const;
    void setLyricDir(const QString & strPath) const;

    QString showTitleFormat() const;
    void setShowTitleFormat(const QString & str) const;

    bool mute() const;//æ≤“Ù
    void setMuted(bool bMuted = false) const;

    bool webFormVisible() const;//webForm
    void setWebFormVisible(bool bVisible= false) const;

    int volume() const; //“Ù¡ø
    void setVolume(int iVolume) const;
    
    QString getAvaibleLyricPath(QString artist, QString title, QString filePath, QString ext="lrc");
    QSettings* setting() const;

private:
    QSettings* m_pSetting;

};

#endif // CONFIG_H
