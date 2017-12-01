#ifndef LGSKINLOADER_H
#define LGSKINLOADER_H

#include <QDomNode>
#include "SkinLoader.h"

class LGSkinLoader : public SkinLoader
{
    Q_OBJECT
public:
    LGSkinLoader(const QString& strFilePath, QObject *parent = 0);
    bool loadSkinFromFile();
private:

    bool setAuthor(const QDomNode & authorNode);
    bool setAllElements(const QDomNode & allNode);
    bool setButtons(const QDomNode & buttonsNode);
    bool setSliders(const QDomNode & slidersNode);
    bool setLabs(const QDomNode & labsNode);
    bool setTabs(const QDomNode & tabsNode);
    bool setTables(const QDomNode & tablesNode);
    bool setDisplays(const QDomNode & displaysNode);
    bool getElemText(const QDomNode & node, QString &strTo);
    bool addComponentAttr(const QDomNode & btnNode, ComponentID id);
};

#endif // LGSKINLOADER_H
