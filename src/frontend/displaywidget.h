#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#include <QGraphicsView>
class QGraphicsScene;

class DisplayWidget : public QGraphicsView
{
    Q_OBJECT;
public:
    DisplayWidget(QWidget * parent = 0);
    virtual ~DisplayWidget();
public slots:
    void updateGraph();
private:
    QGraphicsScene * scene;
};

#endif /* _DISPLAYWIDGET_H_ */
