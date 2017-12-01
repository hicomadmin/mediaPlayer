#include "lyricloader.h"

LyricLoader::LyricLoader(const QString& strPath)
        : m_strPath(strPath),m_info(0), m_nodes(0)
{
    init();
}

LyricLoader::~LyricLoader()
{
    destroy();
}

void LyricLoader::destroy()
{
    if(m_info)
    {
        delete m_info;
        m_info = 0;
    }
    if(m_nodes)
    {
        qDeleteAll(m_nodes->begin(), m_nodes->end());
        //m_nodes->clear();
        delete m_nodes;
        m_nodes = 0;
    }
}

void LyricLoader::init()
{
    if(m_info)
    {
        emit meetError(LL_FOUND_A_NOT_NULL_POINTER, "m_info need to be a null pointer when invoke init!");
    }
    m_info = new LyricInfo;

    if(m_nodes)
    {
        emit meetError(LL_FOUND_A_NOT_NULL_POINTER, "m_nodes need to be a null pointer when invoke init!");
    }
    m_nodes = new QVector<LyricNode*>();  
}

QString LyricLoader::lyricPath()
{
    return this->m_strPath;
}

void LyricLoader::setLyricPath(const QString & strPath)
{
    this->m_strPath = strPath;
}

bool LyricLoader::reloadLyric()
{
    destroy();
    init();
    loadFromFile();
}

LyricInfo* LyricLoader::info()
{
    return this->m_info;
}

QVector<LyricNode*>* LyricLoader::nodes()
{
    return this->m_nodes;
}

void LyricLoader::addNode(LyricNode* pNewNode)
{
    this->m_nodes->push_back(pNewNode);
}
