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
                {//ʱ��tagȴû��:, meet error,
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
                        GET_NEXT(nTimeTemp2);//�õ�sec
                        GET_NEXT(curCh); //�鿴��һ��
                    }
                    else
                    {
                        nMin = nTimeTemp1;
                    }
                    nSec = nTimeTemp2;
                    trace(nSec);
                    if('.' == curCh)
                    {
                        GET_NEXT(nTimeTemp1);//�õ�����
                        nMsec = nTimeTemp1;
                        GET_NEXT(curCh); //�鿴��һ��
                    }
                    tracech(curCh);
                    if(']' != curCh)
                    {//ʱ���ǩ�����ˣ�������]�� error
                        IGNORE_TO(curCh,'[',true);
                    }
                    else
                    {//ʱ���ǩ������ȷ����, �õ��ı�
                        GET_NEXT(curCh);
                        while(curCh.isSpace())
                        {
                            GET_NEXT(curCh);
                        }
                        if('[' == curCh)
                        {//������ǩ�����
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

                        //����ʱ���ǩ
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
                    [al:ר����]
                    [ar:������]
                    [by:���ߣ�ָ�༭LRC��ʵ��ˣ�]
                    [offset:ʱ�䲹��ֵ] �䵥λ�Ǻ��룬��ֵ��ʾ������ǰ����ֵ�෴�������������������ʾ�����ġ�
                    [ti:����]
                    */
                switch(curCh.toLower().digitValue())
                {
                        case 'a':
                    {
                        GET_NEXT(curCh); //�鿴��һ��
                        switch(curCh.toLower().digitValue())
                        {
                                    case 'l':
                            {//[al:ר����]
                                IF_IS_COMEN_ASSIGN(m_info->album, curCh);
                                break;
                            }
                                    case 'r':
                            {//[ar:������]
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
                    {//[ti:����]
                        GET_NEXT(curCh); //�鿴��һ��
                        if('i' == curCh)
                        {//��title
                            IF_IS_COMEN_ASSIGN(m_info->title, curCh);
                            break;
                        }
                        else
                        {//����ʶ
                            IGNORE_TO(curCh,'[',false);
                        }
                        break;
                    }
                        case 'b':
                    {//[by:���ߣ�ָ�༭LRC��ʵ��ˣ�]
                        GET_NEXT(curCh); //�鿴��һ��
                        if('y' == curCh)
                        {//��author
                            IF_IS_COMEN_ASSIGN(m_info->author, curCh);
                            break;
                        }
                        else
                        {//����ʶ
                            IGNORE_TO(curCh,'[',false);
                        }
                        break;
                    }
                        case 'o':
                            {
                                GET_NEXT(curCh); //�鿴��һ��
                                JUST_EQUAL_PASS('f', curCh);
                                JUST_EQUAL_PASS('f', curCh);
                                JUST_EQUAL_PASS('s', curCh);
                                JUST_EQUAL_PASS('e', curCh);
                                JUST_EQUAL_PASS('t', curCh);
                                JUST_EQUAL_PASS(':', curCh);
                                tracech(curCh);
                                int mayOffset = 0;
                                GET_NEXT(mayOffset);
                                GET_NEXT(curCh); //�鿴��һ��
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
            {//ע��
                tracech(curCh);
                JUST_EQUAL_PASS(']', curCh);
            }
            else
            {//unknown, ���������һ�εĽ��
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


