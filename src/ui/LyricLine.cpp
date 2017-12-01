#include "LyricLine.h"
#include <assert.h>
#include <stdio.h>
#include <core/Log.h>
LyricLine::LyricLine(const QString & strText,QWidget *parent/* = 0*/)
    :QWidget(parent),m_strText(strText),m_pSrcGradient(0),m_pMaskGradient(0),m_pSrcImg(0),m_pMaskImg(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pLabel =  new QLabel(m_strText);  

    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    this->layout()->addWidget(m_pLabel);
    
    m_font = QFont("ºÚÌå");
    m_font.setPointSize(30);
    m_font.setBold(true);
    m_font.setStyleStrategy(QFont::ForceOutline);
    
    m_srcPen = QPen(QColor("indianred"), 0, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin);
    m_maskPen = QPen(QColor("lightsalmon"), 0, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin);
    
    m_pSrcGradient = new QLinearGradient(0, 0, 0, 100);
    m_pSrcGradient->setColorAt(0.0, QColor("red"));
    m_pSrcGradient->setColorAt(1.0, QColor("papayawhip"));
    
    m_pMaskGradient = new QLinearGradient(0, 0, 0, 100);
    m_pMaskGradient->setColorAt(0.0, QColor("gold"));
    m_pMaskGradient->setColorAt(1.0, QColor("peru"));
    
    buildSrcImg();
    
    buildMaskImg();
    
    buildResultImg(0);
    
}
/*
void LyricLine::paintEvent(QPaintEvent *event)
{

}*/

void LyricLine::setText(const QString & strText)
{
    this->m_strText = strText;
    //QLabel::setText(m_strText);
   // this->layout()->removeWidget(this->m_pLabel);
  //  delete this->m_pLabel;
  //  this->m_pLabel = new QLabel(m_strText);
   // this->m_pLabel->clear();
   this->m_pLabel->setText(this->m_strText);
   // this->m_pLabel->setBackgroundRole(QPalette::Background);
    //this->m_pLabel->setDisabled(true);
   //  this->layout()->addWidget(m_pLabel);
    //this->m_pLabel->setText(m_strText);
    //QPixmap pixmap;
    buildSrcImg();
    // buildMaskImg();
    buildResultImg(0);
    //this->m_pLabel->setEnabled(true);
}

QString LyricLine::text() const
{
    return this->m_strText;
}

QSize LyricLine::sizeHint() const
{
    QFontMetrics fontMetrics(m_font);
    QSize size = fontMetrics.size(Qt::TextExpandTabs, text());
    return size*1;
}

QGradient* LyricLine::srcGradient()  const
{
    return m_pSrcGradient;
}

void LyricLine::setSrcGradient(QGradient * pGradient)
{
    m_pSrcGradient = pGradient;
}

QGradient* LyricLine::maskGradient() const
{
    return m_pMaskGradient;
}

void LyricLine::setMaskGradient(QGradient * pGradient)
{
    m_pMaskGradient = pGradient;
}


void LyricLine::buildSrcImg()
{
    static int i = 0;

    QImage* pSrcImg = new QImage(sizeHint(), QImage::Format_ARGB32_Premultiplied);

    QString str0("0src%1.bmp");
    str0 = str0.arg(i);
   // pSrcImg->save(str0);

    pSrcImg->fill(0);

    QString str1("1src%1.bmp");
    str1 = str1.arg(i);
  //  pSrcImg->save(str1);

    // QBrush bgBrush(Qt::NoBrush);
    QPainter painter(pSrcImg);
    painter.setBackgroundMode(Qt::TransparentMode);
  //  painter.setBackground(bgBrush);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(m_srcPen);
    painter.setBrush(*m_pSrcGradient);
    
    QString str2("2src%1.bmp");
    str2 = str2.arg(i);
 //   pSrcImg->save(str2);

    QPainterPath textPath;    
    textPath.addText(0, 36, m_font, text());  
    textPath.setFillRule(Qt::WindingFill);//(Qt::OddEvenFill);
    painter.drawPath(textPath);    
    
    QString str3("3src%1.bmp");
    str3 = str3.arg(i);
 //   pSrcImg->save(str3);

    QImage* pBak = m_pSrcImg;
    m_pSrcImg = pSrcImg;
    m_pLabel->setPixmap(QPixmap::fromImage(*pSrcImg));
    if(pBak)
    {
        delete pBak;     
    }
    QString str4("4src%1.bmp");
    str4 = str4.arg(i);
 //   pSrcImg->save(str4);
    //buildResultImg(0);
    i++;
}

void LyricLine::buildMaskImg()
{

    QImage* pMaskImg = new QImage(QSize(2048, sizeHint().height()), QImage::Format_ARGB32_Premultiplied);

    QPainter painter(pMaskImg);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(m_maskPen);
    painter.setBrush(*m_pMaskGradient);

    QPainterPath rectPath;
    rectPath.moveTo(0, 0);
    rectPath.lineTo(2048, 0);//sizeHint().width()
    rectPath.lineTo(2048, sizeHint().height());//sizeHint().width()
    rectPath.lineTo(0, sizeHint().height());
    rectPath.closeSubpath();
    rectPath.setFillRule(Qt::OddEvenFill);  
    painter.drawPath(rectPath);

    QImage* pBak = m_pMaskImg;
    m_pMaskImg = pMaskImg;
    if(pBak)
    {
        delete pBak;     
    }    
}

void LyricLine::buildResultImg(float fPersent)
{
  /*  QPixmap pixmap;
    pixmap = QPixmap(2048, sizeHint().height()*1.5);
    pixmap.fill(Qt::transparent);
m_pLabel->setPixmap(pixmap);
m_pLabel->setPixmap(QPixmap::fromImage(*m_pSrcImg));
*/

    int width = sizeHint().width() * fPersent;
    QPixmap pixmap;
    pixmap = QPixmap(2048, sizeHint().height()*1.5);
    pixmap.fill(Qt::transparent);
    m_pLabel->setPixmap(pixmap);
    QImage resultImage(sizeHint(), QImage::Format_ARGB32_Premultiplied);
    QImage maskImg = m_pMaskImg->copy(0, 0, width,sizeHint().height());
    
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, maskImg);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    painter.drawImage(0, 0, *m_pSrcImg);
    //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //painter.fillRect(resultImage.rect(), Qt::NoBrush);
    painter.end();
    logtrace("persent %d --> %f", width,fPersent);
    m_pLabel->setPixmap(QPixmap::fromImage(resultImage));

}


