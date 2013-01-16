#include "displaywidget.h"

#ifdef OPENGL_ENABLED
#include <QtOpenGL>
#else /* OPENGL_ENABLED */
#include <QtGui>
#endif /* OPENGL_ENABLED */

#include "projectmainwindow.h"
#include "calthread.h"

DisplayWidget::DisplayWidget(QWidget * parent) : QGraphicsView(parent), index(-1), factor(10.0)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
#ifdef OPENGL_ENABLED
    this->setViewport(new QGLWidget());
#endif /* OPENGL_ENABLED */
}

//Get data from calThread and store them locally.
void DisplayWidget::updateData()
{
    CalThread * thd =
        qobject_cast<ProjectMainWindow * >(this->parent())->getThread();

    while (!thd->queueIsEmpty()){
        spotStainTable tb = thd->getData();        
        data.append(tb);
    }
}

//Update graph according to the current index value and display factor value.
void DisplayWidget::updateGraph()
{
    if (index < 0 || index >= data.length())
        return;
    this->drawBackGround();
    foreach(QList<QPointF> line, data[index]) {
        QPainterPath path;
        path.moveTo(line[0].x() * factor,
                    -line[0].y() * factor);
        foreach(QPointF p, line)
            path.lineTo(p.x() * factor,
                        -p.y() * factor);
        scene->addPath(path);
    }
}

//Clear scene and draw background items.
void DisplayWidget::drawBackGround()
{
    scene->clear();
    scene->addEllipse(-1*factor,
                      -1*factor,
                      2*factor,
                      2*factor);
}

DisplayWidget::~DisplayWidget()
{
}

int DisplayWidget::currentIndex()
{
    return index;
}

double DisplayWidget::displayFactor()
{
    return factor;
}

//Checks for out-of-index exceptions.
//Returns false if out-of-index happens.
bool DisplayWidget::setCurrentIndex(int i)
{
    if (i<0 || i>=data.length())
        return false;
    index = i;
    //Update graph
    this->updateGraph();
    return true;
}

//Checks if input value is positive.
//If not, returns false.
bool DisplayWidget::setDisplayFactor(double f)
{
    if (f <= 0)
        return false;
    factor = f;
    //Update Graph
    this->updateGraph();
    return true;
}

void DisplayWidget::clear()
{
    scene->clear();
    data.clear();
    index = -1;
}
