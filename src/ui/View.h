#include <QtGui>
class LyricLine;
class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent = 0);

public slots:
    void startNewLyric(const QString & str, bool founded);
    void updateProcess(long begin, long cur, long end);
    void nextStep();
    void nextSentence();
    void playCurLyric();
    void pauseCurLyric();
    void stopCurLyric();
    void showOneLyricInfo(const QString & str);
private:
    QWidget *m_widget;
    QGraphicsScene *m_scene;
    QGraphicsProxyWidget *m_item;
    LyricLine* m_pLabelUp;
    LyricLine* m_pLabelDown;
    bool m_lastFillUp;
    QPushButton* m_pBtn;
    QWidget *m_parent;
};

