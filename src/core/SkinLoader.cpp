#include <QVector>

#include "SkinLoader.h"
#include "Log.h"

SkinLoader::SkinLoader(const QString& strFilePath, QObject *parent/* = 0*/)
    : QObject(parent),m_skinFilePath(strFilePath), m_componentAttrs(ENUM_CELL,0)
{
    //logtrace("total vector size is (%d), and enum size is (%d)", this->m_componentAttrs.size(), this->ENUM_CELL);
    m_skinAuthor = new SkinAuthor();
}

SkinLoader::~SkinLoader()
{
    destroy();
}

void SkinLoader::destroy()
{
    if(this->m_skinAuthor)
    {
        delete this->m_skinAuthor;
        this->m_skinAuthor = 0;
    }

    QVector<ComponentAttr*>::iterator it = this->m_componentAttrs.begin();
    while(it != this->m_componentAttrs.end())
    {
        if(*it)
        {
            delete *it;
            *it = 0;
        }
        it++;
    }
}

bool SkinLoader::hasAttr(ComponentID id)
{
    return 0 != this->m_componentAttrs[id];
}

ComponentAttr* SkinLoader::getAttr(ComponentID id)
{
    return this->m_componentAttrs[id];
}

bool SkinLoader::setAttr(ComponentID id, ComponentAttr* pNewAttr)
{
    if(hasAttr(id))
    {
        return false;
    }
    this->m_componentAttrs[id] = pNewAttr;
    return true;
}

QString SkinLoader::skinFilePath()
{
    return this->m_skinFilePath;
}

void SkinLoader::setSkinFilePath(const QString & strFilePath)
{
    this->m_skinFilePath = strFilePath;
}

QString SkinLoader::skinName()
{
    return this->m_skinName;
}

SkinAuthor* SkinLoader::skinAuthor()
{
    return this->m_skinAuthor;
}

void SkinLoader::setSkinName(const QString &strSkinName)
{
    this->m_skinName = strSkinName;
}

bool SkinLoader::reload()
{
    //logtrace("here");
    destroy();
    //logtrace("here");
    m_skinAuthor = new SkinAuthor();
    return loadSkinFromFile();
}


