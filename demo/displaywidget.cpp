#include "displaywidget.h"
#include <QtGui>
#include <cmath>

DisplayWidget::DisplayWidget(QWidget *parent, double position):\
    QWidget(parent, Qt::Window), closeFlag(false), view(this)
{
    this->setWindowTitle(tr("Graph"));
    
    lo.addWidget(&view);
    this->setLayout(&lo);
    this->setGeometry(0, 0, 500, 500);

    view.setScene(&scene);
    view.centerOn(0, 0);
    double x = sin(position) * 150;
    double y = cos(position) * 150;
    line = scene.addLine(0, 0, x, y);
}

void DisplayWidget::changeGraph(double position, double velocity)
{
    delete line;
    double x = sin(position) * 150;
    double y = cos(position) * 150;
    line = scene.addLine(0, 0, x, y);
}

void DisplayWidget::setCloseFlag()
{
    closeFlag = true;
}

void DisplayWidget::closeEvent(QCloseEvent *event)
{
    if(closeFlag)
	event->accept();
    else
	event->ignore();
}
