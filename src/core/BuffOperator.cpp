#include <cstring>

#include "BuffOperator.h"

static bool isspace(char ch)
{
	if (' ' == ch || '\r' == ch || '\n' == ch || '\t' == ch || '\v' == ch || '\f' == ch)
	{
		return true;
	}
	return false;
}

//static bool 

BuffOperator::BuffOperator(const string & str)
{
	init(str.c_str(), str.size());
}

BuffOperator::BuffOperator(const char* pCStr)
{
	init(pCStr, strlen(pCStr));
}

BuffOperator::BuffOperator(const char* pBuf, int nLen)
{
	init(pBuf, nLen);
}

BuffOperator::~BuffOperator()
{
	unInit();
}

int BuffOperator::tripNextSpace(bool changeCur/* = false*/)
{
	int iFrom = 0;
	while (indexValid(m_nCur+iFrom) && isspace(chAt(m_nCur+iFrom)))
	{
		iFrom++;
	}
	if (changeCur)
	{
		m_nCur += iFrom;
	}	
	return iFrom;
}

bool BuffOperator::isValid()
{
	return m_nLen > 0 && m_nCur >= 0 && m_nCur < m_nLen;
}

char* BuffOperator::getPointStart()
{
	return m_pBuf;
}

char* BuffOperator::getPointCur()
{
	return (m_pBuf+m_nCur);
}

bool BuffOperator::setCurPoint(int nWhere)
{
	if (0 <= nWhere && nWhere < m_nLen)
	{
		m_nCur = nWhere;
		return true;
	}
	return false;
}

void BuffOperator::setNewBuffer(const char* pBuf, int nLen)
{
	unInit();
	init(pBuf, nLen);
}

int BuffOperator::getBufLen()
{
	return m_nLen;
}

int BuffOperator::curIndex()
{
	return m_nCur;
}

char BuffOperator::curCh()
{
	return m_pBuf[m_nCur];
}

bool BuffOperator::indexValid(int iIndex)
{
	return 0 <= iIndex && iIndex < m_nLen;
}

char BuffOperator::chAt(int iIndex)
{
	//if (indexValid(iIndex))
	//{
		return m_pBuf[iIndex];
	//}
	//return 0;
}

bool BuffOperator::hasNext(bool ignoreSpace/* = false*/)
{	
	int iFrom = 1;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}
	
	return m_nCur+iFrom < m_nLen;
}

bool BuffOperator::hasPrev()
{
	return m_nCur-1 >= 0;
}

bool BuffOperator::back(int n/* = 1*/, bool ignoreSpace/* = false*/)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		while (indexValid(m_nCur-iFrom - 1) && isspace(chAt(m_nCur-iFrom - 1)))
		{
			iFrom++;
		}
	}
	n += iFrom;
	if (m_nCur -n >= 0)
	{
		m_nCur -= n;
		return true;
	}	
	return false;
}

bool BuffOperator::backUntilCh(char ch, bool ignoreSpace/* = false*/, bool gotoBefore/* = false*/)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		while (indexValid(m_nCur-iFrom - 1) && isspace(chAt(m_nCur-iFrom - 1)))
		{
			iFrom++;
		}
	}

	while (indexValid(m_nCur-iFrom - 1) && ch != (chAt(m_nCur-iFrom - 1)))
	{
		iFrom++;
	}

	if (ch == (chAt(m_nCur-iFrom - 1)))
	{
		m_nCur -= iFrom;
		m_nCur -= gotoBefore;
		return true;
	}
	
	return false;
}
bool BuffOperator::forward(int n/* = 1*/)
{
	if (m_nCur+n < m_nLen)
	{
		m_nCur += n;
		return true;
	}	
	return false;
}

bool BuffOperator::nextIsCh(char chWhat, bool ignoreSpace/*= false*/, bool noCase/* = false*/)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);	
	}

	if (indexValid(m_nCur + iFrom))
	{
		if (noCase)
		{
			return tolower(chWhat) == tolower(chAt(m_nCur+iFrom));
		}
		else
		{
			return (chWhat) == chAt(m_nCur+iFrom);
		}
	}
	return false;
}

bool BuffOperator::thisLineNoTextAnyMore(bool ignoreWhiteSpace/* = false*/, bool ignoreTab/* = false*/)
{
    int iFrom = 0;
    if(ignoreWhiteSpace || ignoreTab)
    {
        while(indexValid(m_nCur + iFrom))
        {
            
            if(ignoreWhiteSpace && chAt(m_nCur+iFrom) == ' ' || ignoreTab && chAt(m_nCur+iFrom) == '\t')
            {
                iFrom ++;
            }
            else
            {
                break;
            }
        }
    }
    if('\n' == chAt(m_nCur+iFrom) || '\r' == chAt(m_nCur+iFrom) )
    {
        return true;
    }
	return false;
}

string BuffOperator::toNextLine()
{
    int iFrom = 0;
    stringstream ss;
    while(indexValid(m_nCur + iFrom) && (chAt(m_nCur+iFrom) != '\n' || chAt(m_nCur+iFrom) != '\r'))
    {
        ss << chAt(m_nCur+iFrom);
        iFrom ++;
    }
    return ss.str();
}

bool BuffOperator::curIs(char ch,bool noCase/* = false*/)
{
	if (!isValid())
	{
		return false;
	}

	if (noCase)
	{
		return tolower(ch) == tolower(curCh());
	}
	else
	{
		return ch == curCh();
	}	
}

bool BuffOperator::nextIsDigit(bool ignoreSpace/* = false*/)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}
	
	if (!indexValid(m_nCur+iFrom))
	{
		return false;
	}
	char ch = chAt(m_nCur + iFrom);	
	return '0' <= ch && ch <= '9';
}

bool BuffOperator::curIsDigit()
{
	if (!isValid())
	{
		return false;
	}

	return '0' <= curCh() && curCh() <= '9';
}

bool BuffOperator::prevIsDigit(bool ignoreSpace/* = false*/)
{
	int iFrom = 1;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}

	if (!indexValid(m_nCur+iFrom))
	{
		return false;
	}
	char ch = chAt(m_nCur + iFrom);	
	return '0' <= ch && ch <= '9';
}

bool BuffOperator::nextIsAlpha(bool ignoreSpace/* = false*/)
{
	int iFrom = 1;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}

	if (!indexValid(m_nCur+iFrom))
	{
		return false;
	}
	char ch = chAt(m_nCur + iFrom);	

	return 0 != isalpha(ch);
}

bool BuffOperator::curIsAlpha()
{
	if (!isValid())
	{
		return false;
	}

	return 0 != isalpha(curCh());
}

bool BuffOperator::prevIsAlpha(bool ignoreSpace/* = false*/)
{
	if (!hasPrev())
	{
		return false;
	}
	return false;
//	return 0 != isalpha(prev<char>());
}

bool BuffOperator::nextIsSpace()
{
	if (!hasNext())
	{
		return false;
	}
	
	return isspace(nextNoStepCh());
}

bool BuffOperator::curIsSpace()
{
	if (!isValid())
	{
		return false;
	}

	return 0 != isspace(curCh());
}

bool BuffOperator::prevIsSpace()
{
	if (!hasPrev())
	{
		return false;
	}
return false;
//	return 0 != isspace(prev<char>());
}

bool BuffOperator::nextStrIs(const string& str, bool ignoreSpace/* = false*/, bool noCase/* = false*/, bool ifTrueAddCur/* = false*/)
{
	int iFrom = 1;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);		
	}
	
	string::const_iterator cIt = str.begin();
	while(str.end() != cIt)
	{
		if (noCase)
		{
			if(!(indexValid(m_nCur+iFrom) && tolower(chAt(m_nCur+iFrom)) == tolower(*cIt)))
			{
				return false;
			}
		}
		else
		{
			if (!(indexValid(m_nCur+iFrom) && chAt(m_nCur+iFrom) == (*cIt)))
			{
				return false;
			}
		}
		iFrom++;
		cIt++;
	}

	if (ifTrueAddCur)
	{
		m_nCur += iFrom;
	}
	
	return true;
}

bool BuffOperator::nextChExistInStr(const string& str, bool noCase/*= true*/)
{
	if (!hasNext())
	{
		return false;
	}
	char ch = nextNoStepCh(false);
	string::const_iterator cIt = str.begin();
	while(str.end() != cIt)
	{
		if (noCase)
		{
			if(tolower(*cIt) == tolower(ch))
			{
				return true;
			}
		}
		else
		{
			if((*cIt) == ch)
			{
				return true;
			}
		}
		cIt++;
	}

	return false;
}

bool BuffOperator::prevChExistInStr(const string& str, bool noCase/*= true*/)
{
/*	if (!hasPrev())
	{
		return false;
	}
	char ch = prev<char>();
	string::const_iterator cIt = str.begin();
	while(str.end() != cIt)
	{
		if (noCase)
		{
			if(tolower(*cIt) == tolower(ch))
			{
				return true;
			}
		}
		else
		{
			if((*cIt) == ch)
			{
				return true;
			}
		}	

		cIt++;
	}*/

	return false;
}

int BuffOperator::ignoreChUntilAppearInStr(const string & str, bool ignoreSpace/* = false*/, bool noCase/* = false*/, bool contFindUntilNoAppear/* = false*/, bool addOffset2Cur/* = false*/)
{
	string strCmp = str;
	if (noCase)
	{
		unsigned int i = 0; 
		while (i < strCmp.size())
		{
			if ('A' <= strCmp[i] && strCmp[i] <= 'Z')
			{
				strCmp[i] = strCmp[i]-'A' + 'a';
			}			
		}
	}
	int iRet = 0;
	//stringstream ssTo;
	//ssTo.unsetf(std::ios::skipws);	

	while (hasNext() && (string::npos == strCmp.find(nextCh(false))))
	{
		iRet ++;
		//ssTo << curCh();
	}
	
	if (contFindUntilNoAppear)
	{
		iRet++;
		while (hasNext() && (string::npos != strCmp.find(nextCh(false))))
		{
			iRet ++;
			//ssTo << curCh();
		}
	}
	
	if (addOffset2Cur)
	{
		m_nCur += iRet;
	}
	
	//return ssTo.str();
	return iRet;
}

void BuffOperator::init(const char* pBuf, int nLen)
{
	this->m_nCur = 0;
	this->m_nLen = nLen;
	this->m_pBuf = new char[m_nLen+1];
	memcpy(this->m_pBuf, pBuf, nLen);
	this->m_pBuf[m_nLen] = '\0';
}

void BuffOperator::unInit()
{
	this->m_nCur = 0;
	this->m_nLen = 0;
	if (this->m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = 0;
	}
	else
	{
		logwarning("there must be something wrong! for the m_pBuf is null");
	}
}

char BuffOperator::nextCh(bool ignoreSpace)
{
	if (ignoreSpace)
	{
		tripNextSpace(true);		
	}

	char val;	
	val = m_pBuf[m_nCur];
	m_nCur += 1;
	return val;
}

int BuffOperator::nextInt(bool ignoreSpace)
{
	if (ignoreSpace)
	{
		tripNextSpace(true);		
	}

	int val;
	stringstream ssIn;
	int iFrom = 0;
	while (indexValid(m_nCur+iFrom) && isdigit(chAt(m_nCur+iFrom)))
	{
		ssIn << chAt(m_nCur+iFrom);
		iFrom++;
	}
	ssIn >> val;
	m_nCur += iFrom;
	return val;
}

char BuffOperator::nextNoStepCh(bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);		
	}

	char val;	
	val = m_pBuf[m_nCur+iFrom];

	return val;
}

int BuffOperator::nextNoStepInt(bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);		
	}

	int val;
	stringstream ssIn;

	while (indexValid(m_nCur+iFrom) && isdigit(chAt(m_nCur+iFrom)))
	{
		ssIn << chAt(m_nCur+iFrom);
		iFrom++;
	}
	ssIn >> val;
	return val;
}

bool BuffOperator::nextTypeIsCh(bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}
	if (indexValid(m_nCur+iFrom) && isalpha(chAt(m_nCur+iFrom)))
	{
		return true;
	}

	return false;
}

bool BuffOperator::nextTypeIsInt(bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}

	if (indexValid(m_nCur+iFrom) && isdigit(chAt(m_nCur+iFrom)))
	{
		return true;
	}

	return false;
}
int BuffOperator::nextIsInt(int val,bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);		
	}

	int iVal;
	stringstream ssIn;
	while (indexValid(m_nCur+iFrom) && isdigit(chAt(m_nCur+iFrom)))
	{
		ssIn << chAt(m_nCur+iFrom);
		iFrom++;
	}
	ssIn >> iVal;
	return val == iVal;
}

string BuffOperator::nextStrToCh(char to, bool ignoreSpace/* = false*/, int ignoreFirstN/* = 0*/)
{
	if (ignoreSpace)
	{
		tripNextSpace(true);
	}

	stringstream ssTo;
	ssTo.unsetf(std::ios::skipws);	

redo:
	while (hasNext() && !nextIsCh(to))
	{
		ssTo << nextCh();
	}
	while (ignoreFirstN)
	{
		ignoreFirstN--;
		ssTo << nextCh();
		goto redo;
	}
	return ssTo.str();
}

bool BuffOperator::ignoreNextCh(bool ignoreSpace/* = false*/)
{
	//if (!nextTypeIsCh(ignoreSpace))
	//{
	//	return false;
	//} 
	if (!hasNext())
	{
		return true;
	}
	nextCh(ignoreSpace);
	return true;
}

bool BuffOperator::ignoreToCh(char to)
{
	while (hasNext() && !nextIsCh(to))
	{
		nextCh();
	}

	if (nextIsCh(to))
	{
		return true;
	}

	return false;
}

