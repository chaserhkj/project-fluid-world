#include "displaywidget.h"

#ifdef OPENGL_ENABLED
#include <QtOpenGL>
#else /* OPENGL_ENABLED */
#include <QtGui>
#endif /* OPENGL_ENABLED */

#include "projectmainwindow.h"
#include "calthread.h"

DisplayWidget::DisplayWidget(QWidget * parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
#ifdef OPENGL_ENABLED
    this->setViewport(new QGLWidget());
#endif /* OPENGL_ENABLED */
}

void DisplayWidget::updateGraph()
{
    CalThread * thd =
        qobject_cast<ProjectMainWindow * >(this->parent())->getThread();

    if (thd->queueIsEmpty())
        return;

    spotStainTable tb = thd->getData();
    scene->clear();
    scene->addEllipse(-10, -10, 20, 20);
    foreach(QList<QPointF> line, tb) {
        QPainterPath path;
        path.moveTo(line[0].x() * 10, line[0].y() * 10);
        foreach(QPointF p, line) {
            path.lineTo(p.x() * 10, p.y() * 10);
        }
        scene->addPath(path);
    }
}

DisplayWidget::~DisplayWidget()
{
}
