#ifndef COMPONENTATTR_H
#define COMPONENTATTR_H

#include <QString>

struct SkinAuthor {
    QString name;
    QString email;
    QString url;
    QString version;
    QString comment;
};

class ComponentAttr {

public:

    ComponentAttr(const QString & strName = "");

    void setXY(int x, int y);
    void setSize(int w, int h);
    void setHidden(bool bHidden = true);
    void setDisable(bool bDisable = true);
    void setVertical(bool bVertical = true);

    void setName(const QString & strName);
    void setCss(const QString & strCss);
    void setToolTip(const QString & strToolTip);
    void setMaskImgPath(const QString & strImgPath);
    void setMaskColor(const QString & strMaskColor);
    void setMaskOut(bool bOut = true);
    int x();

    int y();

    int width();

    int height();

    bool needHidden();

    bool needDisable();

    bool needVertical();

    QString name();

    QString css();

    QString toolTip();

    bool hasMask();

    QString getMaskPath();

    bool hasMaskColor();

    QString getMaskColor();

    bool isMaskOut();

    void dump();



private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_disable;
    bool m_hidden;
    bool m_vertical;
    bool m_isMaskOut;
    QString m_name;
    QString m_css;
    QString m_toolTip;
    QString m_maskImgPath;
    QString m_maskColor;
};

#endif // COMPONENTATTR_H
