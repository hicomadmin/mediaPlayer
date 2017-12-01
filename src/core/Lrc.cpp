#include <sstream>
#include <cstring>

#include "Lrc.h"
#include "BuffOperator.h"
#include "StringUtil.h"

Lrc::Lrc()
:LyricParser()
{
	
}

Lrc::Lrc(const string& strPath)
:LyricParser(strPath)
{
	
}


LyricParser::ParserStates Lrc::lyricFromString(const string& strConent)
{
#define MEET_UNKNOW_NO_BACK(ignoreN) 	strThisTime = bo.nextStrTo('[', false, ignoreN);\
	StringUtil::trimRightSpace(strThisTime);\
    logtrace("meet unknow:%s", strThisTime.c_str());\
	vector<LyricNode*>::iterator it = vecLastBack.begin();\
	while (vecLastBack.end() != it)\
	{\
	(*it)->lrc += strThisTime;\
	it++;\
	}\
	nTimeTemp1 = nTimeTemp2 = nHour = nMin = nSec = nMsec = curCh = 0;\
	strThisTime = "";\
	psRet = PS_MEET_UNKNOW;\
	goto start_pars

#define MEET_UNKNOWN(backN, ignoreN) bo.back(backN,true);\
	MEET_UNKNOW_NO_BACK(ignoreN)

#define MEET_UNKNOWN_DFT MEET_UNKNOWN(1, 1)

#define MEET_UNKNOWN_BACKUNTIL(untilCh,ignoreN) bo.backUntilCh(untilCh,true,true);\
	MEET_UNKNOW_NO_BACK(ignoreN)

	ParserStates psRet = PS_OK;
	int nTimeTemp1;
    int nTimeTemp2;
    int nHour;
    int nMin;
    int nSec;
    int nMsec;
	char curCh;
	string strThisTime;
	vector<LyricNode*> vecTemp;
	vector<LyricNode*> vecLastBack;

	nTimeTemp1 = nTimeTemp2 = nHour = nMin = nSec = nMsec = curCh = 0;
	
	BuffOperator bo(strConent);
	while (bo.hasNext())
	{
start_pars:
        		
		if(bo.nextIsCh('[', true))
        {//"["
			logassert(bo.ignoreNextCh(true));
            if(bo.nextIsDigit(true))
            {//"[12"
                nTimeTemp1 = bo.nextInt(true);
				if (bo.nextIsCh(':', true))
				{//"[12:"
					bo.ignoreNextCh(true);
					if (bo.nextIsDigit(true))
					{//"[12:23"
						nTimeTemp2 = bo.nextInt(true);
						if (bo.nextIsCh(':',true))
						{//"[12:23:"
							bo.ignoreNextCh();
							nHour = nTimeTemp1;
							nMin = nTimeTemp2;
							if (bo.nextIsDigit())
							{//"[12:23:34"
								nTimeTemp2 = bo.nextInt();//得到sec
							}
							else
							{//"[12:23:**"
								MEET_UNKNOWN_BACKUNTIL('[', 1);
							}
						}
						else
						{//"[12:23**"
							nMin = nTimeTemp1;
						}
						nSec = nTimeTemp2;
						
						if (bo.nextIsCh('.',true))
						{//"[12:23:34.567" OR "[12:23.567"
							bo.ignoreNextCh(true);
							nMsec = bo.nextInt(true);
						}
						
						if (bo.nextIsCh(']', true))
						{//正常的时间tag标志结束

							bo.ignoreNextCh(true);

							vecTemp.push_back(new LyricNode(nMsec, nSec, nMin, nHour, strThisTime, LyricNode::NT_SENTENCE));

							//查看是否有多个是间标签的情况
                            if(bo.thisLineNoTextAnyMore(true, true))
                            {
                                bo.toNextLine();
                                strThisTime = "";
                                goto lab_set_this_line;
                            }
							else 
                            {
                                if (bo.nextIsCh('[', true))
                                {//连续标签的情况
                                    nTimeTemp1 = nTimeTemp2 = nHour = nMin = nSec = nMsec = 0;					
                            
                                    goto start_pars;//再次回去分析
                                }
                            }
			
//logtrace("cur at %d==>%c", bo.curIndex(), bo.curCh());				
							//所有的这一句的时间标签都完成了
							//:现在得到歌词
							strThisTime = bo.nextStrToCh('[');


							StringUtil::trimRightSpace(strThisTime); //maybe you need to trip the end of \n
			
                            lab_set_this_line:
							vecLastBack.clear(); //为保存最后一次的tag引用
							
							vector<LyricNode*>::iterator it = vecTemp.begin();
							while(vecTemp.end() != it)
							{
								(*it)->lrc = strThisTime;
								this->addNode(*it);
								vecLastBack.push_back(*it);
								it++;
							}
							vecTemp.clear();	
                            strThisTime = "";	
						}
						else
						{//错误的字符		
					
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}		
					}
					else
					{//"[12:**"
						MEET_UNKNOWN_BACKUNTIL('[', 1);
					}
				}
				else
				{//"[12*"
					MEET_UNKNOWN_BACKUNTIL('[', 1);
				}

			}
			else if (bo.nextChExistInStr("abot",true))
			{
				switch(tolower(bo.nextCh(true)))
				{
				case 'a':
					{
						if (bo.nextIsCh('l',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[al:专辑名]
							info()->album = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else if (bo.nextIsCh('r',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[ar:艺人名]
							info()->artist = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//不认识
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 't':
					{
						if (bo.nextIsCh('i',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[ti:曲名]
							info()->title = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//不认识
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 'b':
					{
						if (bo.nextIsCh('y',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[by:编者（指编辑LRC歌词的人）]
							info()->author = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//不认识
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 'o':
					{
						if (bo.nextStrIs("ffset", false, true, true) && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[offset:时间补偿值] 其单位是毫秒，正值表示整体提前，负值相反。这是用于总体调整显示快慢的。
							info()->offset = bo.nextInt(true);
							if (bo.nextIsCh(']', true))
							{//正常结束
								bo.ignoreNextCh();
							}
							else
							{//no ']' as end
								MEET_UNKNOWN_BACKUNTIL('[', 1);
							}
						}
						else
						{//不认识
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				default:
					{//不认识
						MEET_UNKNOWN_BACKUNTIL('[', 1);
						break;
					}
					
				}
			}			
			else if(bo.nextIsCh(':', true))
            {//行注释
                bo.ignoreChUntilAppearInStr("\n\r", false, false, true, true);
            }
			else
			{//"[*" , unknown, 将其加入上一次的结果
				MEET_UNKNOWN(1, 1);
			}

        }
		else
		{//"*", unknown, 将其加入上一次的结果
			MEET_UNKNOWN(1,0);
		 }		
	}
    
	this->sortNodesByTime();

	return psRet;
}
