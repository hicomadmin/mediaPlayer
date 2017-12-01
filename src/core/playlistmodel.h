#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QObject>

#include "musictag.h"

class PlayListModel : public QObject
{
    Q_OBJECT
public:

    PlayListModel(const QString &strName = "default");

    bool insert(int iWhere,MusicTag * tag);

    bool remove(int iFrom, int iTo = -1); //iFrom <= where <= iTo

    void addFile(const QString & strFilePath);

    void addFiles(const QStringList & filePaths);

    void addDir(const QString & strDirPath, bool bRecursion = false);

    void sort(MusicTag::TagColumn sortBy = MusicTag::SHOW_TITLE);

    QString name() const;

    void setName(const QString &strName);

    int currentIndex();
    MusicTag* currentTag();

    QList<MusicTag*>* tags();
    
    bool hasNext();
    bool hasPrev();

public slots:
    void next();
    void prev();

    void setCurrent(int iWhere);

signals:
    void listModelChanged();
    void listModelAppend();
    void addFirst();
private:
    void addFileToTags(const QString & str);
private:
    QString m_strName;
    QList<MusicTag*>* m_tags;
    int m_iCurrent;
};

#endif // PLAYLISTMODEL_H
