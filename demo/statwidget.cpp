#include "statwidget.h"
#include <QtGui>

StatWidget::StatWidget(QWidget *parent) : QWidget(parent, Qt::Window), closeFlag(false)
{
    this->setWindowTitle(tr("Status"));
    
    mainlo = new QVBoxLayout();
    lposition = new QLabel(tr("Position:(rad)"));
    vposition = new QLabel;
    lvelocity = new QLabel(tr("Velocity:(rad/s)"));
    vvelocity = new QLabel;
    ltime = new QLabel(tr("Time:(sec)"));
    vtime = new QLabel;
    stopbtn = new QPushButton(tr("&Stop"));

    mainlo->addWidget(lposition);
    mainlo->addWidget(vposition);
    mainlo->addWidget(lvelocity);
    mainlo->addWidget(vvelocity);
    mainlo->addWidget(ltime);
    mainlo->addWidget(vtime);
    mainlo->addWidget(stopbtn);
    this->setLayout(mainlo);

    this->setGeometry(700, 200, 200, 200);

    QObject::connect(stopbtn, SIGNAL(clicked()), this, SIGNAL(stopRequest()));
}

StatWidget::~StatWidget()
{
    delete mainlo;
    delete lposition;
    delete vposition;
    delete lvelocity;
    delete vvelocity;
    delete ltime;
    delete vtime;
    delete stopbtn;
}

void StatWidget::setPosition(double value)
{
    vposition->setText(QString::number(value));

}

void StatWidget::setVelocity(double value)
{
    vvelocity->setText(QString::number(value));
}

void StatWidget::setTime(double value)
{
    vtime->setText(QString::number(value));
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
