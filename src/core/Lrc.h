#ifndef _LYRICLITE_LRC_H_
#define _LYRICLITE_LRC_H_

#include "LyricParser.h"

class Lrc : public LyricParser
{
public:
    Lrc();
    Lrc(const string& strPath);
    ParserStates lyricFromString(const string& strConent);
protected:

private:

};

#endif //_LYRICLITE_LRC_H_
