#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

class QCloseEvent;
class QGraphicsLineItem;
class QGraphicsEllipseItem;

class DisplayWidget: public QWidget
{

    Q_OBJECT;

public:
    DisplayWidget(QWidget * parent = 0, double position = 0);
    void setCloseFlag();
public slots:
    void changeGraph(double positon);
protected:
    void closeEvent(QCloseEvent * event);
private:
    bool closeFlag;
    QVBoxLayout lo;
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsLineItem * line;
    QGraphicsEllipseItem * ellipse;
};

#endif /* _DISPLAYWIDGET_H_ */
