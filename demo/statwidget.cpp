#include "statwidget.h"
#include <QtGui>

StatWidget::StatWidget(QWidget *parent) : QWidget(parent, Qt::Window), closeFlag(false)
{
    this->setWindowTitle(tr("Status"));
    
    mainlo = new QVBoxLayout();
    lposition = new QLabel(tr("Position:"));
    vposition = new QLabel;
    lvelocity = new QLabel(tr("Velocity:"));
    vvelocity = new QLabel;
    stopbtn = new QPushButton(tr("&Stop"));

    mainlo->addWidget(lposition);
    mainlo->addWidget(vposition);
    mainlo->addWidget(lvelocity);
    mainlo->addWidget(vvelocity);
    mainlo->addWidget(stopbtn);
    this->setLayout(mainlo);

    this->setGeometry(600, 100, 200, 200);

    QObject::connect(stopbtn, SIGNAL(clicked()), this, SIGNAL(stopRequest()));
}

StatWidget::~StatWidget()
{
    delete mainlo;
    delete lposition;
    delete vposition;
    delete lvelocity;
    delete vvelocity;
    delete stopbtn;
}

void StatWidget::setStatus(double position, double velocity)
{
    vposition->setText(QString::number(position));
    vvelocity->setText(QString::number(velocity));
}

void StatWidget::setCloseFlag()
{
    closeFlag = true;
}

void StatWidget::closeEvent(QCloseEvent *event)
{
    if(closeFlag)
	event->accept();
    else
	event->ignore();
}
