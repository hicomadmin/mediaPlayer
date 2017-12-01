#ifndef _LYRICLITE_BUFFOPERATOR_H_
#define _LYRICLITE_BUFFOPERATOR_H_

#include <string>
#include <sstream>
#include <iostream>

#include "Log.h"
using std::string;
using std::stringstream;
using std::cout;

class BuffOperator
{
public:
	BuffOperator(const string & str);
	BuffOperator(const char* pCStr);
	BuffOperator(const char* pBuf, int nLen);
	~BuffOperator();
	
	int tripNextSpace(bool changeCur = false);

	bool isValid();
	char* getPointStart();
	char* getPointCur();
	bool setCurPoint(int nWhere);
	void setNewBuffer(const char* pBuf, int nLen);

	int getBufLen();

	int curIndex();
	char curCh();
	bool indexValid(int iIndex);
	char chAt(int iIndex);

	bool hasNext(bool ignoreSpace = false);		
	bool hasPrev();

	template<class T> T next(bool ignoreSpace = false);
	char nextCh(bool ignoreSpace = false);
	int nextInt(bool ignoreSpace = false);
	template<class T> T prev(bool ignoreSpace = false);
	
	template<class T> T nextNoStep(bool ignoreSpace = false);
	char nextNoStepCh(bool ignoreSpace = false);
	int nextNoStepInt(bool ignoreSpace = false);
	template<class T> T prevNoStep(bool ignoreSpace = false);

	bool back(int n = 1, bool ignoreSpace = false);
	bool backUntilCh(char ch, bool ignoreSpace = false, bool gotoBefore = false);
	bool forward(int n = 1);

	

	template<class T> bool nextTypeIs(bool ignoreSpace = false);
	bool nextTypeIsCh(bool ignoreSpace = false);
	bool nextTypeIsInt(bool ignoreSpace = false);
	template<class T> bool prevTypeIs(bool ignoreSpace = false);

	template<class T> bool nextIs(const T & what,bool ignoreSpace = false);

	int nextIsInt(int val, bool ignoreSpace = false);
	bool nextIsCh(char chWhat, bool ignoreSpace = false, bool noCase = false);
	bool curIs(char ch, bool noCase = false);
	template<class T> bool prevIs(const T & what,bool ignoreSpace = false, bool noCase = false);
	
	bool nextIsDigit(bool ignoreSpace = false);
	bool curIsDigit();
	bool prevIsDigit(bool ignoreSpace = false);

	bool nextIsAlpha(bool ignoreSpace = false);
	bool curIsAlpha();
	bool prevIsAlpha(bool ignoreSpace = false);	

	bool nextIsSpace();
    bool thisLineNoTextAnyMore(bool ignoreWhiteSpace = false, bool ignoreTab = false);
    string toNextLine();

	bool curIsSpace();
	bool prevIsSpace();	
	
	bool nextStrIs(const string& str, bool ignoreSpace = false, bool noCase = false, bool ifTrueAddCur = false);

	bool nextChExistInStr(const string& str, bool noCase = false);
	bool prevChExistInStr(const string& str, bool noCase = false);		

	template<class T> string nextStrTo(const T & to, bool ignoreSpace = false, int ignoreFirstN = 0);
	string nextStrToCh(char to, bool ignoreSpace = false, int ignoreFirstN = 0);
	template<class T> bool ignoreTo(const T & to);
	bool ignoreToCh(char to);
	int ignoreChUntilAppearInStr(const string & str, bool ignoreSpace = false, bool noCase = false, bool contFindUntilNoAppear = false, bool addOffset2Cur = false);
	template<class T> bool ignoreNext(bool ignoreSpace = false);
	bool ignoreNextCh(bool ignoreSpace = false);

	template<class T> bool ignoreIfNextIs(const T & what,bool ignoreSpace = false);
private:
	void init(const char* pBuf, int nLen);
	void unInit();
private:
	char* m_pBuf;
	int m_nLen;
	int m_nCur;
};


template<class T> T BuffOperator::next(bool ignoreSpace/* = false*/)
{
	T val;	
	if (ignoreSpace)
	{
		tripNextSpace(true);		
	}
	stringstream ss(m_pBuf+m_nCur);
	ss.unsetf(std::ios::skipws);

	ss >> val;
	if (!ss.fail())
	{
		stringstream ssConv;
		ssConv.unsetf(std::ios::skipws);
		ssConv << val;
		m_nCur += ssConv.str().size();
	}
	else
	{
		logwarning("when get next fail, check your type!");
	}

	return val;
}


template<class T> T BuffOperator::prev(bool ignoreSpace/* = false*/)
{
	T val=0;
	return val;
}


template<class T> T BuffOperator::nextNoStep(bool ignoreSpace/* = false*/)
{
	T val;	
	stringstream ss(m_pBuf+m_nCur);
	if (!ignoreSpace)
	{
		ss.unsetf(std::ios::skipws);		
	}
	ss >> val;
	if (ss.fail())
	{
		logwarning("when nextNoStep fail, check your type!");
	}
	return val;
}

template<class T> T BuffOperator::prevNoStep(bool ignoreSpace/* = false*/)
{
	T val=0;
	return val;
}

template<class T> bool BuffOperator::nextTypeIs(bool ignoreSpace/* = false*/)
{
	T val;
	stringstream ss(m_pBuf+m_nCur);
	if (!ignoreSpace)
	{
		ss.unsetf(std::ios::skipws);
	}
	
	ss >> val;
	if (ss.fail())
	{
		return false;
	}
	return true;
}

template<class T> bool BuffOperator::prevTypeIs(bool ignoreSpace/* = false*/)
{
	
	return false;
}

template<class T> bool BuffOperator::nextIs(const T &what,bool ignoreSpace/* = false*/)
{
	if (!nextTypeIs<T>(ignoreSpace))
	{
		return false;
	}
	T val = nextNoStep<T>(ignoreSpace);

	return what == val;
}
/*
template<> char BuffOperator::nextIsCh(char val,bool ignoreSpace)
{
	int iFrom = 0;
	if (ignoreSpace)
	{
		iFrom += tripNextSpace(false);
	}

	if (indexValid(m_nCur+iFrom) && (chAt(m_nCur+iFrom) == val))
	{
		return true;
	}

	return false;
}
*/

template<class T> bool BuffOperator::prevIs(const T & what,bool ignoreSpace/* = false*/,bool noCase/* = false*/)
{

	return false;
}


template<class T> string BuffOperator::nextStrTo(const T & to, bool ignoreSpace/* = false*/, int ignoreFirstN/* = 0*/)
{
	if (ignoreSpace)
	{
		tripNextSpace(true);
	}

	stringstream ssTo;
	ssTo.unsetf(std::ios::skipws);	
redo:
	while (hasNext() && !nextIs(to))
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

template<class T> bool BuffOperator::ignoreTo(const T & to)
{
	while (hasNext() && !nextIs(to))
	{
		next<char>();
	}

	if (nextIs(to))
	{
		return true;
	}
	
	return false;
}

template<class T> bool BuffOperator::ignoreNext(bool ignoreSpace/* = false*/)
{
	if (!nextTypeIs<T>(ignoreSpace))
	{
		return false;
	}
	next<T>(ignoreSpace);
	return true;
}


template<class T> bool BuffOperator::ignoreIfNextIs(const T & what,bool ignoreSpace/* = false*/)
{
	if (!nextTypeIs<T>(ignoreSpace))
	{
		return false;
	}
	T val = next<T>(ignoreSpace);
	return what == val;
}



#endif //_LYRICLITE_BUFFOPERATOR_H_
