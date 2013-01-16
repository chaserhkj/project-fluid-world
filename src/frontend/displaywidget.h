#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#include <QGraphicsView>
#include "calthread.h"
class QGraphicsScene;

class DisplayWidget : public QGraphicsView
{
    Q_OBJECT;
public:
    DisplayWidget(QWidget * parent = 0);
    virtual ~DisplayWidget();
    int currentIndex();
    double displayFactor();
public slots:
    bool setCurrentIndex(int i);
    bool setDisplayFactor(double f);
    void updateData();
    void updateGraph();
    void clear();
private:
    QList<spotStainTable> data;
    int index;
    double factor;
    QGraphicsScene * scene;
    void drawBackGround();
};

#endif /* _DISPLAYWIDGET_H_ */
