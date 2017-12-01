#include <fstream>
#include <algorithm>

#include "LyricParser.h"


static bool cmpLyricNode(const LyricNode* node1, const LyricNode* node2)
{
	return node1->timeToMsecs() < node2->timeToMsecs();
}

LyricParser::LyricParser()
:m_info(0),m_nodes(0)
{
	init();
}

LyricParser::LyricParser(const string& strPath)
:m_strPath(strPath),m_info(0),m_nodes(0)
{
	init();
}

LyricParser::~LyricParser()
{
	unInit();
}

LyricParser::ParserStates LyricParser::lyricFromFile(const string& strPath)
{
	string strFilePath = strPath;
	if (strFilePath.empty())
	{
		strFilePath = lyricPath();
	}

	//ifstream iStream(strFilePath.c_str(), ios::in, _SH_DENYRW);strFilePath.c_str()C:/a.txt
	std::fstream iStream;
	iStream.open(strFilePath.c_str(),std::ios::in);//,_SH_DENYNO); 

	if (!iStream.is_open())
	{		
		logwarning("can't open file %s", strFilePath.c_str());
		return PS_FILE_NOT_READABLE;
	}
	string strData((istreambuf_iterator<char>(iStream)), istreambuf_iterator<char>());
	logonlytrace;
	return this->lyricFromString(strData);
}

LyricParser::ParserStates LyricParser::reloadFromFile()
{
	unInit();
	init();
	return lyricFromFile(lyricPath());
}

string LyricParser::lyricPath()
{
	return this->m_strPath;
}

void LyricParser::setLyricPath(const string & strPath)
{
	this->m_strPath = strPath;
}

LyricInfo* LyricParser::info()
{
	return this->m_info;
}

LyricParser::LyricNodes* LyricParser::nodes()
{
	return this->m_nodes;
}

void LyricParser::addNode(LyricNode* pNewNode)
{
	this->m_nodes->push_back(pNewNode);
}

void LyricParser::sortNodesByTime()
{
	sort(m_nodes->begin(), nodes()->end(), cmpLyricNode);
}

void LyricParser::offsetNodes(int msec)
{
	LyricNodes::iterator it = m_nodes->begin();
	while (m_nodes->end() != it)
	{
		(*it)->applyMsecOffset(msec);
		it++;
	}
}

void LyricParser::unInit()
{
	if(m_info)
    {
		delete m_info;
		m_info = 0;
    }
	if (m_nodes)
	{
		LyricNodes::iterator it = m_nodes->begin();
		while (m_nodes->end() != it)
		{
			delete *it;			
			it++;
		}
		//m_nodes->clear();
		delete m_nodes;
		m_nodes = 0;
	}
}

void LyricParser::init()
{
	if(m_info)
    {
		logwarning("you forget to destroy m_info? may memory leak ?");       
    }
    m_info = new LyricInfo;
	
    if(m_nodes)
    {
        logwarning("you forget to destroy m_nodes? may memory leak ?"); 
    }
    m_nodes = new LyricNodes();  
}
