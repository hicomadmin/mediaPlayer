#include <QFileInfo>
#include <QDir>

#include "config.h"
#include "SoundCore.h"
#include "playlistmodel.h"

static MusicTag::TagColumn g_sortBy = MusicTag::SHOW_TITLE;

static bool lessCompare(MusicTag* tag1,MusicTag* tag2)
{
    tag1->setSortBy(g_sortBy);
    return *tag1 < *tag2;
}

PlayListModel::PlayListModel(const QString &strName/* = "default"*/)
    : m_strName(strName),m_tags(new QList<MusicTag*>()),m_iCurrent(-1)
{

}

bool PlayListModel::insert(int iWhere,MusicTag * tag)
{
    if(iWhere < 0)
    {
        return false;
    }

    if(this->m_tags->size() <= iWhere)
    {
        this->m_tags->append(tag);
    }
    else
    {
        this->m_tags->insert(iWhere, tag);
    }
    emit listModelChanged();
    return true;
}

bool PlayListModel::remove(int iFrom, int iTo)//iFrom <= where <= iTo
{
    if(iFrom < 0 || iTo >= m_tags->size())
    {
        return false;
    }
    if(-1 == iTo)
    {
        delete m_tags->at(iFrom);
        this->m_tags->erase(m_tags->begin()+iFrom);
    }

    QList<MusicTag*>::iterator itFrom = m_tags->begin() + iFrom;
    QList<MusicTag*>::iterator itTo = m_tags->begin()+iTo + 1;
    while(itFrom != itTo)
    {
        delete *itFrom;        
        itFrom = m_tags->erase(itFrom);
    }
    //m_tags->erase(itFrom, itTo);
    emit listModelChanged();
    return true;
}

void PlayListModel::addFileToTags(const QString & str)
{
    MusicTag* tag = new MusicTag(str, true);
    this->m_tags->append(tag);
    if(m_tags->size()==1)
    {
     //   this->setCurrent(0);
        emit addFirst();
    }
}

void PlayListModel::addFile(const QString & strFilePath)
{
    addFileToTags(strFilePath);
    emit listModelAppend();
}

void PlayListModel::addFiles(const QStringList & filePaths)
{
    foreach(QString strFilePath, filePaths)
    {
        addFileToTags(strFilePath);
    }
    emit listModelAppend();
}

void PlayListModel::addDir(const QString & strDirPath, bool bRecursion/* = false*/)
{
    QFileInfo info(strDirPath);
    if(info.isDir())
    {
        QDir dir(strDirPath);
        QStringList filePaths;
        if(bRecursion)
        {
            filePaths = dir.entryList(Config::instance()->musicFiltersList(), QDir::AllEntries | QDir::Hidden);
        }
        else
        {
            filePaths = dir.entryList(Config::instance()->musicFiltersList(), QDir::Files | QDir::Hidden);
        }

        foreach(QString strPath, filePaths)
        {
            addDir(strPath, bRecursion);
        }
    }
    else
    {
       addFileToTags(strDirPath);
    }
    emit listModelAppend();
}

void PlayListModel::sort(MusicTag::TagColumn sortBy/* = MusicTag::SHOW_TITLE*/)
{
    g_sortBy = sortBy;
    qSort(m_tags->begin(), m_tags->end(), lessCompare);
    emit listModelChanged();
}

QString PlayListModel::name() const
{
    return this->m_strName;
}

void PlayListModel::setName(const QString &strName)
{
    this->m_strName = strName;
}

int PlayListModel::currentIndex()
{
    return this->m_iCurrent;
}

MusicTag* PlayListModel::currentTag()
{
    if(-1<m_iCurrent && m_iCurrent < this->m_tags->size())
    {
        return (*m_tags)[m_iCurrent];
    }
    return 0;
}

void PlayListModel::next()
{
    if(hasNext())
    {
        m_iCurrent += 1;
        //SoundCore::instance()->changeUrlTo(this->m_tags->at(m_iCurrent)->fileName());
        SoundCore::instance()->setCurUrl(this->m_tags->at(m_iCurrent)->fileName());
    }
    else
    {
        loginfo("reach the list end! no next m_iCurrent is %d, m_tags->size()=%d",m_iCurrent, m_tags->size());
        SoundCore::instance()->stop();
    }
}
void PlayListModel::prev()
{
    if(hasPrev())
    {
        m_iCurrent -= 1;
        SoundCore::instance()->changeUrlTo(this->m_tags->at(m_iCurrent)->fileName());
    }
    else
    {
        loginfo("reach the list start! no prev m_iCurrent is %d, m_tags->size()=%d",m_iCurrent, m_tags->size());
        SoundCore::instance()->stop();
    }
}

void PlayListModel::setCurrent(int iWhere)
{
    if(iWhere > -1 && iWhere < m_tags->size())
    {
        m_iCurrent = iWhere;
        SoundCore::instance()->changeUrlTo(this->m_tags->at(m_iCurrent)->fileName());
    }
    else
    {
        loginfo("set current to(%d) error, no m_tags->size() is (%d)", iWhere, m_tags->size());
        SoundCore::instance()->stop();
    }
}

QList<MusicTag*>* PlayListModel::tags()
{
    return this->m_tags;
}

bool PlayListModel::hasNext()
{
    return m_iCurrent+1 < m_tags->size();
}

bool PlayListModel::hasPrev()
{
    return m_iCurrent-1 > -1 && m_tags->size();
}


