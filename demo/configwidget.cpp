#include "configwidget.h"
#include "calthread.h"
#include "displaywidget.h"
#include "statwidget.h"
#include <QtGui>

ConfigWidget::ConfigWidget(QWidget *parent) : QWidget(parent,Qt::Window)
{
    //Initialize UI objects
    mainlo = new QVBoxLayout;
    plabel = new QLabel(tr("Position:(rad)"));
    pedit = new QLineEdit;
    vlabel = new QLabel(tr("Velocity:(rad/s)"));
    vedit = new QLineEdit;
    llabel = new QLabel(tr("Length:(m)"));
    ledit = new QLineEdit;
    slabel = new QLabel(tr("Calculation Step:(ms)"));
    sedit = new QLineEdit;
    btnlo = new QHBoxLayout;
    submitbtn = new QPushButton(tr("&Submit"));
    quitbtn = new QPushButton(tr("&Quit"));

    //Set up UI
    mainlo->addWidget(plabel);
    mainlo->addWidget(pedit);
    mainlo->addWidget(vlabel);
    mainlo->addWidget(vedit);
    mainlo->addWidget(llabel);
    mainlo->addWidget(ledit);
    mainlo->addWidget(slabel);
    mainlo->addWidget(sedit);
    btnlo->addWidget(submitbtn);
    btnlo->addWidget(quitbtn);
    mainlo->addLayout(btnlo);
    this->setLayout(mainlo);
    this->setWindowTitle(tr("Config"));

    //Set default value
    sedit->setText("10");
    
    //Connect signals
    QObject::connect(quitbtn,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(submitbtn,SIGNAL(clicked()),this,SLOT(start()));
}						  

ConfigWidget::~ConfigWidget()
{
    delete mainlo;
    delete plabel;
    delete pedit;
    delete vlabel;
    delete vedit;
    delete llabel;
    delete ledit;
    delete slabel;
    delete sedit;
    delete btnlo;
    delete submitbtn;
    delete quitbtn;
}

void ConfigWidget::start()
{
    //Passing arguments from line edits to CalThread
    //And initialize CalThread object
    double p = pedit->text().toDouble();
    double v = vedit->text().toDouble();
    double l = ledit->text().toDouble();
    double s = sedit->text().toDouble();
    s = s / 1000;
    cal = new CalThread(p,v,l,s,this);
    QObject::connect(cal, SIGNAL(finished()), this, SLOT(autoDeleteThread()));
    
    dw = new DisplayWidget(this,p);
    sw = new StatWidget(this);

    QObject::connect(cal, SIGNAL(valueChanged(double, double)), dw, SLOT(changeGraph(double, double)));
    QObject::connect(cal, SIGNAL(valueChanged(double, double)), sw, SLOT(setStatus(double,double)));
    QObject::connect(sw, SIGNAL(stopRequest()), this, SLOT(stop()));
    
    this->setBlocked(false);
    dw->show();
    sw->show();

    cal->start();
}

void ConfigWidget::stop()
{
    cal->quit();

    dw->setCloseFlag();
    dw->close();
    delete dw;
    dw = 0;

    sw->setCloseFlag();
    sw->close();
    delete sw;
    sw = 0;

    this->setBlocked(true);
}

void ConfigWidget::setBlocked(bool b)
{
    plabel->setEnabled(b);
    pedit->setEnabled(b);
    vlabel->setEnabled(b);
    vedit->setEnabled(b);
    llabel->setEnabled(b);
    ledit->setEnabled(b);
    slabel->setEnabled(b);
    sedit->setEnabled(b);
    submitbtn->setEnabled(b);
    quitbtn->setEnabled(b);

}

void ConfigWidget::autoDeleteThread()
{
    delete cal;
    cal = 0;
}
