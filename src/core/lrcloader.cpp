#include "lrcloader.h"

LrcLoader::LrcLoader(const QString& strPath)
    : LyricLoader(strPath)
{

}

bool LrcLoader::loadFromFile(const QString& strPath/* = ""*/)
{
    if(strPath.isEmpty())
    {
        strPath = this->lyricPath();
    }
}

QString LrcLoader::readText(const QString strPath, QTextCodec *codec)
{
    QString text;
    QFile file(strPath);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream floStream(&file);
        floStream.setCodec(codec);
        text = floStream.readAll();
    }
    return text;
}

void LrcLoader::parseFile()
{

#define HAS_NEXT (strSource.end() != (it+1))

#define GET_NEXT(ch) if(!HAS_NEXT)\
    {\
     nMsec = nSec = nMin = nHour = 0;\
                                   strThisTime = "";\
                                                 break;\
                                             }\
ch = *(++it);

#define TRY_NEXT(ch) if
#define GET_CUR curCh=*it
#define PUSH_BACK --it
#define GET_INT(to) while(HAS_NEXT)
                        if()

    QString strSource = LrcLoader::readText(this->m_strPath);
    QString strThisTime;
    QChar curCh;
    int nTimeTemp1;
    int nTimeTemp2;
    int nHour;
    int nMin;
    int nSec;
    int nMsec;


    QString::iterator it = strSource.begin();
    while(it != strSource.end())
    {
        GET_CUR;
        if('[' == curCh)
        {
            if(curCh.isDigit())
            {
                PUSH_BACK;
                floStream >> nTimeTemp1; //maybe hour, maybe min
                trace(nTimeTemp1);
                GET_NEXT(curCh);
                if(':' != curCh)
                {//时间tag却没有:, meet error,
                    IGNORE_TO(curCh,'[',true);
                }
                else
                {//maybe min, maybe sec
                    GET_NEXT(nTimeTemp2);
                    trace(nTimeTemp2);
                    GET_NEXT(curCh);
                    if(':' == curCh)
                    {//nTimeTemp1 is hour, nTimeTemp2 is min
                        nHour = nTimeTemp1;
                        nMin = nTimeTemp2;
                        GET_NEXT(nTimeTemp2);//得到sec
                        GET_NEXT(curCh); //查看下一个
                    }
                    else
                    {
                        nMin = nTimeTemp1;
                    }
                    nSec = nTimeTemp2;
                    trace(nSec);
                    if('.' == curCh)
                    {
                        GET_NEXT(nTimeTemp1);//得到毫秒
                        nMsec = nTimeTemp1;
                        GET_NEXT(curCh); //查看下一个
                    }
                    tracech(curCh);
                    if(']' != curCh)
                    {//时间标签结束了，但不是]， error
                        IGNORE_TO(curCh,'[',true);
                    }
                    else
                    {//时间标签结束正确结束, 得到文本
                        GET_NEXT(curCh);
                        while(curCh.isSpace())
                        {
                            GET_NEXT(curCh);
                        }
                        if('[' == curCh)
                        {//连续标签的情况
                            vecTemp.push_back(new LyricNode(nMsec, nSec, nMin, strThisTime, LyricNode::NT_SENT, nHour));
                            nMsec = nSec = nMin = nHour = 0;
                            floStream << curCh;
                            tracech(curCh);
                            goto start_pars;
                        }
                        else
                        {
                            vecTemp.push_back(new LyricNode(nMsec, nSec, nMin, strThisTime, LyricNode::NT_SENT, nHour));
                            tracech(curCh);
                        }
                        tracech(curCh);
                        ADD_STR_UP_TO('[', curCh);
                        trace(strThisTime.toLocal8Bit().data());

                        //加入时间标签
                        //LyricNode* curNode = new LyricNode(nMsec, nSec, nMin, strThisTime, LyricNode::NT_SENT, nHour);
                        //vec->push_back(curNode);
                        QVector<LyricNode*>::iterator it = vecTemp.begin();
                        while(vecTemp.end() != it)
                        {
                            (*it)->lrc = strThisTime;
                            vec->push_back(*it);
                            it++;
                        }
                        vecTemp.clear();
                        nMsec = nSec = nMin = nHour = 0;
                        strThisTime = "";
                        tracech(curCh);
                    }
                    tracech(curCh);
                }
                tracech(curCh);
            }
            else if(curCh.isLetter())
            {
                tracech(curCh);
                /*
                    [al:专辑名]
                    [ar:艺人名]
                    [by:编者（指编辑LRC歌词的人）]
                    [offset:时间补偿值] 其单位是毫秒，正值表示整体提前，负值相反。这是用于总体调整显示快慢的。
                    [ti:曲名]
                    */
                switch(curCh.toLower().digitValue())
                {
                        case 'a':
                    {
                        GET_NEXT(curCh); //查看下一个
                        switch(curCh.toLower().digitValue())
                        {
                                    case 'l':
                            {//[al:专辑名]
                                IF_IS_COMEN_ASSIGN(m_info->album, curCh);
                                break;
                            }
                                    case 'r':
                            {//[ar:艺人名]
                                IF_IS_COMEN_ASSIGN(m_info->artist, curCh);
                                break;
                            }
                                    default:
                            {
                                IGNORE_TO(curCh,'[',false);
                            }
                            break;
                        }
                        break;
                    }
                        case 't':
                    {//[ti:曲名]
                        GET_NEXT(curCh); //查看下一个
                        if('i' == curCh)
                        {//是title
                            IF_IS_COMEN_ASSIGN(m_info->title, curCh);
                            break;
                        }
                        else
                        {//不认识
                            IGNORE_TO(curCh,'[',false);
                        }
                        break;
                    }
                        case 'b':
                    {//[by:编者（指编辑LRC歌词的人）]
                        GET_NEXT(curCh); //查看下一个
                        if('y' == curCh)
                        {//是author
                            IF_IS_COMEN_ASSIGN(m_info->author, curCh);
                            break;
                        }
                        else
                        {//不认识
                            IGNORE_TO(curCh,'[',false);
                        }
                        break;
                    }
                        case 'o':
                            {
                                GET_NEXT(curCh); //查看下一个
                                JUST_EQUAL_PASS('f', curCh);
                                JUST_EQUAL_PASS('f', curCh);
                                JUST_EQUAL_PASS('s', curCh);
                                JUST_EQUAL_PASS('e', curCh);
                                JUST_EQUAL_PASS('t', curCh);
                                JUST_EQUAL_PASS(':', curCh);
                                tracech(curCh);
                                int mayOffset = 0;
                                GET_NEXT(mayOffset);
                                GET_NEXT(curCh); //查看下一个
                                if(']' == curCh)
                                {//ok
                                    m_info->offset = mayOffset;
                                }
                                else
                                {
                                    IGNORE_TO(curCh,'[',false);
                                }
                                break;
                            }
                        default:
                            {
                                IGNORE_TO(curCh,'[',false);
                                break;
                            }
                        }
                tracech(curCh);
            }
            else if(':' == curCh)
            {//注释
                tracech(curCh);
                JUST_EQUAL_PASS(']', curCh);
            }
            else
            {//unknown, 将其加入上一次的结果
                tracech(curCh);
                ADD_STR_UP_TO('[', curCh);
                if(vec->size())
                {
                    LyricNode* lastNode = (*vec)[vec->size()-1];
                    lastNode->lrc += strThisTime;
                }
                strThisTime = "";
            }
        }


        ++it;
    }
}


