#ifndef LYRICLOADER_H
#define LYRICLOADER_H

#include <QObject>
#include <QVector>
#include "Log.h"

class LyricInfo {
    public:
    QString artist;
    QString title;
    QString album;
    QString author;
    int offset;
    public:
    LyricInfo(const QString & strArtist = "",
              const QString & strTitle = "",
              const QString & strAlbum = "",
              const QString & strAuthor = "",
              int nOffset = 0)
    {
        this->artist = strArtist;
        this->title = strTitle;
        this->album = strAlbum;
        this->author = strAuthor;
        this->offset = nOffset;
    }

    void dump()
    {
        logoutput("-----------------");
        logoutput("artist = %s",artist.toLocal8Bit().data());
        logoutput("title  = %s",title.toLocal8Bit().data());
        logoutput("album  = %s",album.toLocal8Bit().data());
        logoutput("author = %s",author.toLocal8Bit().data());
        logoutput("offset = %s",offset);
        logoutput("-----------------");
    }
};

class LyricNode {
public:
    enum NodeType {
        NT_STEP,
        NT_SENT,
    };
    int hour;
    int min;
    int sec;
    int msec;
    NodeType type;
    QString lrc;
public:
    LyricNode(int nMillSeconds = 0, int nSeconds = 0, int nMinutes = 0, const QString & strLrc = "", NodeType eType = NT_SENT, int nHours = 0)
    {
        this->msec = nMillSeconds;
        this->sec = nSeconds;
        this->min = nMinutes;
        this->lrc = strLrc;
        this->type = eType;
        this->hour = nHours;
    }

    void dump()
    {
        logoutput("-----------------");
        logoutput("hour = ",hour);
        logoutput("min  = ",min);
        logoutput("sec  = ",sec);
        logoutput("msec = ",msec);
        logoutput("type = ",type);
        logoutput("hour = ",hour);
        logoutput("lrc  = ",lrc.toLocal8Bit().data());
        logoutput("-----------------");
    }

};


class LyricLoader : public QObject
{
    Q_OBJECT
public:
    LyricLoader(const QString& strPath);
    virtual ~LyricLoader();

    virtual bool loadFromFile(const QString& strPath = "") = 0;
    bool reloadLyric();

    QString lyricPath();
    void setLyricPath(const QString & strPath);

    LyricInfo* info();
    QVector<LyricNode*>* nodes();

protected:
    void addNode(LyricNode* pNewNode);
    virtual void destroy();
    virtual void init();

signals:
    void meetError(ErrorType, const QString & strWhy);

private:
    QString m_strPath;
    LyricInfo* m_info;
    QVector<LyricNode*>* m_nodes;
};

#endif // LYRICLOADER_H
