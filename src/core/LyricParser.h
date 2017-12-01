#ifndef _LYRICLITE_LYRICPARSER_H_
#define _LYRICLITE_LYRICPARSER_H_

#include <string>
#include <vector>
#include "Log.h"

using namespace std;

struct LyricInfo {
public:
    string artist;
    string title;
    string album;
    string author;
    int offset;
	void *ext_pointer;

public:
    LyricInfo(const string & strArtist = "",
              const string & strTitle = "",
              const string & strAlbum = "",
              const string & strAuthor = "",
              int nOffset = 0,
			  void* pExt = 0)
    {
        this->artist = strArtist;
        this->title = strTitle;
        this->album = strAlbum;
        this->author = strAuthor;
        this->offset = nOffset;
		this->ext_pointer = pExt;
    }

    void dump()
    {
        logoutput("-----------------");
        logoutput("artist = %s",artist.c_str());
        logoutput("title  = %s",title.c_str());
        logoutput("album  = %s",album.c_str());
        logoutput("author = %s",author.c_str());
        logoutput("offset = %s",offset);
        logoutput("-----------------");
    }
};

struct LyricNode {
public:
    enum NodeType {
        NT_STEP,
        NT_SENTENCE,
    };
    int hour;
    int min;
    int sec;
    int msec;
    NodeType type;
    string lrc;

public:
    LyricNode(int nMillSeconds = 0, int nSeconds = 0, int nMinutes = 0, int nHours = 0, const string & strLrc = "", NodeType eType = NT_SENTENCE)
    {
        this->msec = nMillSeconds;
        this->sec = nSeconds;
        this->min = nMinutes;
		this->hour = nHours;
        this->lrc = strLrc;
        this->type = eType;        
    }

	void setTimeFromMsecs(long msecs, int nOffset = 0)
	{
		msecs += nOffset;
		
		msec = msecs % 1000;
		sec = msecs / 1000;

		sec = sec % 60;
		min = sec / 60;

		min = min%60;
		hour = min/60;
	}
	
	long timeToMsecs() const
	{
		return ((hour*60+min)*60 + sec)*1000;
	}
	
	void applyMsecOffset(int nOffset)
	{
		setTimeFromMsecs(timeToMsecs(), nOffset);
	}

    void dump()
    {
        printf("\n-----------------\n");
        printf("hour = %d\n",hour);
        printf("min  = %d\n",min);
        printf("sec  = %d\n",sec);
        printf("msec = %d\n",msec);
        printf("type = %d\n",type);
        printf("lrc  = %s\n",lrc.c_str());
        printf("-----------------\n");
    }
};

class LyricParser
{
public:
	typedef vector<LyricNode*> LyricNodes;
	enum ParserStates
	{
		PS_OK,		
		PS_MEET_UNKNOW,
		PS_MEET_ERROR,
		PS_FILE_NOT_EXIST,
		PS_FILE_NOT_READABLE,
	};

public:
	LyricParser();
    LyricParser(const string& strPath);

    virtual ~LyricParser();
	
    virtual ParserStates lyricFromFile(const string& strPath = "");
	virtual ParserStates lyricFromString(const string& strConent) = 0;
    ParserStates reloadFromFile();

    string lyricPath();
    void setLyricPath(const string & strPath);

    LyricInfo* info();
    LyricNodes* nodes();

protected:
    void addNode(LyricNode* pNewNode);
    void sortNodesByTime();
	void offsetNodes(int msec);

	virtual void unInit();
    virtual void init();

private:
    string m_strPath;
    LyricInfo* m_info;
    LyricNodes* m_nodes;
};

#endif //_LYRICLITE_LYRICPARSER_H_
