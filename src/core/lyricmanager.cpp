#include "lyricmanager.h"

#include "Lrc.h"

LyricManager* LyricManager::m_LM = 0;

LyricManager* LyricManager::instance()
{
    if(!m_LM)
    {
        m_LM = new LyricManager();
    }
    return m_LM;
}

LyricManager::LyricManager()
    :m_pLrc(0),m_iCurIndex(-1),m_totalTime(0),m_curLyricIsOK(false)
{

}
LyricManager::~LyricManager()
{
    if(m_pLrc)
    {
        delete m_pLrc;
        m_pLrc = 0;
    }
}

bool LyricManager::setNewLyricFileName(const QString &strLyricFileName,long totalTime,const QString &strMusicShowTitle)
{
    logtrace("you set the filename to %s, and total time is %ld", strLyricFileName.toUtf8().data(), totalTime);
    if(m_pLrc)
    {logonlytrace;
        stopLyric();
        logonlytrace;
        delete m_pLrc;
        m_pLrc = 0;
    }
    logonlytrace;
    m_curLyricFileName = strLyricFileName;
    logonlytrace;
    m_pLrc = new Lrc(m_curLyricFileName.toLocal8Bit().data());
    logonlytrace;
    LyricParser::ParserStates ps = m_pLrc->lyricFromFile();
    logonlytrace;
     m_iCurIndex = 0;
    if(ps == LyricParser::PS_MEET_UNKNOW || ps == LyricParser::PS_OK)
    {
        this->m_curLyricIsOK = true;

        m_totalTime = totalTime;
        logonlytrace;
        emit startNewLyric(strMusicShowTitle, true);
        logtrace(" emit startNewLyric ps = %d",ps);
        return true;
    }
    else
    {logonlytrace;
        this->m_curLyricIsOK = false;
        m_iCurIndex = -1;
        m_totalTime = 0;
        logwarning("return is not ok or meet unknow:open file(%s:%s:%s) and ret code(%d)", m_curLyricFileName.toStdString().c_str(),m_curLyricFileName.toLocal8Bit().data(),m_curLyricFileName.toUtf8().data(), ps);
        emit startNewLyric(strMusicShowTitle, false);
        return false;
    }
    logonlytrace;
}

LyricNode* LyricManager::curNode()
{
    if(-1 < m_iCurIndex && m_iCurIndex < this->m_pLrc->nodes()->size())
    {
        return (*(m_pLrc->nodes()))[m_iCurIndex];
    }
    else
    {
        return 0;
    }
}

void LyricManager::setCurIndex(int iWhere/* = 0*/)
{
    this->m_iCurIndex = iWhere;
}

bool LyricManager::hasNext()
{
    return nextIndex() != -1;
}

LyricNode* LyricManager::nextNode()
{
    if(!hasNext())
    {
        return 0;
    }
    return (*(m_pLrc->nodes()))[nextIndex()];
}

int LyricManager::curIndex()
{
    if(-1 < m_iCurIndex && m_iCurIndex < this->m_pLrc->nodes()->size())
    {
        return m_iCurIndex;
    }
    else
    {
        return -1;
    }
}
int LyricManager::nextIndex()
{
    if(-1 < m_iCurIndex+1 && m_iCurIndex+1 < this->m_pLrc->nodes()->size())
    {
        return m_iCurIndex+1;
    }
    else
    {
        return -1;
    }
}

int LyricManager::totalCount()
{
    return this->m_pLrc->nodes()->size();
}

int LyricManager::findIndexByTime(long time, int indexFrom/* = 0*/)
{
    LyricParser::LyricNodes* lNodes = this->m_pLrc->nodes();
    if(indexFrom >= lNodes->size()-1 || indexFrom < 0)
    {
        return -1;
    }

    for(int i = indexFrom; i < lNodes->size()-1; i++)
    {
        logtrace("at(%d) lasttime(%ld) curTime(%ld) nextTime(%ld)",i, lNodes->at(i)->timeToMsecs(), time,lNodes->at(i+1)->timeToMsecs());
        if(lNodes->at(i)->timeToMsecs() <= time && time < lNodes->at(i+1)->timeToMsecs())
        {
            return i;
        }
    }
    return -1;
}

void LyricManager::scroll(long time, long totalTime)
{
    logtrace("this->m_curLyricIsOK = %d, time=%ld,totalTime=%ld", this->m_curLyricIsOK,time,totalTime);
    if(this->m_curLyricIsOK)
    {
        LyricNode* cur = this->curNode();
        if(!cur)
        {
            logwarning("no cur at %d, total is %d", this->curIndex(), this->totalCount());
            return;
        }

        LyricNode* next = this->nextNode();
        long endTime = this->m_totalTime;
        if(next)
        {
            endTime = next->timeToMsecs();
        }

        logtrace("m_iCurIndex(%d),cur->timeToMsecs(%ld) < time(%ld) < endTime(%ld)",
                 m_iCurIndex,cur->timeToMsecs(), time, endTime);
        if(cur->timeToMsecs() < time && time < endTime)
        {
            emit updateProcess(cur->timeToMsecs(),time, endTime);
            logonlytrace;
        }
        else if(time < cur->timeToMsecs() && (this->curIndex() == 0))
        {
            logtrace("current time is %ld, before play time %ld", time, cur->timeToMsecs());
        }
        else
        {
            if(next)
            {
                int start = 0;
                if(endTime < time)
                {
                    start = m_iCurIndex;
                }
                this->m_iCurIndex = findIndexByTime(time,start);
                if(m_iCurIndex == -1)
                {
                    stopLyric();
                    logtrace("at end...");
                }
                else
                {
                    emit this->playCurLyric();
                    this->curNode()->dump();

                    if(next->type == LyricNode::NT_STEP)
                    {
                        emit nextStep();
                    }
                    else if(next->type == LyricNode::NT_SENTENCE)
                    {
                        emit nextSentence();
                    }
                    else
                    {
                        logerror("no this type %d", next->type);
                    }
                }
            }
            else
            {
                logtrace("no next any more, time(%ld), endTime(%ld)!cur at %d, total is %d", time ,endTime, this->curIndex(), this->totalCount());
                this->stopLyric();
            }
        }
    }
}

void LyricManager::playLyric()
{
    emit this->playCurLyric();
}
void LyricManager::pauseLyric()
{
    emit this->pauseCurLyric();
}
void LyricManager::stopLyric()
{
    emit this->stopCurLyric();
}
