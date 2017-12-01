#ifndef UIBUILDER_H
#define UIBUILDER_H

#include <QObject>
#include <QMainWindow>
#include <QPoint>
#include <QSize>
#include <QMouseEvent>
#include <QWidget>
#include <QVector>

#include <core/Log.h>
#include <core/SkinLoader.h>

class SoundCore;
class QPushButton;
class QWidget;
class QLabel;
class QSlider;
class QTableWidget;
class QTabWidget;
class QImage;

class DockWidget : public QMainWindow
{
    Q_OBJECT
public:
    DockWidget(QWidget *parent = 0);
    void addDockWidget(QWidget* pWidget);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint dragPosition;
    QVector<QWidget*>* m_pFriends;
};

class UiBuilder : public QObject
{
    Q_OBJECT
public:
    UiBuilder(SkinLoader* pSkLoader);

    void doMain();
    QMainWindow* getDP_WebForm();
    QMainWindow* getDP_Play();
    QWidget* getDP_KTVLyric();

    QPushButton* getBtn(SkinLoader::ComponentID id, QWidget* pParent = 0);
    QMainWindow* getDisplay(SkinLoader::ComponentID id, QWidget* pParent = 0);
    QSlider* getSlider(SkinLoader::ComponentID id, QWidget* pParent = 0);
    QLabel* getLabel(SkinLoader::ComponentID id, QWidget* pParent = 0);
    QTabWidget* getTabWidget(SkinLoader::ComponentID id, QWidget* pParent = 0);
    QTableWidget* getTable(SkinLoader::ComponentID id, QWidget* pParent = 0);
private:
    bool setSliderAttr(SkinLoader::ComponentID id, QSlider* pWidget);
    bool setWidgetAttr(SkinLoader::ComponentID id, QWidget* pWidget);
signals:
    void meetError(ErrorType errorType, const QString & strWhy);
    void skinChanged();
public slots:
    bool reloadSkin();

    void dealCommonError(ErrorType errorType,const QString& why);
private:
    SkinLoader* m_pSkLoader;
    SoundCore* m_pSound;
    QVector<QWidget*> m_widgets;
    QLabel *m_pShowInfo;
    QWidget* pKTVDisplay;
};

#endif // UIBUILDER_H
