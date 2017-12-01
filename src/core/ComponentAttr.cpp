#include "ComponentAttr.h"

#include "Log.h"
ComponentAttr::ComponentAttr(const QString & strName)
    :m_name(strName)
{
    m_x = m_y = m_width = m_height = 0;
    m_hidden = m_disable = m_vertical = false;
}


void ComponentAttr::setXY(int x, int y)
{
    this->m_x = x;
    this->m_y = y;
}

void ComponentAttr::setSize(int w, int h)
{
    this->m_width = w;
    this->m_height = h;
}

void ComponentAttr::setHidden(bool bHidden)
{
    this->m_hidden = bHidden;
}

void ComponentAttr::setDisable(bool bDisable)
{
    this->m_disable = bDisable;
}

void ComponentAttr::setVertical(bool bVertical)
{
    this->m_vertical = bVertical;
}

void ComponentAttr::setName(const QString & strName)
{
    this->m_name = strName;
}

void ComponentAttr::setCss(const QString & strCss)
{
    this->m_css = strCss;
}

void ComponentAttr::setToolTip(const QString & strToolTip)
{
    this->m_toolTip = strToolTip;
}

void ComponentAttr::setMaskImgPath(const QString & strImgPath)
{
    this->m_maskImgPath = strImgPath;
}
 void ComponentAttr::setMaskColor(const QString & strMaskColor)
 {
     this->m_maskColor = strMaskColor;
 }

void ComponentAttr::setMaskOut(bool bOut/* = true*/)
{
    this->m_isMaskOut = bOut;
}
int ComponentAttr::x()
{
    return m_x;
}

int ComponentAttr::y()
{
    return m_y;
}

int ComponentAttr::width()
{
    return m_width;
}

int ComponentAttr::height()
{
    return m_height;
}

bool ComponentAttr::needHidden()
{
    return m_hidden;
}

bool ComponentAttr::needDisable()
{
    return m_disable;
}

bool ComponentAttr::needVertical()
{
    return m_vertical;
}

QString ComponentAttr::name()
{
    return this->m_name;
}

QString ComponentAttr::css()
{
    return this->m_css;
}

QString ComponentAttr::toolTip()
{
    return this->m_toolTip;
}

bool ComponentAttr::hasMask()
{
    return !m_maskImgPath.isEmpty();
}

QString ComponentAttr::getMaskPath()
{
    return this->m_maskImgPath;
}

bool ComponentAttr::hasMaskColor()
{
    return !m_maskColor.isEmpty();
}

QString ComponentAttr::getMaskColor()
{
    return this->m_maskColor;
}

bool ComponentAttr::isMaskOut()
{
    return this->m_isMaskOut;
}

void ComponentAttr::dump()
{
    logoutput("+++++++++++++++++++++++++++++++++++++++++++++++\n");
    logoutput("x=(%d)", m_x);
    logoutput("y=(%d)", m_y);
    logoutput("width=(%d)", m_width);
    logoutput("height=(%d)", m_height);
    logoutput("hidden=(%d)", m_hidden);
    logoutput("disable=(%d)", m_disable);
    logoutput("vertical=(%d)", m_vertical);
    logoutput("name=(%s)", qPrintable(m_name));
    logoutput("css=(%s)", qPrintable(m_css));
    logoutput("toolTip=(%s)", qPrintable(m_toolTip));
    logoutput("maskImgPath=(%s)", qPrintable(m_maskImgPath));
    logoutput("maskColor=(%s)", qPrintable(m_maskColor));
    logoutput("maskOut=(%d)", m_isMaskOut);
    logoutput("+++++++++++++++++++++++++++++++++++++++++++++++\n");
}
