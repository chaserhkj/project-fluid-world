#include "displaywidget.h"
#include <QtGui>
#include <cmath>

DisplayWidget::DisplayWidget(QWidget * parent, double position): \
    QWidget(parent, Qt::Window), closeFlag(false), view(this)
{
    this->setWindowTitle(tr("Graph"));

    lo.addWidget(&view);
    this->setLayout(&lo);
    this->setGeometry(100, 100, 500, 500);

    view.setScene(&scene);
    view.setSceneRect(-200, -200, 400, 400);
    double x = sin(position) * 150;
    double y = cos(position) * 150;
    line = scene.addLine(0, 0, x, y);
    ellipse = scene.addEllipse(x - 5, y - 5, 10, 10, QPen(), QBrush(QColor("Black")));
}

void DisplayWidget::changeGraph(double position)
{
    delete line;
    delete ellipse;
    double x = sin(position) * 150;
    double y = cos(position) * 150;
    line = scene.addLine(0, 0, x, y);
    ellipse = scene.addEllipse(x - 5, y - 5, 10, 10, QPen(), QBrush(QColor("Black")));
}

void DisplayWidget::setCloseFlag()
{
    closeFlag = true;
}

void DisplayWidget::closeEvent(QCloseEvent * event)
{
    if (closeFlag)
        event->accept();
    else
        event->ignore();
}
