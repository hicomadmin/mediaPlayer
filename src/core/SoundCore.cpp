#include "SoundCore.h"

#include <Phonon/AudioOutput>
#include <Phonon/BackendCapabilities>
#include <Phonon/MediaObject>
#include <Phonon/VideoWidget>
#include <Phonon/Path>
#include "Log.h"

SoundCore* SoundCore::m_pInstance = 0;

SoundCore::SoundCore()
{
    m_pMediaObject = new Phonon::MediaObject;
    m_pAudioOut = new Phonon::AudioOutput(Phonon::MusicCategory);
    m_audioOutPath = Phonon::createPath(m_pMediaObject, m_pAudioOut);
    Q_ASSERT(m_audioOutPath.isValid());

    m_pMediaObject->setTickInterval(1000);

    connect(m_pMediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
    // connect(m_pMediaObject, SIGNAL(tick(qint64)), this, SLOT(test(qint64)));
    // connect(this, SIGNAL(timeTick(qint64)), this, SLOT(test(qint64)));
    connect(m_pMediaObject, SIGNAL(tick(qint64)), this, SIGNAL(timeTick(qint64)));
    connect(m_pMediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(about2Finish()));
    connect(m_pMediaObject, SIGNAL(finish()), this, SIGNAL(finish()));
}


SoundCore* SoundCore::instance()
{
    if(!m_pInstance)
    {
        m_pInstance = new SoundCore;
    }
    return m_pInstance;
}

SoundCore::~SoundCore()
{
    stop();
}

Phonon::MediaObject* SoundCore::mediaObj()
{
    return m_pMediaObject;
}

Phonon::AudioOutput* SoundCore::audioOut()
{
    return m_pAudioOut;
}

qint64 SoundCore::totalTime()
{
    return this->m_pMediaObject->totalTime();
}

qint64 SoundCore::currentTime()
{
    return this->m_pMediaObject->currentTime();
}

bool SoundCore::isMuted()
{
    return m_pAudioOut->isMuted();
}

void SoundCore::seek(qint64 time)
{
    logtrace("seek...%lld", time);
    this->m_pMediaObject->seek(time);
}

bool SoundCore::isPlaying()
{
    return this->m_pMediaObject->state() == Phonon::PlayingState;
}
bool SoundCore::isPause()
{
    return this->m_pMediaObject->state() == Phonon::PausedState;
}
bool SoundCore::isStop()
{
    return this->m_pMediaObject->state() == Phonon::StoppedState;
}

void SoundCore::setMuted()
{
    m_pAudioOut->setMuted(!m_pAudioOut->isMuted());
}

void SoundCore::setVolume(qreal value)
{
    m_pAudioOut->setVolume(value*0.01);
}

void SoundCore::test(qint64 time)
{
    logtrace("test...%ld", time);
    // emit timeTick(time);
}

void SoundCore::play()
{
    if(this->m_pMediaObject->currentSource().Invalid)
    {
        this->m_pMediaObject->play();
    }
    else
    {
        emit whenPlayMeetError(NoMedia2Play, tr("no available url to play for current!"));
    }
}

void SoundCore::pause()
{
    this->m_pMediaObject->pause();
}

void SoundCore::stop()
{
    this->m_pMediaObject->stop();
}

void SoundCore::clearQueue()
{
    this->m_pMediaObject->clearQueue();
}

void SoundCore::changeUrlTo(const QString strUrl)
{
    this->stop();

    this->m_pMediaObject->setCurrentSource(Phonon::MediaSource(strUrl));
    emit startPlay(strUrl);
    //this->pushUrl2Queue(strUrl);
    this->play();
}
void SoundCore::setCurUrl(const QString strUrl)
{
    clearQueue();
    this->m_pMediaObject->setCurrentSource(Phonon::MediaSource(strUrl));
}

void SoundCore::pushUrl2Queue(const QString strUrl)
{
    this->m_pMediaObject->enqueue(Phonon::MediaSource(strUrl));
}

void SoundCore::stateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    logonlytrace;
    switch (newState)
    {
    case Phonon::ErrorState:
        {
            if (this->m_pMediaObject->errorType() == Phonon::FatalError)
            {
                emit whenPlayMeetError(FatalError, m_pMediaObject->errorString());
            }
            else
            {
                emit whenPlayMeetError(CommonError, m_pMediaObject->errorString());
            }
            break;
        }
    case Phonon::LoadingState:
        emit state2Loading();
        break;
    case Phonon::BufferingState:
        emit state2Buffering();
        break;
    case Phonon::PlayingState:
        {
            logtrace("PlayingState....");
            emit state2Play();
            break;
        }
    case Phonon::PausedState:
        emit state2Pause();
        break;
    case Phonon::StoppedState:
        emit state2Stop();
        break;
    default:
        logerror("what this state(%d)", newState);
        break;
    }
}
