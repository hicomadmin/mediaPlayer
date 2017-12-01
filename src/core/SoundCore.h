#ifndef _SOUNDCORE_H_
#define _SOUNDCORE_H_


#include <Phonon>

class MediaObject;
class AudioOutput;
class Path;

#include "Log.h"

class SoundCore : public QObject
{
    Q_OBJECT

private:
    SoundCore();
    static SoundCore* m_pInstance;
public:
    static  SoundCore* instance();

public:
    enum PlayErrorType
    {
        NoMedia2Play,
        CommonError,
        FatalError
    };

    ~SoundCore();
    Phonon::MediaObject* mediaObj();
    Phonon::AudioOutput* audioOut();

    qint64 totalTime();
    qint64 currentTime();
    bool isMuted();
    
    bool isPlaying();
    bool isPause();
    bool isStop();

signals:
    void whenPlayMeetError(PlayErrorType errorType, const QString & errorMsg);
    void state2Loading();
    void state2Buffering();
    void state2Play();
    void state2Pause();
    void state2Stop();
    void about2Finish();
    void finish();
    void startPlay(const QString & fileName);
    void timeTick(qint64 time);

public slots:
    void play();
    void pause();
    void stop();
    void clearQueue();
    void seek(qint64 time);
    void setMuted();
    void setVolume(qreal value);
    
    void changeUrlTo(const QString strUrl);
    void setCurUrl(const QString strUrl);
    void pushUrl2Queue(const QString strUrl);
    void test(qint64 time);
private slots:
    void stateChanged(Phonon::State newState, Phonon::State oldState);

private:    
    Phonon::MediaObject     *m_pMediaObject;
    Phonon::AudioOutput     *m_pAudioOut;    
    Phonon::Path            m_audioOutPath;
};






#endif //_SOUNDCORE_H_
