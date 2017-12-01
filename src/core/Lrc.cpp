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
								nTimeTemp2 = bo.nextInt();//�õ�sec
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
						{//������ʱ��tag��־����

							bo.ignoreNextCh(true);

							vecTemp.push_back(new LyricNode(nMsec, nSec, nMin, nHour, strThisTime, LyricNode::NT_SENTENCE));

							//�鿴�Ƿ��ж���Ǽ��ǩ�����
                            if(bo.thisLineNoTextAnyMore(true, true))
                            {
                                bo.toNextLine();
                                strThisTime = "";
                                goto lab_set_this_line;
                            }
							else 
                            {
                                if (bo.nextIsCh('[', true))
                                {//������ǩ�����
                                    nTimeTemp1 = nTimeTemp2 = nHour = nMin = nSec = nMsec = 0;					
                            
                                    goto start_pars;//�ٴλ�ȥ����
                                }
                            }
			
//logtrace("cur at %d==>%c", bo.curIndex(), bo.curCh());				
							//���е���һ���ʱ���ǩ�������
							//:���ڵõ����
							strThisTime = bo.nextStrToCh('[');


							StringUtil::trimRightSpace(strThisTime); //maybe you need to trip the end of \n
			
                            lab_set_this_line:
							vecLastBack.clear(); //Ϊ�������һ�ε�tag����
							
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
						{//������ַ�		
					
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
						{//[al:ר����]
							info()->album = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else if (bo.nextIsCh('r',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[ar:������]
							info()->artist = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//����ʶ
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 't':
					{
						if (bo.nextIsCh('i',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[ti:����]
							info()->title = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//����ʶ
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 'b':
					{
						if (bo.nextIsCh('y',false,true) && bo.ignoreNextCh() && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[by:���ߣ�ָ�༭LRC��ʵ��ˣ�]
							info()->author = bo.nextStrTo(']', true);
							bo.ignoreNextCh();
						}
						else
						{//����ʶ
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				case 'o':
					{
						if (bo.nextStrIs("ffset", false, true, true) && bo.nextIsCh(':',true,true) && bo.ignoreNextCh())
						{//[offset:ʱ�䲹��ֵ] �䵥λ�Ǻ��룬��ֵ��ʾ������ǰ����ֵ�෴�������������������ʾ�����ġ�
							info()->offset = bo.nextInt(true);
							if (bo.nextIsCh(']', true))
							{//��������
								bo.ignoreNextCh();
							}
							else
							{//no ']' as end
								MEET_UNKNOWN_BACKUNTIL('[', 1);
							}
						}
						else
						{//����ʶ
							MEET_UNKNOWN_BACKUNTIL('[', 1);
						}
						break;
					}
				default:
					{//����ʶ
						MEET_UNKNOWN_BACKUNTIL('[', 1);
						break;
					}
					
				}
			}			
			else if(bo.nextIsCh(':', true))
            {//��ע��
                bo.ignoreChUntilAppearInStr("\n\r", false, false, true, true);
            }
			else
			{//"[*" , unknown, ���������һ�εĽ��
				MEET_UNKNOWN(1, 1);
			}

        }
		else
		{//"*", unknown, ���������һ�εĽ��
			MEET_UNKNOWN(1,0);
		 }		
	}
    
	this->sortNodesByTime();

	return psRet;
}
