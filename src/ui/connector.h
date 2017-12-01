#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QSystemTrayIcon>
#include <core/Log.h>

class UiBuilder;
class PlayListModel;
class QModelIndex;
class QMenu;
class QAction;

class Connector : public QObject
{
    Q_OBJECT
public:
    Connector(UiBuilder *pUI, PlayListModel* pListModel);
    ~Connector();

    void connectBtn();
    void connectSlider();
    void connectLabel();
    void connectOthers();
    void connectTabWidget();
    void connectTabel();
    void connectAll();
signals:
    void meetError(ErrorType errorType, const QString & strWhy);
public slots:
    void play();
    void pause();
    void stop();
    void prev();
    void next();
    void openFiles();
    void showTime(qint64 nTime);
    void seek(int n);
    void state2Play();
    void setMuted();
    void seekVolume(int);
    void updateTablePlayList();
    void dClickTableIndex(const QModelIndex & index);
    void cellClicked(int row, int column);
    void addFirst();
    void startNewMusicPlay(const QString & strMusicPath);
    void minimize();
    void minimode();
    void normal();
    void quit();
    void pausePlay();
    void showHideLyric();
    void showHideWebForm();
    void skinChanged();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    void createSystemTray();
private:
    UiBuilder *m_pUI;
    PlayListModel* m_pListModel;
    long m_lastTime;
    bool isStopping;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *nextAction;
    QAction *pausePlayAction;
    QAction *prevAction;
    QAction *minimizeAction;
    QAction *minimodeAction;
    QAction *normalAction;
    QAction *quitAction;
};

#endif // CONNECTOR_H
