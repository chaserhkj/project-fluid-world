#ifndef _DISPLAYWIDGET_H_
#define _DISPLAYWIDGET_H_

#ifdef OPENGL_ENABLED

#include <QGLWidget>
class QWidget;

class DisplayWidget: public QGLWidget
{
    Q_OBJECT;
public:
    DisplayWidget(QWidget * parent = 0);
    virtual ~ DisplayWidget();
public slots:
    void updateGraph();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#else /* OPENGL_ENABLED */

#include <QGraphicsView>
class QGraphicsScene;

class DisplayWidget: public QGraphicsView
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

#endif /* OPENGL_ENABLED */

#endif /* _DISPLAYWIDGET_H_ */
