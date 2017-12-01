#ifndef _LYRICLINE_H_
#define _LYRICLINE_H_

#include <QtGui>

class LyricLine : public QWidget 
{
Q_OBJECT
public:
    LyricLine(const QString & strText,QWidget *parent = 0);
    
    //void paintEvent(QPaintEvent *event);
    
    QSize sizeHint() const;
    
    void setText(const QString & strText);
    QString text() const;
    
    void setFont(const QFont & font);
    QFont font() const;
    
    //QPainterPath maskPath();
    
    QGradient* srcGradient() const;
    void setSrcGradient(QGradient * pGradient);

    QGradient* maskGradient() const;
    void setMaskGradient(QGradient * pGradient);
        
    void buildSrcImg();
    void buildMaskImg();


    void setSrcPen(const QPen & strPen);
    void setMaskPen(const QPen & dstPen);

    public slots:
    void buildResultImg(float fPersent);

    
private:
    QString m_strText;
    QFont m_font;
    QPen m_srcPen;
    QPen m_maskPen;
    QGradient* m_pSrcGradient;
    QGradient* m_pMaskGradient;
    QImage* m_pSrcImg;
    QImage* m_pMaskImg;
    QLabel* m_pLabel;
};

#endif //_LYRICLINE_H_
