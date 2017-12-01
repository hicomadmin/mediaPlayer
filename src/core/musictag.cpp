#include <QFileInfo>
#include <fileref.h>
#include <tag.h>
#include <config.h>
#include "musictag.h"


MusicTag::MusicTag()
{
    this->m_uiFileSize = this->m_uiTrack = this->m_uiYear = this->m_uiLength = 0;
    this->m_bitrate = this->m_sampleRate = 0;
    this->m_bInited = false;
    this->m_tagRef = 0;
}

MusicTag::MusicTag(const QString & strFileName, bool readNow /*=false*/)
    :m_strFileName(strFileName)
{
    this->m_tagRef = 0;
    this->m_uiFileSize = this->m_uiTrack = this->m_uiYear = this->m_uiLength = 0;
    this->m_bitrate = this->m_sampleRate = 0;
    this->m_bInited = false;
    if(readNow)
    {
        readTag();
    }
}

MusicTag::MusicTag(const MusicTag & copy)
{
    m_strFileName = copy.fileName();
    m_strShowTitle = copy.showTitle();
    m_strTitle = copy.title();
    m_strArtist = copy.artist();
    m_strAlbum = copy.album();
    m_strComment = copy.comment();
    m_strGenre = copy.genre();
    m_strHash = copy.hash();
    m_strLastPlayTime = copy.lastPlayTime();

    m_bitrate = copy.bitrate();
    m_sampleRate = copy.sampleRate();

    m_uiLength = copy.length();
    m_uiFileSize = copy.fileSize();
    m_uiYear = copy.year();
    m_uiTrack = copy.track();

    m_bInited = copy.m_bInited;
    m_sortBy = copy.sortBy();

    m_tagRef = copy.m_tagRef;
}

MusicTag& MusicTag::operator =(const MusicTag & copy)
{
    m_strFileName = copy.fileName();
    m_strShowTitle = copy.showTitle();
    m_strTitle = copy.title();
    m_strArtist = copy.artist();
    m_strAlbum = copy.album();
    m_strComment = copy.comment();
    m_strGenre = copy.genre();
    m_strHash = copy.hash();
    m_strLastPlayTime = copy.lastPlayTime();

    m_bitrate = copy.bitrate();
    m_sampleRate = copy.sampleRate();

    m_uiLength = copy.length();
    m_uiFileSize = copy.fileSize();
    m_uiYear = copy.year();
    m_uiTrack = copy.track();

    m_bInited = copy.m_bInited;
    m_sortBy = copy.sortBy();

    m_tagRef = copy.m_tagRef;
    return *this;
}

MusicTag::~MusicTag()
{
    delete this->m_tagRef;
}

bool MusicTag::readTag()
{
    if(!init())
    {
        return false;
    }

    TagLib::Tag *tag = m_tagRef->tag();
    if(tag) {
        
        this->m_strTitle = QString::fromLocal8Bit(tag->title().toCString());
logtrace("m_strTitle=%s", this->m_strTitle.toUtf8().data());
        this->m_strArtist = QString::fromLocal8Bit(tag->artist().toCString());
        this->m_strAlbum = QString::fromLocal8Bit(tag->album().toCString());
        this->m_strComment = QString::fromLocal8Bit(tag->comment().toCString());
        this->m_strGenre = QString::fromLocal8Bit(tag->genre().toCString());
    logtrace("m_strArtist=%s", this->m_strArtist.toUtf8().data());
    if(this->m_strTitle.isEmpty() || this->m_strArtist.isEmpty())
    {
        QFileInfo info(this->m_strFileName);
        this->m_strShowTitle = info.fileName();
    }
    else
    {
        this->m_strShowTitle = formatShowTitle(Config::instance()->showTitleFormat());
    }
logtrace("m_strShowTitle=%s", this->m_strShowTitle.toUtf8().data());

logtrace("m_strAlbum=%s", this->m_strAlbum.toUtf8().data());
        this->m_uiYear = tag->year();
        this->m_uiTrack = tag->track();
    }

    TagLib::AudioProperties *properties = m_tagRef->audioProperties();
    if(properties)
    {
        this->m_uiLength = properties->length();
        logtrace("m_uiLength=%d", this->m_uiLength);
        this->m_bitrate = properties->bitrate();
        this->m_sampleRate = properties->sampleRate();
    }
            logtrace("out");
    return true;
}

bool MusicTag::writeTag()
{
    if(!init())
    {
        return false;
    }
    TagLib::Tag *tag = m_tagRef->tag();
    if(tag) {
        tag->setTitle(this->m_strTitle.toLocal8Bit().data());
        tag->setArtist(this->m_strArtist.toLocal8Bit().data());
        tag->setAlbum(this->m_strAlbum.toLocal8Bit().data());
        tag->setComment(this->m_strComment.toLocal8Bit().data());
        tag->setGenre(this->m_strGenre.toLocal8Bit().data());
        tag->setYear(this->m_uiYear);
        tag->setTrack(this->m_uiTrack);
    }
    return true;
}

QString MusicTag::fileName() const
{
    return this->m_strFileName;
}

QString MusicTag::title() const
{
    return this->m_strTitle;
}

QString MusicTag::artist() const
{
    return this->m_strArtist;
}

QString MusicTag::album() const
{
    return this->m_strAlbum;
}

QString MusicTag::comment() const
{
    return this->m_strComment;
}

QString MusicTag::genre() const
{
    return this->m_strGenre;
}

QString MusicTag::hash() const
{
    return this->m_strHash;
}

QString MusicTag::lastPlayTime() const
{
    return m_strLastPlayTime;
}

QString MusicTag::showTitle() const
{
    return this->m_strShowTitle;
}

QString MusicTag::lengthStr() const
{
    int seconds = this->m_uiLength%60;
    int mintues = (this->m_uiLength/60)%60;
    int hours = this->m_uiLength/3600;
    QString str;
    if(hours)
    {
        str = QString("%1:").arg(hours);
    }

    str += QString("%1:%2").arg(mintues).arg(seconds);
    return str;
}

int MusicTag::bitrate() const
{
    return this->m_bitrate;
}

int MusicTag::sampleRate() const
{
    return this->m_sampleRate;
}

unsigned int MusicTag::length() const
{
    return this->m_uiLength;
}

unsigned int MusicTag::year() const
{
    return this->m_uiYear;
}

unsigned int MusicTag::track() const
{
    return this->m_uiTrack;
}

unsigned int MusicTag::fileSize() const
{
    return this->m_uiFileSize;
}

MusicTag::TagColumn MusicTag::sortBy() const
{
    return this->m_sortBy;
}

bool MusicTag::setFileName(const QString &s, bool readNow/* = false*/)
{
    this->m_strFileName = s;
    if(readNow)
    {
        return this->readTag();
    }
    return true;
}

void MusicTag::setShowTitleFormat(const QString & strFormat, bool updateNow/* = false*/)
{
    this->m_strFormat = strFormat;
    if(updateNow)
    {
        this->m_strShowTitle = formatShowTitle(m_strFormat.toStdString().c_str());
    }
}

void MusicTag::setShowTitle(const QString & strShowTitle)
{
    this->m_strShowTitle = strShowTitle;
}

bool MusicTag::setTitle(const QString &s, bool writeNow/* = false*/)
{
    this->m_strTitle = s;
    if(writeNow)
    {
        this->m_tagRef->tag()->setTitle(this->m_strTitle.toLocal8Bit().data());
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setArtist(const QString &s, bool writeNow/* = false*/)
{
    this->m_strArtist = s;
    if(writeNow)
    {
        this->m_tagRef->tag()->setArtist(this->m_strArtist.toLocal8Bit().data());
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setAlbum(const QString &s, bool writeNow/* = false*/)
{
    this->m_strAlbum = s;
    if(writeNow)
    {
        this->m_tagRef->tag()->setAlbum(this->m_strAlbum.toLocal8Bit().data());
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setComment(const QString &s, bool writeNow/* = false*/)
{
    this->m_strComment = s;
    if(writeNow)
    {
        this->m_tagRef->tag()->setComment(this->m_strComment.toLocal8Bit().data());
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setGenre(const QString &s, bool writeNow/* = false*/)
{
    this->m_strGenre = s;
    if(writeNow)
    {
        this->m_tagRef->tag()->setGenre(this->m_strGenre.toLocal8Bit().data());
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setYear(unsigned int i, bool writeNow/* = false*/)
{
    this->m_uiYear = i;
    if(writeNow)
    {
        this->m_tagRef->tag()->setYear(this->m_uiYear);
        return this->m_tagRef->save();
    }
    return true;
}

bool MusicTag::setTrack(unsigned int i, bool writeNow/* = false*/)
{
    this->m_uiTrack = i;
    if(writeNow)
    {
        this->m_tagRef->tag()->setTrack(this->m_uiTrack);
        return this->m_tagRef->save();
    }
    return true;
}

void MusicTag::setHash(const QString &s)
{
    this->m_strHash = s;
}

void MusicTag::setLastPlayTime(const QString &s)
{
    this->m_strLastPlayTime = s;
}

void MusicTag::setFileSize(unsigned int uiFileSize)
{
    this->m_uiFileSize = uiFileSize;
}

void MusicTag::setSortBy(MusicTag::TagColumn sortBy)
{
    this->m_sortBy = sortBy;
}

bool MusicTag::testHashEq()
{
    return MusicTag::computeHash(this->m_strFileName) == this->m_strHash;
}

bool MusicTag::testFileSizeEq()
{
    return MusicTag::fileSize(this->m_strFileName) == this->m_uiFileSize;
}

bool MusicTag::testIsRightFile()
{
    return testFileSizeEq() && testHashEq();
}

bool MusicTag::fileExists()
{
    if(this->m_strFileName.isEmpty())
    {
        return false;
    }
    QFileInfo info(this->m_strFileName);
    return info.exists();
}

QString MusicTag::columnStr(MusicTag::TagColumn tagColumn) const
{
    switch(tagColumn)
    {
        case FILE_NAME:
            return this->m_strFileName;
        case TITLE:
            return this->m_strTitle;
        case ARTIST:
            return this->m_strArtist;
        case ALBUM:
            return this->m_strAlbum;
        case COMMENT:
            return this->m_strComment;
        case GENRE:
            return this->m_strGenre;
        case HASH:
            return this->m_strHash;
        case LAST_PLAY_TIME:
            return this->m_strLastPlayTime;
        case SHOW_TITLE:
            return this->m_strShowTitle;
        case BITRATE:
            return QString("%1").arg(this->m_bitrate);
        case SAMPLE_RATE:
            return QString("%1").arg(this->m_sampleRate);
        case LENGTH:
            return QString("%1").arg(this->m_uiLength);
        case YEAR:
            return QString("%1").arg(this->m_uiYear);
        case TRACK:
            return QString("%1").arg(this->m_uiTrack);
        case FILE_SIZE:
            return QString("%1").arg(this->m_uiFileSize);
        default:
            return QString("no this column(%1)").arg(tagColumn);
    }
    return "never touch here";
}

unsigned int MusicTag::columnInt(MusicTag::TagColumn tagColumn) const
{
    switch(tagColumn)
    {
        case BITRATE:
            return (this->m_bitrate);
        case SAMPLE_RATE:
            return (this->m_sampleRate);
        case LENGTH:
            return (this->m_uiLength);
        case YEAR:
            return (this->m_uiYear);
        case TRACK:
            return (this->m_uiTrack);
        case FILE_SIZE:
            return (this->m_uiFileSize);
        default:
            return 0;
    }
    return 0;
}

bool MusicTag::operator<(const MusicTag & tag) const
{
    switch(this->m_sortBy)
    {
        case BITRATE:
        case SAMPLE_RATE:
        case LENGTH:
        case YEAR:
        case TRACK:
        case FILE_SIZE:
            return this->columnInt(this->m_sortBy) < tag.columnInt(tag.sortBy());
        default:
            return this->columnStr(this->m_sortBy) < tag.columnStr(tag.sortBy());
    }
}

QString MusicTag::computeHash(const QString& s)
{//:TODO
    return "no hash method be implements";
}

unsigned int MusicTag::fileSize(const QString& s)
{
    QFileInfo info(s);
    if(!info.exists())
    {
        //emit meetError(MT_FILE_NOT_EXIST,QString("when test fileSize, file(%1) was not exists").arg(s));
        return 0;
    }

    return info.size();
}

void MusicTag::clean()
{
    m_strFileName = m_strTitle = m_strArtist = m_strAlbum = m_strComment = m_strGenre = m_strHash = m_strLastPlayTime = "";
    this->m_uiFileSize = this->m_uiTrack = this->m_uiYear = this->m_uiLength = 0;
    this->m_bitrate = this->m_sampleRate = 0;
    this->m_bInited = false;
    this->m_sortBy = MusicTag::SHOW_TITLE;
}

bool MusicTag::init(bool bCheckExistsTag)
{
   logtrace("deal (%s)!", qPrintable(m_strFileName));
   // this->m_tagRef = new TagLib::FileRef;
    if(bCheckExistsTag && this->m_tagRef && !this->m_tagRef->isNull())
    {
       // 
        return true;
    }

    if(this->m_strFileName.isEmpty())
    {
        emit meetError(MT_FILE_NOT_EXIST,tr("when read tag, find no file name was setted"));
        return false;
    }

    QFileInfo info(this->m_strFileName);
    
    if(!info.exists())
    {
        emit meetError(MT_FILE_NOT_EXIST,tr("when read tag, file(%1) was not exists").arg(this->m_strFileName));
        return false;
    }

    this->m_uiFileSize = info.size();
    this->m_strTitle = info.fileName();
    m_bInited = true;
    m_tagRef = new TagLib::FileRef(m_strFileName.toLocal8Bit().data());
    if(m_tagRef->isNull())
    {
        logwarning("(%s)'s tag is not readable!", qPrintable(m_strFileName));
        return false;
    }
    logtrace("(%s)'s tag is readable!", qPrintable(m_strFileName));
    return true;
}

QString MusicTag::formatShowTitle(const QString & format)
{
    int i = 0;
    QString strRet;
    while(i < format.size())
    {
        if('%' == format.at(i).unicode())
        {
            i++;
            switch(format.at(i).toLatin1())
            {
                case '%':
                {
                    strRet += '%';
                    break;
                }
                case 'f':
                {
                    strRet += this->m_strFileName;
                    break;
                }
                case 't':
                {
                    strRet += this->m_strTitle;
                    break;
                }
                case 'a':
                {
                    strRet += this->m_strArtist;
                    break;
                }
                case 'u':
                {
                    strRet += this->m_strAlbum;
                    break;
                }
                case 'c':
                {
                    strRet += this->m_strComment;
                    break;
                }
                case 'g':
                {
                    strRet += this->m_strGenre;
                    break;
                }
                case 'h':
                {
                    strRet += this->m_strHash;
                    break;
                }
                case 'p':
                {
                    strRet += this->m_strLastPlayTime;
                    break;
                }
                case 'b':
                {
                    strRet += QString("%1").arg(this->m_bitrate);
                    break;
                }
                case 's':
                {
                    strRet += QString("%1").arg(this->m_sampleRate);
                    break;
                }
                case 'l':
                {
                    strRet += QString("%1").arg(this->m_uiLength);
                    break;
                }
                case 'z':
                {
                    strRet += QString("%1").arg(this->m_uiFileSize);
                    break;
                }
                case 'y':
                {
                    strRet += QString("%1").arg(this->m_uiYear);
                    break;
                }
                case 'k':
                {
                    strRet += QString("%1").arg(this->m_uiTrack);
                    break;
                }
                default:
                {
                    strRet += format.at(i);
                    break;
                }
            }           
        }
        else
        {
            strRet += format.at(i);
        }
        i++;
    }
    return strRet;
}
