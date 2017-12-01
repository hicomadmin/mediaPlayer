#include <QtGui/QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QStringList>

#include <core/SkinLoader.h>
#include <core/LGSkinLoader.h>
#include <core/playlistmodel.h>
#include <core/config.h>


#include "connector.h"
#include "uibuilder.h"
#include "Log.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);

    #if defined(USE_LIBARY_WLOG) && (USE_LIBARY_WLOG==1)
    WLog::setLog_File_Xml();
    int nLevel = W_LEVEL_ERROR;
    QStringList args = QApplication::arguments();
    if(args.size() >= 3)
    {
        if(args[1] == "-level")
        {
            bool isOK = false;
            nLevel = args[2].toInt(&isOK, 10);
            if(!isOK)
            {
                nLevel = W_LEVEL_FLOOR;
                logerror((QString("can't convert arg 3 from %1 to int").arg(args[2])).toUtf8().data());
            }
            else
            {
                lognotice((QString("set log level to %1 ").arg(nLevel)).toUtf8().data());
            }
        }
        else
        {
            lognotice((QString("args[1] is not '-level' but %1 ").arg(args[1])).toUtf8().data());
        }
    }
    else
    {
        lognotice((QString("args.size() = %1 and args[0] is %2").arg(args.size()).arg(args[0])).toUtf8().data());
    }

    WLog::getLog()->setOutLevel(nLevel);
    #endif

    UiBuilder builder(new LGSkinLoader(Config::instance()->currentSkinPath()));
    PlayListModel listModel;
    Connector conn(&builder, &listModel);
    conn.connectAll();

    builder.doMain();
    builder.reloadSkin();


    int iRet = a.exec();

    #if defined(USE_LIBARY_WLOG) && (USE_LIBARY_WLOG==1)
        logtrace("exit program with code(%d)", iRet);
        WLog::autoDelLog();
    #endif
    
    return iRet;
}
