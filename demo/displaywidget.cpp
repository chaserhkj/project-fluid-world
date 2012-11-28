#include "displaywidget.h"
#include <QtGui>
#include <cmath>

DisplayWidget::DisplayWidget(QWidget *parent, double position) : QWidget(parent), closeFlag(false)
{
    this->setGeometry(0, 0, 300, 300);
    QPainter ptr(this);
    double x = 150 + sin(position) * 140;
    double y = 150 + cos(position) * 140;
    ptr.drawLine(150, 150, x, y);
    ptr.end();
}

void DisplayWidget::changeGraph(double position)
{
    QPainter ptr(this);
    ptr.erase(0, 0, 300, 300);
    double x = 150 + sin(position) * 140;
    double y = 150 + cos(position) * 140;
    ptr.drawLine(150, 150, x, y);
    ptr.end();
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
