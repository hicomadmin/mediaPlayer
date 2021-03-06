#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QWidgetItem>
#include <QVector>

class FlowLayout : public QLayout
{
public:
    FlowLayout(QWidget *parent, int margin = -1, int spacing = 0);
    FlowLayout(int spacing = 0);
    ~FlowLayout();

    void addItem(QLayoutItem *item);
    void setIndexRight(int nIndex, bool isRight = false);
    Qt::Orientations expandingDirections() const;
    bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QSize minimumSize() const;
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QLayoutItem *takeAt(int index);

private:
    int doLayout(const QRect &rect, bool testOnly) const;

    QList<QLayoutItem *> itemList;
    QVector<bool> toRight;
};

#endif

