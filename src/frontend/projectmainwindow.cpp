#include "projectmainwindow.h"
#include <QtGui>
#include "displaywidget.h"

ProjectMainWindow::ProjectMainWindow(QWidget * parent) : QMainWindow(parent)
{
    //Initializing central display widget.
    displayWidget = new DisplayWidget;
    this->setCentralWidget(displayWidget);
    
    //Initializing actions.
    aboutAct=new QAction(QIcon::fromTheme("help-about"), tr("&About"), this);

    //Setting up menus.
    //Help menu.
    helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

ProjectMainWindow::~ProjectMainWindow()
{}
