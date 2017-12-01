#include "StringUtil.h"

bool StringUtil::isSpace(char ch)
{
	if (' ' == ch || '\r' == ch || '\n' == ch || '\t' == ch || '\v' == ch || '\f' == ch)
	{
		return true;
	}
	return false;
}

int StringUtil::trimRightSpace(string & ret_strNeedTrim)
{
    if(ret_strNeedTrim.empty())
    {
        return 0;
    }
    unsigned int nIndex = ret_strNeedTrim.length() - 1;

	while (nIndex >= 0)
	{
		if (isSpace(ret_strNeedTrim.at(nIndex)))
		{
			nIndex--;
		}
		else
		{
			break;
		}		
	}
	if (ret_strNeedTrim.length() - 1 != nIndex)
	{
		ret_strNeedTrim = ret_strNeedTrim.erase(nIndex+1);
	}
	
	return ret_strNeedTrim.length() - 1 - nIndex;
}
