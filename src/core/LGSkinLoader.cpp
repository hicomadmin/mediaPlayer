#include <QFile>

#include <QDomDocument>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomNode>
#include <QDomNamedNodeMap>


#include "LGSkinLoader.h"
#include "Log.h"

#define IF_FALSE_RET(exp) do{if(!(exp)){emit meetError(SL_EXP_IS_FALSE,QString("exp(%1) is flase").arg("d")); return false;}}while(0)

#define WHEN_NODE_NAME_EQ_SET_TEXT(node, strName, strTo) do{\
     if(strName == node.nodeName()){\
        IF_FALSE_RET(this->getElemText(node, strTo));\
    }\
    }while(0)

#define IF_NODE_NAME_NOT_EQ_RET(node, strName) do{\
        if(node.nodeName() != strName)\
        {\
            emit meetError(SL_NODE_NAME_NOT_EQ,QString("node name is not (%1) but (%2)!").arg(strName).arg(node.nodeName()));\
            return false;\
        }\
        }while(0)

#define IF_NODE_NAME_EQ_ADD_ATTR(node, strName, id) do{\
            if(strName == node.nodeName()){\
                IF_FALSE_RET(addComponentAttr(node, id));\
            }\
            }while(0)


                   // logtrace("strName(%s) == node.nodeName(%s)",strName,qPrintable(node.nodeName()));\
                   
LGSkinLoader::LGSkinLoader(const QString& strFilePath, QObject *parent/* = 0*/)
    : SkinLoader(strFilePath, parent)
{
    logtrace("strFilePath=(%s) ", qPrintable(strFilePath));
}

bool LGSkinLoader::loadSkinFromFile()
{
    
    QDomDocument doc("mydocument");
    QFile file(this->skinFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        emit meetError(SL_CANT_OPEN_FILE,QString("Can't open file %1").arg(this->skinFilePath()));
        return false;
    }

    QString errorMsg;
    int errorLine = -1, errorColumn = -1;

    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        emit meetError(SL_WHEN_PARSE_MEET_ERROR,QString("Error:%1, at line(%2), column(%3)").arg(errorMsg).arg(errorLine).arg(errorColumn));
        return false;
    }
    file.close();
   // logtrace("errormsg is (%s)", qPrintable(errorMsg));
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement elemDoc = doc.documentElement();
    this->setSkinName(elemDoc.attribute("name", "unknow skin name"));
    QDomNode authorNode = elemDoc.firstChild();
    IF_FALSE_RET(setAuthor(authorNode));
/*do{
    if(!(setAuthor(authorNode)))
    {
        emit meetError(QString("exp(%1) is flase").arg("d"));
        return false;
            }}while(0);*/
    

    QDomNode allNode = elemDoc.firstChild().nextSibling().firstChild();
    IF_NODE_NAME_NOT_EQ_RET(allNode, "AllElements");
    IF_FALSE_RET(this->setAllElements(allNode));
    
    
    QDomNode buttonsNode = allNode.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(buttonsNode, "Buttons");
    IF_FALSE_RET(this->setButtons(buttonsNode));
    
    
    QDomNode sliders = buttonsNode.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(sliders, "Sliders");
    IF_FALSE_RET(this->setSliders(sliders));
    

    QDomNode labs = sliders.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(labs, "Labels");    
    IF_FALSE_RET(this->setLabs(labs));
    

    QDomNode tabs = labs.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(tabs, "Tabs");    
    IF_FALSE_RET(this->setTabs(tabs));
    

    QDomNode tables = tabs.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(tables, "Tables");    
    IF_FALSE_RET(this->setTables(tables));
    
        
    QDomNode displays = tables.nextSibling();
    IF_NODE_NAME_NOT_EQ_RET(displays, "Displays");
    IF_FALSE_RET(this->setDisplays(displays));
    

    return true;
}

bool LGSkinLoader::setAuthor(const QDomNode & authorNode)
{
    IF_NODE_NAME_NOT_EQ_RET(authorNode, "Author");

    QDomNodeList childNodes = authorNode.childNodes();
    int i = 0;
    while(i < childNodes.size())
    {
        WHEN_NODE_NAME_EQ_SET_TEXT(childNodes.at(i), "Name", this->skinAuthor()->name);
        WHEN_NODE_NAME_EQ_SET_TEXT(childNodes.at(i), "Email", this->skinAuthor()->email);
        WHEN_NODE_NAME_EQ_SET_TEXT(childNodes.at(i), "Url", this->skinAuthor()->url);
        WHEN_NODE_NAME_EQ_SET_TEXT(childNodes.at(i), "Version", this->skinAuthor()->version);
        WHEN_NODE_NAME_EQ_SET_TEXT(childNodes.at(i), "Comment", this->skinAuthor()->comment);
        i++;
    }

    return true;
}

bool LGSkinLoader::getElemText(const QDomNode & node, QString & strTo)
{
    QDomElement elem = node.toElement();
    if(elem.isNull())
    {
        emit meetError(SL_CANT_CONVERT_TO_ELE,QString("node(%1) are not element").arg(node.nodeName()));
        return false;
    }

    strTo = elem.text();
//    logtrace("element (%s) has text (%s)", (node.nodeName().toUtf8().data()), strTo.toUtf8().data());
    return true;
}

bool LGSkinLoader::setAllElements(const QDomNode & allNode) 
{
    ComponentAttr *pAttr = new ComponentAttr("AllElements");
    QDomNode childNode = allNode.firstChild();
    if("Css" == childNode.nodeName())
    {
        pAttr->setCss(childNode.toElement().text());
    }
    return this->setAttr(ALL_ELEMENTS, pAttr);
}

bool LGSkinLoader::setButtons(const QDomNode & buttonsNode)
{
    QDomNodeList nodes = buttonsNode.childNodes();
   // logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Play", BTN_PLAY);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Pause", BTN_PAUSE);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Stop", BTN_STOP);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Open", BTN_OPEN);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Prev", BTN_PREV);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Next", BTN_NEXT);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Lyric", BTN_LYRIC);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Music", BTN_MUSIC);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Mute", BTN_MUTE);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Singer", BTN_SINGER);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Minimize", BTN_MINIMIZE);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Minimode", BTN_MINIMODE);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Exit", BTN_EXIT);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "ReloadSkin", BTN_RELOADSKIN);
        i++;
    }

    return true;
}


bool LGSkinLoader::setSliders(const QDomNode & slidersNode)
{
    QDomNodeList nodes = slidersNode.childNodes();
   // logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "VolumSlider", SLD_VOLUM);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "ProcessSlider", SLD_PROCESS);
        i++;
    }

    return true;
}

bool LGSkinLoader::setLabs(const QDomNode & labsNode)
{
    QDomNodeList nodes = labsNode.childNodes();
   // logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Number", LAB_NUMBER);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "PlayInfo", LAB_PLAYINFO);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "UserName", LAB_USERNAME);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "ShowInfo", LAB_SHOWINFO);
        i++;
    }

    return true;
}

bool LGSkinLoader::setTabs(const QDomNode & tabsNode)
{
    QDomNodeList nodes = tabsNode.childNodes();
  //  logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "PlayList", TAB_PLAYLIST);
        i++;
    }

    return true;
}

bool LGSkinLoader::setTables(const QDomNode & tablesNode)
{
    QDomNodeList nodes = tablesNode.childNodes();
    //logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "PlayList", TABLE_PLAYLIST);
        i++;
    }

    return true;
}

bool LGSkinLoader::setDisplays(const QDomNode & displaysNode)
{
    QDomNodeList nodes = displaysNode.childNodes();
   // logtrace("logtrace(nodes.size()) = (%d)",nodes.size());
    int i = 0;
    while(i < nodes.size())
    {
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Play", DP_PLAY);
        IF_NODE_NAME_EQ_ADD_ATTR(nodes.at(i), "Web", DP_WEBFORM);
        
        i++;
    }

    return true;
}

 bool LGSkinLoader::addComponentAttr(const QDomNode & btnNode, ComponentID id)
 {
    QDomNamedNodeMap attrs = btnNode.attributes();
    ComponentAttr *pAttr = new ComponentAttr(attrs.namedItem("name").toAttr().value());
    pAttr->setXY(attrs.namedItem("x").toAttr().value().toInt(),attrs.namedItem("y").toAttr().value().toInt());
    pAttr->setSize(attrs.namedItem("width").toAttr().value().toInt(),attrs.namedItem("height").toAttr().value().toInt());

    if(attrs.contains("hidden"))
    {
        pAttr->setHidden(attrs.namedItem("hidden").toAttr().value().toInt());
    }

    if(attrs.contains("disable"))
    {
        pAttr->setDisable(attrs.namedItem("disable").toAttr().value().toInt());
    }

    if(attrs.contains("vertical"))
    {
        pAttr->setVertical(attrs.namedItem("vertical").toAttr().value().toInt());
    }

    QDomNodeList childrenNodes = btnNode.childNodes();
    int i = 0;
    while(i < childrenNodes.size())
    {
        QDomNode childNode = childrenNodes.at(i);
        if("Css" == childNode.nodeName())
        {
            pAttr->setCss(childNode.toElement().text());
        }
        if("ToolTip" == childNode.nodeName())
        {
            pAttr->setToolTip(childNode.toElement().text());
        }
        if("MaskImage" == childNode.nodeName())
        {
            QDomNamedNodeMap childNodeattrs = childNode.attributes();
            if(childNodeattrs.contains("maskColor"))
            {
                pAttr->setMaskColor(childNodeattrs.namedItem("maskColor").toAttr().value());
            }
            if(childNodeattrs.contains("maskOut"))
            {
                pAttr->setMaskOut(childNodeattrs.namedItem("maskOut").toAttr().value().toInt());
            }
            pAttr->setMaskImgPath(childNode.toElement().text());
        }

        i++;
    }


   // pAttr->dump();

    this->setAttr(id, pAttr);

    return true;
 }








