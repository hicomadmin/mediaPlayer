#include <QPushButton>
#include <QWidget>
#include <QMainWindow>
#include <QLayout>
#include <QBitmap>
#include <QLabel>
#include <QSlider>
#include <QTableWidget>
#include <QApplication>
#include <QTabWidget>
#include <QTabBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>
#include <QFontMetrics>
#include <QPainterPath>
#include <QPainter>
#include <QPixmap>
#include <QLinearGradient>
#include <QPen>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <core/SoundCore.h>
#include <core/ComponentAttr.h>
#include <core/LGSkinLoader.h>
#include <core/config.h>

#include "uibuilder.h"
//#include "renderarea.h"
#include "View.h"

#define ENUM_NAME(name) #name

DockWidget::DockWidget(QWidget *parent)
    :QMainWindow(parent,Qt::FramelessWindowHint),m_pFriends(new QVector<QWidget*>())
{

}

void DockWidget::addDockWidget(QWidget* pWidget)
{
    m_pFriends->push_back(pWidget);
}


void DockWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        //  dragPosition = frameGeometry().topLeft();
        event->accept();
    }
}

void DockWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint gp = event->globalPos();
        //   logtrace("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ngp.x(%d), y(%d)", gp.x(), gp.y());
        QPoint myP = gp - dragPosition;
        //  logtrace("1myP.x(%d), y(%d)", myP.x(), myP.y());
        //  logtrace("1dragPosition.x(%d), y(%d)", dragPosition.x(), dragPosition.y());


        QVector<QWidget*>::iterator it = m_pFriends->begin();
        while(it != m_pFriends->end())
        {
            (*it)->raise();
            int friendX = (*it)->frameGeometry().topLeft().x();
            int myX = frameGeometry().topLeft().x();
            int width = friendX - myX;
            // logtrace("friendX(%d), myX(%d), width(%d)", friendX, myX, width);
            QPoint friendP = gp - dragPosition+QPoint(width,0);
            // logtrace("friendP.x(%d), y(%d)", friendP.x(), friendP.y());
            (*it)->move(friendP);
            it++;
        }
        // logtrace("2myP.x(%d), y(%d)", myP.x(), myP.y());
        //  logtrace("//////////////////////////////\n2dragPosition.x(%d), y(%d)", dragPosition.x(), dragPosition.y());
        move(myP);
        event->accept();
    }
}

UiBuilder::UiBuilder(SkinLoader* pSkLoader)
    : m_pSkLoader(pSkLoader), m_widgets(SkinLoader::ENUM_CELL,0),pKTVDisplay(0)
{
    connect(m_pSkLoader, SIGNAL(meetError(ErrorType, const QString&)), this, SLOT(dealCommonError(ErrorType, const QString&)));
    connect(this, SIGNAL(meetError(ErrorType, const QString&)), this, SLOT(dealCommonError(ErrorType, const QString&)));
    this->m_pSkLoader->loadSkinFromFile();
}

QMainWindow* UiBuilder::getDP_Play()
{
    QMainWindow* pMain = this->getDisplay(SkinLoader::DP_PLAY);
    //pMain->show();    
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_PAUSE, this->getDisplay(SkinLoader::DP_PLAY)));
    this->getBtn(SkinLoader::BTN_PAUSE)->setEnabled(false);
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_PLAY, this->getDisplay(SkinLoader::DP_PLAY)));
    this->getBtn(SkinLoader::BTN_PLAY)->setEnabled(false);
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_STOP, this->getDisplay(SkinLoader::DP_PLAY)));
    this->getBtn(SkinLoader::BTN_STOP)->setEnabled(false);
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_OPEN, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_PREV, this->getDisplay(SkinLoader::DP_PLAY)));
    this->getBtn(SkinLoader::BTN_PREV)->setEnabled(false);
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_NEXT, this->getDisplay(SkinLoader::DP_PLAY)));
    this->getBtn(SkinLoader::BTN_NEXT)->setEnabled(false);
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_LYRIC, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_MUSIC, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_MUTE, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_SINGER, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_MINIMIZE, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_MINIMODE, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_EXIT, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getBtn(SkinLoader::BTN_RELOADSKIN, this->getDisplay(SkinLoader::DP_PLAY)));

    pMain->layout()->addWidget(this->getSlider(SkinLoader::SLD_VOLUM, this->getDisplay(SkinLoader::DP_PLAY)));
    
    pMain->layout()->addWidget(this->getSlider(SkinLoader::SLD_PROCESS, this->getDisplay(SkinLoader::DP_PLAY)));

    pMain->layout()->addWidget(this->getLabel(SkinLoader::LAB_NUMBER, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getLabel(SkinLoader::LAB_PLAYINFO, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getLabel(SkinLoader::LAB_USERNAME, this->getDisplay(SkinLoader::DP_PLAY)));
    pMain->layout()->addWidget(this->getLabel(SkinLoader::LAB_SHOWINFO, this->getDisplay(SkinLoader::DP_PLAY)));
    
    QTabWidget* pTabs = this->getTabWidget(SkinLoader::TAB_PLAYLIST, this->getDisplay(SkinLoader::DP_PLAY));
    
    pTabs->addTab(this->getTable(SkinLoader::TABLE_PLAYLIST, pTabs), tr("PlayList"));
    
    pMain->layout()->addWidget(pTabs);
    return pMain;
}

QMainWindow* UiBuilder::getDP_WebForm()
{
    QMainWindow* pMain = this->getDisplay(SkinLoader::DP_WEBFORM,this->getDisplay(SkinLoader::DP_PLAY));
    pMain->resize(983, 646);

    return pMain;
}

QWidget* UiBuilder::getDP_KTVLyric()
{
    if(!pKTVDisplay)
    {
        QDesktopWidget desk;
        const QRect dskRect = desk.screenGeometry();
        pKTVDisplay = new QWidget(0, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint|Qt::SubWindow);
        pKTVDisplay->setAttribute(Qt::WA_NoSystemBackground);
        pKTVDisplay->setAttribute(Qt::WA_TranslucentBackground);

        new QGridLayout(pKTVDisplay);
        pKTVDisplay->resize(dskRect.width(), 180);
        pKTVDisplay->move(0, dskRect.height()/10*7);
        pKTVDisplay->layout()->addWidget(new View(pKTVDisplay));

    }
    return pKTVDisplay;
}
void UiBuilder::doMain()
{
    // QMainWindow* pMain = new QMainWindow;
    //pMain->layout()->addWidget(getDP_Play());
    //pMain->layout()->addWidget(getDP_WebForm());


    DockWidget* dockPlay = qobject_cast<DockWidget*>(getDP_Play());
    DockWidget* dockWebForm = qobject_cast<DockWidget*>(getDP_WebForm());

    //dockPlay->show();

    dockPlay->addDockWidget(dockWebForm);
    dockWebForm->addDockWidget(dockPlay);
    // return pMain;
}

void UiBuilder::dealCommonError(ErrorType errorType,const QString& why)
{
    logerror("meet error:%s",qPrintable(why));
    this->getBtn(SkinLoader::LAB_SHOWINFO)->setText(why);
}

QTableWidget* UiBuilder::getTable(SkinLoader::ComponentID id, QWidget* pParent/* = 0*/)
{
    QTableWidget* pRet = 0;
    if(!this->m_widgets.at(id))
    {
        pRet = new QTableWidget(pParent);
        if(!setWidgetAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }
        this->m_widgets[id] = pRet;
    }
    return qobject_cast<QTableWidget*>(this->m_widgets.at(id));
}

QPushButton* UiBuilder::getBtn(SkinLoader::ComponentID id, QWidget* pParent)
{
    QPushButton* pRet = 0;
    if(!this->m_widgets.at(id))
    {   

        pRet = new QPushButton(pParent);
pRet->setWindowFlags(pRet->windowFlags()|Qt::SubWindow);
        if(!setWidgetAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }

        this->m_widgets[id] = pRet;
    }

    return qobject_cast<QPushButton*>(this->m_widgets.at(id));
}

QLabel* UiBuilder::getLabel(SkinLoader::ComponentID id, QWidget* pParent)
{
    QLabel* pRet = 0;
    if(!this->m_widgets.at(id))
    {
        pRet = new QLabel(pParent);
pRet->setWindowFlags(pRet->windowFlags()|Qt::SubWindow);
        if(!setWidgetAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }
        this->m_widgets[id] = pRet;
    }

    return qobject_cast<QLabel*>(this->m_widgets.at(id));
}

QTabWidget* UiBuilder::getTabWidget(SkinLoader::ComponentID id, QWidget* pParent)
{
    QTabWidget* pRet = 0;
    if(!this->m_widgets.at(id))
    {
        pRet = new QTabWidget(pParent);
        if(!setWidgetAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }
        this->m_widgets[id] = pRet;
    }

    return qobject_cast<QTabWidget*>(this->m_widgets.at(id));
}

QMainWindow* UiBuilder::getDisplay(SkinLoader::ComponentID id, QWidget* pParent)
{
    QMainWindow* pRet = 0;
    if(!this->m_widgets.at(id))
    {
        if(id == SkinLoader::DP_WEBFORM)
        {
            pRet = new DockWidget(pParent);

        }
        else
        {
            pRet = new DockWidget(pParent);

            //pRet->show();
            //logonlytrace;
        }

        if(!setWidgetAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }
        this->m_widgets[id] = pRet;
    }

    return qobject_cast<QMainWindow*>(this->m_widgets.at(id));
}
QSlider* UiBuilder::getSlider(SkinLoader::ComponentID id, QWidget* pParent)
{
    QSlider* pRet = 0;
    if(!this->m_widgets.at(id))
    {
        pRet = new QSlider(pParent);
        if(!setSliderAttr(id, pRet))
        {
            delete pRet;
            return 0;
        }
        this->m_widgets[id] = pRet;
    }

    return qobject_cast<QSlider*>(this->m_widgets.at(id));
}

bool UiBuilder::setSliderAttr(SkinLoader::ComponentID id, QSlider* pWidget)
{
    if(!pWidget)
    {
        emit meetError(UB_WIDGET_POINTER_IS_NULL,QString("can't set widget attribute for widget pointer(%1:%2) is null").arg(ENUM_NAME(id)).arg(id));
        return false;
    }
    ComponentAttr* pAttr = this->m_pSkLoader->getAttr(id);
    if(!pAttr)
    {
        emit meetError(UB_COMPONENTATTR_NOT_FOUND,QString("no (%1:%2) ComponentAttr found!").arg(ENUM_NAME(id)).arg(id));
        return false;
    }
    if(pAttr->needVertical())
    {
        pWidget->setOrientation(Qt::Vertical);
    }
    else
    {
        pWidget->setOrientation(Qt::Horizontal);
    }

    return setWidgetAttr(id, pWidget);
}

bool UiBuilder::setWidgetAttr(SkinLoader::ComponentID id, QWidget* pWidget)
{
    if(!pWidget)
    {
        emit meetError(UB_WIDGET_POINTER_IS_NULL,QString("can't set widget attribute for widget pointer(%1:%2) is null").arg(ENUM_NAME(id)).arg(id));
        return false;
    }

    if(!this->m_pSkLoader)
    {

        return false;
    }
    ComponentAttr* pAttr = this->m_pSkLoader->getAttr(id);

    if(!pAttr)
    {
        emit meetError(UB_COMPONENTATTR_NOT_FOUND,QString("no (%1:%2) ComponentAttr found!").arg(ENUM_NAME(id)).arg(id));
        return false;
    }
    pAttr->dump();
    //logtrace("here(%s)", qPrintable(pAttr->name()));
    pWidget->setObjectName(pAttr->name());

    pWidget->move(pAttr->x(), pAttr->y());

    pWidget->resize(pAttr->width(), pAttr->height());

    if(pAttr->hasMask())
    {
        QImage maskImg;
        if(pAttr->hasMaskColor())
        {
            QImage srcImg(pAttr->getMaskPath());
            if(pAttr->isMaskOut())
            {
                maskImg = srcImg.createMaskFromColor(QColor(pAttr->getMaskColor()).rgb(),Qt::MaskOutColor);
            }
            else
            {
                maskImg = srcImg.createMaskFromColor(QColor(pAttr->getMaskColor()).rgb(),Qt::MaskInColor);
            }
            logonlytrace;
        }
        else
        {
            maskImg = QImage(pAttr->getMaskPath());
            logonlytrace;
        }
       // maskImg.save(pAttr->name()+"_mask.bmp");
        pWidget->setMask(QBitmap::fromImage(maskImg));
    }
    pWidget->setDisabled(pAttr->needDisable());
    pWidget->setVisible(!pAttr->needHidden());
    pWidget->setStyleSheet(pAttr->css());
    pWidget->setToolTip(pAttr->toolTip());

    return true;
}

bool UiBuilder::reloadSkin()
{
    if(!this->m_pSkLoader->reload())
    {
        emit meetError(UB_RELOAD_SKIN_ERROR, QString("reload skin error! path(%1)").arg(this->m_pSkLoader->skinFilePath()));    
        return false;
    }

    qApp->setStyleSheet(this->m_pSkLoader->getAttr(SkinLoader::ALL_ELEMENTS)->css());

    int i = 0;
    for(i = SkinLoader::ENUM_FLOOR_BTN; i < SkinLoader::ENUM_CELL_BTN; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setWidgetAttr((SkinLoader::ComponentID)i, getBtn((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getBtn").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }

    for(i = SkinLoader::ENUM_FLOOR_SLD; i < SkinLoader::ENUM_CELL_SLD; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setSliderAttr((SkinLoader::ComponentID)i, getSlider((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getSlider").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }

    for(i = SkinLoader::ENUM_FLOOR_LAB; i < SkinLoader::ENUM_CELL_LAB; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setWidgetAttr((SkinLoader::ComponentID)i, getLabel((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getLabel").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }

    for(i = SkinLoader::ENUM_FLOOR_TABLE; i < SkinLoader::ENUM_CELL_TABLE; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setWidgetAttr((SkinLoader::ComponentID)i, getTable((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getTabel").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }
    
    for(i = SkinLoader::ENUM_FLOOR_TAB; i < SkinLoader::ENUM_CELL_TAB; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setWidgetAttr((SkinLoader::ComponentID)i, getTabWidget((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getTab").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }                    

    for(i = SkinLoader::ENUM_FLOOR_DP; i < SkinLoader::ENUM_CELL_DP; i++)
    {
        if(this->m_widgets[i])
        {
            if(!setWidgetAttr((SkinLoader::ComponentID)i, getDisplay((SkinLoader::ComponentID)i)))
            {
                emit meetError(UB_UPDATE_SKIN_FOR_COMPONENT_FAIL,QString("when update skin for (%1:%2) failed!getDisplay").arg(ENUM_NAME(i)).arg(i));
                return false;
            }
        }
    }

    emit skinChanged();
    return true;
}

















