//#include <QtGui>
#include <QTimer>
#include "View.h"
#include "flowlayout.h"
#include "lyricline.h"
#include <core/lyricmanager.h>
#include <core/lyricparser.h>

QColor colors[] = { Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
                    QColor(255, 183, 230), QColor(160, 200, 120), QColor(150, 85, 79) };
const char *names[] = { "我就是世界上最伤心的人，我也是世界上最看你的人, 我也是这个salfdkksdflasdf你的人", "Green", "我的心为你晟?", "Cyan", "Magenta", "Yellow",
                        "Custom 1", "Custom 2", "Custom 3" };

View::View(QWidget *parent)
    : QGraphicsView(parent)
    , m_widget(new QWidget),m_lastFillUp(false),m_parent(parent)
{
    //m_widget->resize(100, 100);
    //m_widget->move(100, 100);
    //QPushButton* btn = new QPushButton("adsfasdasdfasdfflkjasdf");
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::transparent);

    viewport()->setAutoFillBackground(false);
    m_widget->setPalette(palette);

    QVBoxLayout* layout = new QVBoxLayout(m_widget);
    layout->setAlignment(Qt::AlignCenter);

    FlowLayout* layoutUp = new FlowLayout();
    FlowLayout* layoutDown = new FlowLayout();
    m_pLabelUp = new LyricLine("");
    m_pLabelDown = new LyricLine("");
    // m_pBtn = new QPushButton("exit");

    layoutUp->addWidget(m_pLabelUp);
    // layoutUp->addWidget(m_pBtn);
    //layout->layout()->addWidget(btn);
    layoutUp->setIndexRight(0, false);
    //layoutUp->setIndexRight(1, true);

    layoutDown->addWidget(m_pLabelDown);
    layoutDown->setIndexRight(0, true);
    
    layout->addLayout(layoutUp);
    layout->addLayout(layoutDown);

    //  connect(m_pBtn, SIGNAL(clicked()), qApp, SLOT(quit()));
    
    m_scene = new QGraphicsScene;
    setScene(m_scene);
    setFrameShape(QFrame::NoFrame);

    m_item = m_scene->addWidget(m_widget);
    m_item->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    setRenderHints(QPainter::SmoothPixmapTransform);
    m_widget->show();
    //this->show();
    QScrollBar* pScollBar = horizontalScrollBar();
    pScollBar->setVisible(false);
    pScollBar = verticalScrollBar();
    pScollBar->setVisible(false);
    //this->setResizeAnchor(QGraphicsView::NoAnchor);

    connect(LyricManager::instance(), SIGNAL(startNewLyric(const QString & , long)), this, SLOT(startNewLyric(const QString & , long)));
    connect(LyricManager::instance(), SIGNAL(updateProcess(long, long, long)), this, SLOT(updateProcess(long, long, long)));
    connect(LyricManager::instance(), SIGNAL(nextStep()), this, SLOT(nextStep()));
    connect(LyricManager::instance(), SIGNAL(nextSentence()), this, SLOT(nextSentence()));
    connect(LyricManager::instance(), SIGNAL(playCurLyric()), this, SLOT(playCurLyric()));
    connect(LyricManager::instance(), SIGNAL(pauseCurLyric()), this, SLOT(pauseCurLyric()));
    connect(LyricManager::instance(), SIGNAL(stopCurLyric()), this, SLOT(stopCurLyric()));


}

void View::showOneLyricInfo(const QString & str)
{
    logtrace("set up label text for %s", str.toUtf8().data());
    this->m_pLabelUp->setText(str);
}

void View::startNewLyric(const QString & str, bool founded)
{
   // logtrace("str = %s, totalTime = %ld", str.toLocal8Bit().data(), totalTime);
    if(!founded)
    {
        if(this->m_parent->isHidden())
        {
            m_parent->show();
        }
        showOneLyricInfo(str);
        QTimer::singleShot(1000*3,this,SLOT(stopCurLyric()));
        return;
    }

    if(LyricManager::instance()->m_pLrc->nodes()->size() > 0)
    {
        LyricInfo* pInfo = LyricManager::instance()->m_pLrc->info();

        LyricNode* pFirst = LyricManager::instance()->m_pLrc->nodes()->at(0);
        int timeOne = pFirst->timeToMsecs();
        logtrace("timeOne = %d,timeOne/4000=%d", timeOne,timeOne/4);
        if(this->m_parent->isHidden())
        {
            m_parent->show();
        }
        QString title = QString::fromUtf8("曲名：") + QString::fromLocal8Bit(pInfo->title.c_str());
        showOneLyricInfo(title);
        if(timeOne > 3*1000)
        {
            timeOne = timeOne/4;
            // QTimer::singleShot(0,this,SLOT(showOneLyricInfo(title)));


            QString artist = QString::fromUtf8("演唱：") + QString::fromLocal8Bit(pInfo->artist.c_str());
            QTimer::singleShot(timeOne*1,this,SLOT(showOneLyricInfo(artist)));
            logtrace("showOneLyricInfo will be start after %d ms", timeOne*1);
            QTimer *timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(showOneLyricInfo(artist)));
            timer->start(1000);

            QString album = QString::fromUtf8("专辑：") + QString::fromLocal8Bit(pInfo->album.c_str());
            QTimer::singleShot(timeOne*2,this,SLOT(showOneLyricInfo(album)));

            QString author = QString::fromUtf8("歌词作者：") + QString::fromLocal8Bit(pInfo->author.c_str());
            QTimer::singleShot(timeOne*3,this,SLOT(showOneLyricInfo(author)));           
        }
        else
        {
            logtrace("timeOne is %d", timeOne);
        }
    }
}

void View::updateProcess(long begin, long cur, long end)
{
    float lF = cur-begin;
    float lS = end - begin;
    float persent = lF/lS;
    logtrace("begin = %ld, cur = %ld, end = %ld,lF=%f,lS=%f, persent=%f", begin, cur, end,lF,lS, persent);

    if(m_lastFillUp)
    {
        this->m_pLabelDown->buildResultImg(persent);
    }
    else
    {
        this->m_pLabelUp->buildResultImg(persent);
    }
}
void View::nextStep()
{
    logwarning("no come true");
}

void View::nextSentence()
{
    logonlytrace;
    LyricNode* next = LyricManager::instance()->nextNode();
    if(next)
    {
        // logtrace("QString::fromStdString(next->lrc) = %s", QString::fromStdString(next->lrc).toUtf8().data());
        // logtrace("QString::fromLocal8Bit(next->lrc) = %s", QString::fromLocal8Bit(next->lrc).toUtf8().data());
        // logtrace("QString::fromStdString_c_str(next->lrc) = %s", QString::fromStdString(next->lrc.c_str()).toUtf8().data());
        // logtrace("QString::fromLocal8Bit_c_str(next->lrc) = %s", QString::fromLocal8Bit(next->lrc.c_str()).toUtf8().data());
        //  logtrace("(next->lrc) = %s", next->lrc.c_str());

        if(m_lastFillUp)
        {
            this->m_pLabelDown->setText(QString::fromLocal8Bit(next->lrc.c_str()));
        }
        else
        {
            this->m_pLabelUp->setText(QString::fromLocal8Bit(next->lrc.c_str()));
        }
        m_lastFillUp = !m_lastFillUp;
    }
}

void View::playCurLyric()
{
    if(this->m_parent->isHidden())
    {
        m_parent->show();
    }
}

void View::pauseCurLyric()
{
    if(this->m_parent->isHidden())
    {
        m_parent->show();
    }
}

void View::stopCurLyric()
{
    m_lastFillUp = false;
    this->m_pLabelDown->setText("");
    this->m_pLabelUp->setText("");
    if(this->m_parent->isVisible())
    {
        m_parent->hide();
    }
}
