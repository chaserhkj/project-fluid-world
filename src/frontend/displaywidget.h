#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#include <QGLWidget>
class QWidget;

class DisplayWidget: public QGLWidget
{
public:
    DisplayWidget(QWidget * parent = 0);
    virtual ~ DisplayWidget();
};

#endif /* _DISPLAYWIDGET_H_ */
