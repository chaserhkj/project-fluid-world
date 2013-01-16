#include "displaywidget.h"

#ifdef OPENGL_ENABLED
#include <QtOpenGL>
#else /* OPENGL_ENABLED */
#include <QtGui>
#endif /* OPENGL_ENABLED */

#include "projectmainwindow.h"
#include "calthread.h"

#ifdef OPENGL_ENABLED

DisplayWidget::DisplayWidget(QWidget * parent): QGLWidget(parent)
{
}

void DisplayWidget::updateGraph()
{
    CalThread * thd = qobject_cast<ProjectMainWindow *>(this->parent())->getThread();
    if (tbd->queueIsEmpty())
        return;
    spotStainTable tb = thd->getData();
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    foreach(QList<QPointF> line, tb) {
        glBegin(GL_LINES);
        foreach(QPointF p, line)
            glVertex3f(p.x(),p.y(),0.0);
        glEnd();
        glFlush();
        }
    }
}

void DisplayWidget::initializeGL()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glScalef(2.0/25,1.0/5,1.0);
    glTranslatef(-7.5,0.0,0.0);
}

void DisplayWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
}

void DisplayWidget::resizeGL(int w, int h)
{
    
}
#else /* OPENGL_ENABLED */

DisplayWidget::DisplayWidget(QWidget * parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

void DisplayWidget::updateGraph()
{
    CalThread * thd = qobject_cast<ProjectMainWindow * >(this->parent())->getThread();
    if (thd->queueIsEmpty())
        return;
    spotStainTable tb = thd->getData();
    scene->clear();
    foreach(QList<QPointF> line, tb) {
        QPainterPath path;
        path.moveTo(line[0].x()*10, line[0].y()*10);
        foreach(QPointF p, line)
            path.lineTo(p.x()*10, p.y()*10);
        scene->addPath(path);
    }
    
}

#endif /* OPENGL_ENABLED */

DisplayWidget::~DisplayWidget()
{
}

