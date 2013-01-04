#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#include <QGLWidget>
class QWidget;

class DisplayWidget: public QGLWidget
{
public:
    DisplayWidget(QWidget * parent = 0);
    virtual ~ DisplayWidget();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif /* _DISPLAYWIDGET_H_ */
