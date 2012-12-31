#include "configwidget.h"
#include "calthread.h"
#include "displaywidget.h"
#include "statwidget.h"
#include <QtGui>

ConfigWidget::ConfigWidget(QWidget * parent) : QWidget(parent, Qt::Window)
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
    pck = new QCheckBox(tr("Pause at beginning"));
    btnlo = new QHBoxLayout;
    submitbtn = new QPushButton(tr("&Start"));
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
    mainlo->addWidget(pck);
    btnlo->addWidget(submitbtn);
    btnlo->addWidget(quitbtn);
    mainlo->addLayout(btnlo);
    this->setLayout(mainlo);
    this->setWindowTitle(tr("Config"));

    //Set default value
    sedit->setText("10");

    //Connect signals
    QObject::connect(quitbtn, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(submitbtn, SIGNAL(clicked()), this, SLOT(start()));
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
    delete pck;
    delete btnlo;
    delete submitbtn;
    delete quitbtn;
}

void ConfigWidget::start()
{
    //Converting arguments and check if they are valid.
    double p = pedit->text().toDouble();
    double v = vedit->text().toDouble();
    double l = ledit->text().toDouble();
    double s = sedit->text().toDouble();

    if (l <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Length must be greater than zero."));
        return;
    }

    if (s > 10) {
        QMessageBox::warning(this, tr("Warning"), tr("Calculation step must be lesser than 10 ms."));
        return;
    }

    if (s <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Calculation step must be greater than 0 ms"));
        return;
    }

    //Passing arguments and initialize CalThread object
    s = s / 1000;
    cal = new CalThread(p, v, l, s, pck->isChecked(), this);
    QObject::connect(cal, SIGNAL(finished()), this, SLOT(autoDeleteThread()));

    dw = new DisplayWidget(this, p);
    sw = new StatWidget(this);

    QObject::connect(cal, SIGNAL(positionChanged(double)), dw, SLOT(changeGraph(double)));
    QObject::connect(cal, SIGNAL(positionChanged(double)), sw, SLOT(setPosition(double)));
    QObject::connect(cal, SIGNAL(velocityChanged(double)), sw, SLOT(setVelocity(double)));
    QObject::connect(cal, SIGNAL(timeChanged(double)), sw, SLOT(setTime(double)));
    QObject::connect(sw, SIGNAL(stopRequest()), this, SLOT(stop()));
    QObject::connect(sw, SIGNAL(togglePauseRequest()), cal, SLOT(togglePause()));

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
