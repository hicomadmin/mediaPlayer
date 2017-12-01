#ifndef _SKINLOADER_H_
#define _SKINLOADER_H_

#include <QObject>
#include <QVector>
#include "ComponentAttr.h"
#include "Log.h"

class SkinLoader : public QObject
{
    Q_OBJECT
public:

    SkinLoader(const QString& strFilePath, QObject *parent = 0);
    virtual bool loadSkinFromFile() = 0;
    virtual ~SkinLoader();
    virtual void destroy();

    enum ComponentID
    {        
        ENUM_FLOOR,

        ENUM_FLOOR_BTN,        
        BTN_PAUSE,
        BTN_PLAY,
        BTN_STOP,        
        BTN_OPEN,
        BTN_PREV,
        BTN_NEXT,
        BTN_LYRIC,
        BTN_MUSIC,
        BTN_MUTE,
        BTN_SINGER,
        BTN_MINIMIZE,
        BTN_MINIMODE,
        BTN_EXIT,
        BTN_RELOADSKIN,
        ENUM_CELL_BTN,

        ENUM_FLOOR_SLD,
        SLD_VOLUM,
        SLD_PROCESS,
        ENUM_CELL_SLD,

        ENUM_FLOOR_LAB,
        LAB_NUMBER,
        LAB_PLAYINFO,
        LAB_USERNAME,
        LAB_SHOWINFO,
        ENUM_CELL_LAB,
        
        ENUM_FLOOR_TABLE,
        TABLE_PLAYLIST,
        ENUM_CELL_TABLE,        
        
        ENUM_FLOOR_TAB,
        TAB_PLAYLIST,
        ENUM_CELL_TAB,
                
        ENUM_FLOOR_DP,
        DP_PLAY,
        DP_WEBFORM,
        ENUM_CELL_DP,

        ALL_ELEMENTS,
        
        ENUM_CELL,
        /*titlebar.* */
        BT_MENU_N,
        BT_MENU_P,
        BT_MINIMIZE_N,
        BT_MINIMIZE_P,
        BT_CLOSE_N,
        BT_CLOSE_P,
        BT_SHADE1_N,
        BT_SHADE1_P,
        BT_SHADE2_N,
        BT_SHADE2_P,
        BT_CLOSE_SHADED_N,
        BT_CLOSE_SHADED_P,

        /* posbar.* */
        BT_POSBAR_N,
        BT_POSBAR_P,
        /* pledit.* */
        PL_BT_ADD,
        PL_BT_SUB,
        PL_BT_SEL,
        PL_BT_SORT,
        PL_BT_LST,
        PL_BT_SCROLL_N,
        PL_BT_SCROLL_P,
        PL_BT_CLOSE_N,
        PL_BT_CLOSE_P,
        PL_BT_SHADE1_N,
        PL_BT_SHADE1_P,
        PL_BT_SHADE2_N,
        PL_BT_SHADE2_P,

        /* eqmain.* */
        EQ_BT_BAR_N,
        EQ_BT_BAR_P,
        EQ_BT_ON_N,
        EQ_BT_ON_P,
        EQ_BT_OFF_N,
        EQ_BT_OFF_P,
        EQ_BT_PRESETS_N,
        EQ_BT_PRESETS_P,
        EQ_BT_AUTO_1_N,
        EQ_BT_AUTO_1_P,
        EQ_BT_AUTO_0_N,
        EQ_BT_AUTO_0_P,
        EQ_BT_CLOSE_N,
        EQ_BT_CLOSE_P,
        EQ_BT_SHADE1_N,

        /* eq_ex.* */
        EQ_BT_SHADE1_P,
        EQ_BT_SHADE2_N,
        EQ_BT_SHADE2_P,

        /* shufrep.* */
        BT_EQ_ON_N,
        BT_EQ_ON_P,
        BT_EQ_OFF_N,
        BT_EQ_OFF_P,
        BT_PL_ON_N,
        BT_PL_ON_P,
        BT_PL_OFF_N,
        BT_PL_OFF_P,
        BT_PL_CLOSE_N,
        BT_PL_CLOSE_P,
        REPEAT_ON_N,
        REPEAT_ON_P,
        REPEAT_OFF_N,
        REPEAT_OFF_P,
        SHUFFLE_ON_N,
        SHUFFLE_ON_P,
        SHUFFLE_OFF_N,
        SHUFFLE_OFF_P,
        /* volume.* */
        BT_VOL_N,
        BT_VOL_P,
        /* balance.* */
        BT_BAL_N,
        BT_BAL_P,

        TITLEBAR_A = 0,
        TITLEBAR_I,
        TITLEBAR_SHADED_A,
        TITLEBAR_SHADED_I,

        PL_CORNER_UL_A = 0,
        PL_CORNER_UL_I,
        PL_CORNER_UR_A,
        PL_CORNER_UR_I,
        PL_TITLEBAR_A,
        PL_TITLEBAR_I,
        PL_TFILL1_A,
        PL_TFILL1_I,
        PL_TFILL2_A,
        PL_TFILL2_I,
        PL_LFILL,
        PL_RFILL,
        PL_LSBAR,
        PL_RSBAR,
        PL_SFILL1,
        PL_SFILL2,
        PL_CONTROL,
        PL_TITLEBAR_SHADED1_A,
        PL_TITLEBAR_SHADED1_I,
        PL_TITLEBAR_SHADED2,
        PL_TFILL_SHADED,

        EQ_MAIN = 0,
        EQ_TITLEBAR_A,
        EQ_TITLEBAR_I,
        EQ_GRAPH,
        EQ_TITLEBAR_SHADED_A,
        EQ_TITLEBAR_SHADED_I,
        EQ_VOLUME1,
        EQ_VOLUME2,
        EQ_VOLUME3,
        EQ_BALANCE1,
        EQ_BALANCE2,
        EQ_BALANCE3,

        MONO_A = 0,
        MONO_I,
        STEREO_A,
        STEREO_I,

        PLAY = 0,
        PAUSE,
        STOP,

        NORMAL = 0,
        EQUALIZER,
        WINDOW_SHADE,
        EQUALIZER_WS,


    };

    bool hasAttr(ComponentID id);
    ComponentAttr* getAttr(ComponentID id);
    bool setAttr(ComponentID id, ComponentAttr* pNewAttr);

    QString skinName();
    SkinAuthor* skinAuthor();

    QString skinFilePath();
    void setSkinFilePath(const QString & strFilePath);

    bool reload();

signals:
    void meetError(ErrorType, const QString & strWhy);

protected:
    void setSkinName(const QString &strSkinName);
   // void setSkinAuthor(SkinAuthor * pSkinAuthor);

private:
    QString m_skinFilePath;
    QString m_skinName;
    QVector<ComponentAttr*> m_componentAttrs;
    SkinAuthor *m_skinAuthor;
};

#endif //_SKINLOADER_H_
