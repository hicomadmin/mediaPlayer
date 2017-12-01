#include <QPushButton>
#include <QSlider>
#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QTime>
#include <QTabWidget>
#include <QTableWidget>
#include <QTabBar>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QStringList>
#include <QFileDialog>
#include <QDesktopWidget>
#include <core/config.h>
#include <core/SoundCore.h>
#include <core/SkinLoader.h>
#include <core/playlistmodel.h>
#include <core/musictag.h>
#include <core/lyricmanager.h>

#include "uibuilder.h"
#include "connector.h"

Connector::Connector(UiBuilder *pUI, PlayListModel* pListModel)
    :m_pUI(pUI), m_pListModel(pListModel),m_lastTime(0),isStopping(false)
{
    connect(this->m_pUI, SIGNAL(skinChanged()),this, SLOT(skinChanged()));
}

void Connector::connectBtn()
{
    QPushButton* pWgt;

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PLAY);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(play()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(pause()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_STOP);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(stop()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_OPEN);
    connect(pWgt, SIGNAL(clicked()),this, SLOT(openFiles()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PREV);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(prev()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_NEXT);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(next()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_LYRIC);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(showHideLyric()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_MUSIC);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(showHideWebForm()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_MUTE);    
    connect(pWgt, SIGNAL(clicked()), this, SLOT(setMuted()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_SINGER);
    // connect(pWgt, SIGNAL(clicked()), SoundCore::instance(), SLOT(play()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_MINIMIZE);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(minimize()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_MINIMODE);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(minimode()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_EXIT);
    connect(pWgt, SIGNAL(clicked()), this, SLOT(quit()));

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_RELOADSKIN);
    connect(pWgt, SIGNAL(clicked()), this->m_pUI, SLOT(reloadSkin()));
}
void Connector::minimize()
{
    this->m_pUI->getDisplay(SkinLoader::DP_PLAY)->showMinimized();
}
void Connector::minimode()
{
    //this->m_pUI->getDisplay(SkinLoader::DP_PLAY)->showMaximized();
    this->m_pUI->getDisplay(SkinLoader::DP_PLAY)->hide();
    this->m_pUI->getDisplay(SkinLoader::DP_WEBFORM)->hide();
}
void Connector::showHideLyric()
{
    QWidget* pWidget = this->m_pUI->getDP_KTVLyric();
    if(pWidget->isHidden())
    {
        pWidget->show();
    }
    else
    {
        pWidget->hide();
    }
}
void Connector::showHideWebForm()
{
    QWidget* pWidget = this->m_pUI->getDP_WebForm();
    if(pWidget->isHidden())
    {
        pWidget->show();
    }
    else
    {
        pWidget->hide();
    }
    Config::instance()->setWebFormVisible(pWidget->isVisible());
}

void Connector::normal()
{
    this->m_pUI->getDisplay(SkinLoader::DP_PLAY)->showNormal();
}

void Connector::quit()
{
    qApp->quit();
}

void Connector::pausePlay()
{
    if(SoundCore::instance()->isPlaying())
    {
        this->pause();
    }
    else if(SoundCore::instance()->isPause())
    {
        this->play();

    }
    else
    {
    }
}

void Connector::skinChanged()
{
    QWidget* pWidget = this->m_pUI->getDP_WebForm();
    if(Config::instance()->webFormVisible())
    {
        pWidget->show();
    }
    else
    {
        pWidget->hide();
    }


    QPushButton* pWgt;

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PLAY);
    pWgt->setEnabled(false);

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);
    pWgt->setEnabled(false);

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_STOP);
    pWgt->setEnabled(false);

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PREV);
    pWgt->setEnabled(false);

    pWgt = this->m_pUI->getBtn(SkinLoader::BTN_NEXT);
    pWgt->setEnabled(false);

    //QDesktopWidget desk;
    //const QRect dskRect = desk.screenGeometry();
    QWidget* pKTVDisplay = this->m_pUI->getDP_KTVLyric();
pKTVDisplay->setHidden(true);
    //pKTVDisplay->resize(dskRect.width(), 200);
    //pKTVDisplay->move(dskRect.width()/35, dskRect.height()/10*7);

}

void Connector::createSystemTray()
{


    QMainWindow* pMain = this->m_pUI->getDisplay(SkinLoader::DP_PLAY);

    nextAction = new QAction(QIcon(":/images/menu_logo (0).png"),tr("&Next"), pMain);
    connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

    prevAction = new QAction(QIcon(":/images/menu_logo (1).png"),tr("&Prev"), pMain);
    connect(prevAction, SIGNAL(triggered()), this, SLOT(prev()));

    pausePlayAction = new QAction(QIcon(":/images/menu_logo (2).png"),tr("Pause"), pMain);
    connect(pausePlayAction, SIGNAL(triggered()), this, SLOT(pausePlay()));

    minimizeAction = new QAction(QIcon(":/images/menu_logo (3).png"),tr("Minimize"), pMain);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimize()));

    minimodeAction = new QAction(QIcon(":/images/menu_logo (4).png"),tr("Minimode"), pMain);
    connect(minimodeAction, SIGNAL(triggered()), this, SLOT(minimode()));

    normalAction = new QAction(QIcon(":/images/menu_logo (5).png"),tr("Normal"), pMain);
    connect(normalAction, SIGNAL(triggered()), this, SLOT(normal()));

    quitAction = new QAction(QIcon(":/images/menu_logo (6).png"),tr("&Quit"), pMain);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));


    trayIconMenu = new QMenu(pMain);
    trayIconMenu->addAction(nextAction);
    trayIconMenu->addAction(pausePlayAction);
    trayIconMenu->addAction(prevAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(minimodeAction);
    trayIconMenu->addAction(normalAction);
    trayIconMenu->addAction(quitAction);
    trayIconMenu->addSeparator();

    trayIcon = new QSystemTrayIcon(pMain);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QIcon icon(":/images/lingou_logo.png");
    trayIcon->setIcon(icon);
    pMain->setWindowIcon(icon);

    trayIcon->show();
}

Connector::~Connector()
{
trayIcon->hide();
delete trayIcon;
}

void Connector::connectSlider()
{
    QSlider* pWgt;

    pWgt = this->m_pUI->getSlider(SkinLoader::SLD_PROCESS);
    pWgt->setMinimum(0);
    connect(pWgt, SIGNAL(sliderMoved(int)),this, SLOT(seek(int)));
    pWgt = this->m_pUI->getSlider(SkinLoader::SLD_VOLUM);    
    connect(pWgt, SIGNAL(valueChanged(int)), this, SLOT(seekVolume(int)));
    pWgt->setValue(Config::instance()->volume());
    if(Config::instance()->mute())
    {        
        pWgt->setProperty("mute", true);
        pWgt->setEnabled(false);
        SoundCore::instance()->setMuted();
    }
}

void Connector::connectLabel()
{
    QLabel* pWgt;

    pWgt = this->m_pUI->getLabel(SkinLoader::LAB_NUMBER);
    connect(SoundCore::instance(), SIGNAL(timeTick(qint64)), this, SLOT(showTime(qint64)));
}

void Connector::connectTabel()
{
    QTableWidget* pWgt;
    logtrace("here");
    pWgt = this->m_pUI->getTable(SkinLoader::TABLE_PLAYLIST);
    pWgt->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pWgt->setSelectionBehavior(QAbstractItemView::SelectRows);
    pWgt->setAlternatingRowColors(true);
    pWgt->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pWgt->rowHeight(20);
    //connect(SoundCore::instance(), SIGNAL(timeTick(qint64)), this, SLOT(showTime(qint64)));
    connect(this->m_pListModel, SIGNAL(listModelChanged()), this, SLOT(updateTablePlayList()));
    connect(this->m_pListModel, SIGNAL(listModelAppend()), this, SLOT(updateTablePlayList()));
    connect(pWgt, SIGNAL(doubleClicked(const QModelIndex & )), this, SLOT(dClickTableIndex(const QModelIndex &)));
    connect(pWgt, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(cellClicked(int, int)));

    //  connect(pWgt, SIGNAL(cellClicked(int, int)),this, SLOT(cellClicked(int, int)));
}

void Connector::connectTabWidget()
{
    QTabWidget* pWgt;

    pWgt = this->m_pUI->getTabWidget(SkinLoader::TAB_PLAYLIST);
    // connect(SoundCore::instance(), SIGNAL(timeTick(qint64)), this, SLOT(showTime(qint64)));
    //connect(this->m_pListModel, SIGNAL(listModelChanged()), this, SLOT(updateTablePlayList()));
}

void Connector::connectAll()
{
    this->connectBtn();
    this->connectSlider();
    this->connectLabel();
    this->connectTabWidget();
    this->connectTabel();
    this->connectOthers();
    this->createSystemTray();
}

void Connector::connectOthers()
{
    connect(SoundCore::instance(), SIGNAL(state2Play()), this, SLOT(state2Play()));

    connect(SoundCore::instance(), SIGNAL(about2Finish()), this, SLOT(next()));
    connect(this->m_pListModel, SIGNAL(addFirst()), this, SLOT(addFirst()));
    //connect(LyricManager::instance(), SIGNAL(
    connect(SoundCore::instance(), SIGNAL(startPlay(const QString &)), this, SLOT(startNewMusicPlay(const QString &)));

    QIcon icon(":/images/lingou_logo.png");
    this->m_pUI->getDP_KTVLyric()->setWindowIcon(icon);

}

void Connector::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(
            this->m_pUI->getDisplay(SkinLoader::DP_PLAY),
            tr("Open Musics"),
            Config::instance()->lastOpenDir(),
            Config::instance()->musicFilters());
    this->m_pListModel->addFiles(files);

    if(files.size() > 0)
    {
        QFileInfo info(files.at(0));
        Config::instance()->setLastOpenDir(info.absoluteDir().path());
        QPushButton* pWgt;

        pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PLAY);
        pWgt->setEnabled(true);

        pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);
        pWgt->setEnabled(true);

        pWgt = this->m_pUI->getBtn(SkinLoader::BTN_STOP);
        pWgt->setEnabled(true);

        pWgt = this->m_pUI->getBtn(SkinLoader::BTN_PREV);
        pWgt->setEnabled(true);

        pWgt = this->m_pUI->getBtn(SkinLoader::BTN_NEXT);
        pWgt->setEnabled(true);

    }



}

void Connector::addFirst()
{
    QPushButton *pPlay=this->m_pUI->getBtn(SkinLoader::BTN_PLAY),
    *pPause=this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);

    pPause->setEnabled(true);
    if(pPlay->x() == pPause->x() && pPlay->y() == pPause->y())
    {
        pPlay->setVisible(false);
        pPause->setVisible(true);
    }
    next();

    //logtrace("有没有音乐lrc文件啊？=%d",LyricManager::instance()->setNewLyricFileName(strLrcPath,this->m_pListModel->currentTag()->length(), this->m_pListModel->currentTag()->showTitle()));

}

void Connector::startNewMusicPlay(const QString & strMusicPath)
{
    //int nIndex = strMusicPath.lastIndexOf('.');

    //QString strLrcPath = strMusicPath.left(nIndex) + ".lrc";
    //QString artist, QString title, QString filePath, QString ext;
    QString strLrcPath = Config::instance()->getAvaibleLyricPath(this->m_pListModel->currentTag()->artist()
                                                                 ,this->m_pListModel->currentTag()->title()
                                                                 ,this->m_pListModel->currentTag()->fileName()
                                                                 ,"lrc");

    loginfo("is Lyric path is %s?", strLrcPath.toUtf8().data());//SoundCore::instance()->totalTime()

    logtrace("有没有音乐lrc文件啊？=%d",LyricManager::instance()->setNewLyricFileName(strLrcPath,this->m_pListModel->currentTag()->length(), this->m_pListModel->currentTag()->showTitle()));
}
void Connector::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->normal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void Connector::play()
{
    if(!(this->m_pListModel->currentTag()))
    {
        return;
    }      
    
    this->m_pUI->getSlider(SkinLoader::SLD_PROCESS)->setEnabled(true);
    SoundCore::instance()->play();
    QPushButton *pPlay=this->m_pUI->getBtn(SkinLoader::BTN_PLAY),
    *pPause=this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);

    if(pPlay->x() == pPause->x() && pPlay->y() == pPause->y())
    {
        pPlay->setVisible(false);
        pPause->setVisible(true);
    }
    pausePlayAction->setText("To Pause");
    pausePlayAction->setEnabled(true);
    LyricManager::instance()->playLyric();
    this->m_pUI->getDisplay(SkinLoader::DP_PLAY)->setWindowTitle(this->m_pListModel->currentTag()->showTitle());


    QMainWindow* pMain = this->m_pUI->getDisplay(SkinLoader::DP_PLAY);
    QIcon icon(":/images/lingou_logo_hover.png");
    trayIcon->setIcon(icon);
    pMain->setWindowIcon(icon);


    //QSystemTrayIcon::MessageIcon msgIcon(QIcon(":/images/show_msg.png"), pMain);
    trayIcon->showMessage(QString::fromUtf8("当前播放："),this->m_pListModel->currentTag()->showTitle(),QSystemTrayIcon::Information,
                          2 * 1000);
}

void Connector::pause()
{
    if(!(this->m_pListModel->currentTag()))
    {
        return;
    }   
    SoundCore::instance()->pause();
    QPushButton *pPlay=this->m_pUI->getBtn(SkinLoader::BTN_PLAY),
    *pPause=this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);

    if(pPlay->x() == pPause->x() && pPlay->y() == pPause->y())
    {
        pPause->setVisible(false);
        pPlay->setVisible(true);
    }
    LyricManager::instance()->pauseLyric();
    pausePlayAction->setText("To Play");
    pausePlayAction->setEnabled(true);

    QMainWindow* pMain = this->m_pUI->getDisplay(SkinLoader::DP_PLAY);
    QIcon icon(":/images/lingou_logo_pressed.png");
    trayIcon->setIcon(icon);
    pMain->setWindowIcon(icon);
}

void Connector::stop()
{

    if(!(this->m_pListModel->currentTag()))
    {
        return;
    }   
    SoundCore::instance()->stop();
    QSlider* pWgt;
    pWgt = this->m_pUI->getSlider(SkinLoader::SLD_PROCESS);
    pWgt->setValue(0);
    pWgt->setEnabled(false);
    this->m_pUI->getLabel(SkinLoader::LAB_PLAYINFO)->setText("");
    this->m_pUI->getLabel(SkinLoader::LAB_NUMBER)->setText("");
    QPushButton *pPlay=this->m_pUI->getBtn(SkinLoader::BTN_PLAY),
    *pPause=this->m_pUI->getBtn(SkinLoader::BTN_PAUSE);

    if(pPlay->x() == pPause->x() && pPlay->y() == pPause->y())
    {
        pPause->setVisible(false);
        pPlay->setVisible(true);
    }
    LyricManager::instance()->stopLyric();
    pausePlayAction->setText("stopped");
    pausePlayAction->setEnabled(false);

    QMainWindow* pMain = this->m_pUI->getDisplay(SkinLoader::DP_PLAY);
    QIcon icon(":/images/lingou_logo.png");
    trayIcon->setIcon(icon);
    pMain->setWindowIcon(icon);
    m_lastTime = 0;
}


void Connector::prev()
{
    if(this->m_pListModel->hasPrev())
    {
        isStopping = true;
        stop();
        this->m_pListModel->prev();
        QTableWidget* pWgt = this->m_pUI->getTable(SkinLoader::TABLE_PLAYLIST);
        pWgt->selectRow(this->m_pListModel->currentIndex());
        play();
        startNewMusicPlay(this->m_pListModel->currentTag()->fileName());
        isStopping = false;
        logtrace(" 7#m_lastTime = %ld",  m_lastTime);
        m_lastTime = 0;
    }
    else
    {
        stop();
        loginfo("no prev any more, current is %d", this->m_pListModel->currentIndex());
    }
}

void Connector::next()
{
    if(this->m_pListModel->hasNext())
    {
        logtrace(" 1#m_lastTime = %ld",  m_lastTime);
        isStopping = true;
        stop();
        logtrace(" 2#m_lastTime = %ld",  m_lastTime);
        this->m_pListModel->next();
        QTableWidget* pWgt = this->m_pUI->getTable(SkinLoader::TABLE_PLAYLIST);
        pWgt->selectRow(this->m_pListModel->currentIndex());
        play();
        startNewMusicPlay(this->m_pListModel->currentTag()->fileName());
        isStopping = false;
        logtrace(" 3#m_lastTime = %ld",  m_lastTime);
        m_lastTime = 0;
    }
    else
    {
        stop();
        loginfo("no next any more, current is %d", this->m_pListModel->currentIndex());
    }
  //  logtrace("dclick %d", index.row());
  //  stop();
  //  this->m_pListModel->setCurrent(index.row());
  //  play();

}

void Connector::dClickTableIndex(const QModelIndex & index)
{
    logtrace("dclick %d", index.row());
    stop();
    this->m_pListModel->setCurrent(index.row());
    play();
}

void Connector::cellClicked(int row, int column)
{
    logtrace("cellclick %d", row);
    this->m_pListModel->setCurrent(row);
    QTableWidget* pWgt = this->m_pUI->getTable(SkinLoader::TABLE_PLAYLIST);
    pWgt->selectRow(row);
}


void Connector::seek(int n)
{
    //logtrace("click pos is %d", n);
    //SoundCore::instance()->pause();
    SoundCore::instance()->seek(n*1000);
    //SoundCore::instance()->play();
}

void Connector::seekVolume(int n)
{
    //logtrace("click pos is %d", n);

    SoundCore::instance()->setVolume(static_cast<qreal>(n));    
    Config::instance()->setVolume(n);
}

void Connector::updateTablePlayList()
{
    
    logtrace("here");
    QList<MusicTag*> tags = (*(m_pListModel->tags()));
    QStringList headers;
    headers << tr("title")<< tr("length");
    QTableWidget* pWgt = this->m_pUI->getTable(SkinLoader::TABLE_PLAYLIST);
    pWgt->setColumnCount(2);
    pWgt->setRowCount(tags.size());
    pWgt->setColumnWidth(0, (int)(pWgt->width()*0.80));
    pWgt->setColumnWidth(1, (int)(pWgt->width()*0.16));
    pWgt->setHorizontalHeaderLabels(headers);

    
    QTableWidgetItem* item = 0;
    for(int i = 0; i < tags.size(); i ++)
    {
        //for(int j = 0; j < 3; j++)
        {
            item = new QTableWidgetItem(tags[i]->showTitle());
            //item->setAlternatingRowColors(true);
            pWgt->setItem(i, 0, item);
            pWgt->setRowHeight(i,20);

            item = new QTableWidgetItem(tags[i]->lengthStr());
            
            pWgt->setItem(i, 1, item);
            //item = new QTableWidgetItem(tags[i]->album());
            // pWgt->setItem(i, 2, item);
        }
    }
}


void Connector::setMuted()
{
    QSlider* pWgt = this->m_pUI->getSlider(SkinLoader::SLD_VOLUM);
    if(SoundCore::instance()->isMuted())
    {//before is muted, so realse the slider bar
        pWgt->setEnabled(true);
        pWgt->setProperty("mute", false);
    }
    else
    {
        pWgt->setEnabled(false);
        pWgt->setProperty("mute", true);
    }
    
    SoundCore::instance()->setMuted();
    Config::instance()->setMuted(SoundCore::instance()->isMuted());
}

void Connector::state2Play()
{
    //qint64 len = this->m_pListModel->currentTag()->length()*1000;//SoundCore::instance()->totalTime();
    //int sec_total = len / 1000;
    if(!(this->m_pListModel->currentTag()))
    {
        logtrace("this->m_pListModel->currentTag() is null ");

    }
    else
    {
        logtrace("this->m_pListModel->currentTag() is not null ");
    }

    this->m_pUI->getSlider(SkinLoader::SLD_PROCESS)->setMaximum(this->m_pListModel->currentTag()->length()-1);

    this->m_pUI->getLabel(SkinLoader::LAB_PLAYINFO)->setText(this->m_pListModel->currentTag()->showTitle());


}

void Connector::showTime(qint64 nTime)
{


    qint64 len = this->m_pListModel->currentTag()->length()*1000; //SoundCore::instance()->totalTime();
    qint64 pos = SoundCore::instance()->currentTime();
    if(isStopping || (m_lastTime==0 && pos>2*1000))
    {
        this->m_pUI->getLabel(SkinLoader::LAB_NUMBER)->setText("stopping...");
        return;
    }
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;
        int sec_current = sec;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;
        // int sec_total = sec;
logtrace("pos=%lld,len=%lld,sec_current = %d, m_lastTime = %ld",pos,len,sec_current,  m_lastTime);
        if(pos > m_lastTime)
        {
            LyricManager::instance()->scroll(pos, len);
            m_lastTime = pos;
            logtrace("sec_current = %d, m_lastTime = %ld",sec_current,  m_lastTime);

        }


        if(len > 0){
            //int iValue = (sec_current*100)/sec_total;
            // this->m_pUI->getSlider(SkinLoader::SLD_PROCESS)->setMaximum(sec_total);
            // logtrace("iValue = %d, pos=%lld, len = %lld, sec_current=%d, sec_total=%d", iValue, pos, len,sec_current, sec_total);
            this->m_pUI->getSlider(SkinLoader::SLD_PROCESS)->setValue(sec_current);
        }

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    this->m_pUI->getLabel(SkinLoader::LAB_NUMBER)->setText(timeString);

    // logtrace("%ld", nTime);
    /*   int seconds = nTime/1000;
    int mins = seconds/60;
    int hours=mins/60;
    QTime time(hours, mins, seconds);
    this->m_pUI->getLabel(SkinLoader::LAB_NUMBER)->setText(time.toString("hh:mm:ss"));
*/
}
