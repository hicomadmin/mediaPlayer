#ifndef LYRICMANAGER_H
#define LYRICMANAGER_H

#include <QObject>
#include "LyricParser.h"

class LyricManager : public QObject
{
    Q_OBJECT
private:
    LyricManager();
    public:
    static LyricManager* instance();
    static LyricManager* m_LM;

    ~LyricManager();
public slots:
    bool setNewLyricFileName(const QString & strLyricFileName,long totalTime, const QString & strMusicShowTitle);

signals:
    void startNewLyric(const QString & strFileName,bool founded);
    void updateProcess(long begin, long cur, long end);
    void nextStep();
    void nextSentence();
    void playCurLyric();
    void pauseCurLyric();
    void stopCurLyric();

    public slots:
    void scroll(long time, long totalTime);
    void playLyric();
    void pauseLyric();
    void stopLyric();

    public:
    LyricNode* curNode();
    void setCurIndex(int iWhere = 0);
    bool hasNext();
    LyricNode* nextNode();
    int curIndex();
    int nextIndex();
    int totalCount();
    int findIndexByTime(long time, int indexFrom = 0);

    QString m_curLyricFileName;
    LyricParser* m_pLrc;
    int m_iCurIndex;
    long m_totalTime;

    bool m_curLyricIsOK;
};

#endif // LYRICMANAGER_H
