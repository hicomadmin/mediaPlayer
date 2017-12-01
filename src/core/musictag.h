#ifndef MUSICTAG_H
#define MUSICTAG_H

#include <QObject>
//#include <fileref.h>
#include "Log.h"
namespace TagLib{
    class FileRef;
}
class MusicTag : public QObject
{
    Q_OBJECT
public:
    enum TagColumn {
        FILE_NAME = 1<<0,
        TITLE = 1<<1,
        ARTIST = 1<<2,
        ALBUM = 1<<3,
        COMMENT = 1<<4,
        GENRE = 1<<5,
        HASH = 1<<6,
        LAST_PLAY_TIME = 1<<7,
        SHOW_TITLE = 1<<8,
        BITRATE = 1<<9,
        SAMPLE_RATE = 1<<10,
        LENGTH = 1<<11,
        YEAR = 1<<12,
        TRACK = 1<<13,
        FILE_SIZE = 1<<14,
    };
public:
    MusicTag();

    MusicTag(const QString & strFileName, bool readNow = false);
    ~MusicTag();
private:
    MusicTag(const MusicTag & copy);
    MusicTag& operator =(const MusicTag & copy);
public:
    bool readTag();

    bool writeTag();

    QString fileName() const;

    QString title() const;

    QString artist() const;

    QString album() const;

    QString comment() const;

    QString genre() const;

    QString hash() const;

    QString lastPlayTime() const;

    QString showTitle() const;
    QString lengthStr() const;
    int bitrate() const;

    int sampleRate() const;

    unsigned int length() const;

    unsigned int year() const;

    unsigned int track() const;

    unsigned int fileSize() const;

    TagColumn sortBy() const;

    bool setFileName(const QString &s, bool readNow = false);

    bool setTitle(const QString &s, bool writeNow = false);

    bool setArtist(const QString &s, bool writeNow = false);

    bool setAlbum(const QString &s, bool writeNow = false);

    bool setComment(const QString &s, bool writeNow = false);

    bool setGenre(const QString &s, bool writeNow = false);

    bool setYear(unsigned int i, bool writeNow = false);

    bool setTrack(unsigned int i, bool writeNow = false);

    void setHash(const QString &s);

    void setLastPlayTime(const QString &s);

    void setFileSize(unsigned int uiFileSize);

    void setShowTitleFormat(const QString & strFormat, bool updateNow = false);

    void setShowTitle(const QString & strShowTitle);

    void setSortBy(TagColumn sortBy);

    bool testHashEq();

    bool testFileSizeEq();

    bool testIsRightFile();

    bool fileExists();

    QString columnStr(TagColumn tagColumn) const;

    unsigned int columnInt(TagColumn tagColumn) const;

    bool operator<(const MusicTag & tag) const;

public:
    static QString computeHash(const QString& s);
    static unsigned int fileSize(const QString& s);


signals:
    void meetError(ErrorType type, const QString &why);

private:
    void clean();
    bool init(bool bCheckExistsTag = true);
    QString formatShowTitle(const QString & format);

private:
    QString m_strFileName;
    QString m_strFormat;
    QString m_strShowTitle;
    QString m_strTitle;
    QString m_strArtist;
    QString m_strAlbum;    
    QString m_strComment;
    QString m_strGenre;
    QString m_strHash;
    QString m_strLastPlayTime;

    int m_bitrate;
    int m_sampleRate;

    unsigned int m_uiLength;
    unsigned int m_uiFileSize;
    unsigned int m_uiYear;
    unsigned int m_uiTrack;

    bool m_bInited;
    TagColumn m_sortBy;
    TagLib::FileRef* m_tagRef;
};

#endif // MUSICTAG_H
