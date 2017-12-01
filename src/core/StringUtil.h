#ifndef _LYRICLITE_STRINGUTIL_H_
#define _LYRICLITE_STRINGUTIL_H_

#include <string>
using std::string;

class StringUtil
{
public:
	static bool isSpace(char ch);
	static int trimRightSpace(string & ret_strNeedTrim);

protected:
private:
};

#endif //_LYRICLITE_STRINGUTIL_H_
