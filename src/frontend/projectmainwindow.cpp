#include "projectmainwindow.h"
#include <QtGui>
#include "displaywidget.h"

ProjectMainWindow::ProjectMainWindow(QWidget * parent): QMainWindow(parent)
{
    //Setting window title.
    this->setWindowTitle("Fluid World");
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,QSize(800,600),qApp->desktop()->availableGeometry()));
    
    //Initializing central display widget.
    displayWidget = new DisplayWidget;
    this->setCentralWidget(displayWidget);

    //Initializing actions.
    aboutAct = new QAction(QIcon::fromTheme("help-about"), tr("&About"), this);
    aboutQtAct = new QAction(QIcon::fromTheme("help-about"), tr("About &Qt"),  this);

    //Connecting slots for actions.
    QObject::connect(aboutAct,SIGNAL(triggered()),this,SLOT(aboutActivated()));
    QObject::connect(aboutQtAct,SIGNAL(triggered()), this, SLOT(aboutQtActivated()));
    
    //Setting up menus.
    //Help menu.
    helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

ProjectMainWindow::~ProjectMainWindow()
{
}

void ProjectMainWindow::aboutActivated()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About Fluid World"));
    msgBox.setText(tr("<h3>Fluid World</h3>"));
    msgBox.setInformativeText(tr(
                                  "A simple cross-platform CFD framework\n"
                                  "Copyright (C) 2012 Project Fluid World\n"                                
                                  "\n"
                                  "This program is free software: you can redistribute it and/or modify\n"
                                  "it under the terms of the GNU General Public License as published by\n"
                                  "the Free Software Foundation, either version 3 of the License, or\n"  
                                  "(at your option) any later version.\n"  
                                  "\n"
                                  "This program is distributed in the hope that it will be useful,\n"
                                  "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"  
                                  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"  
                                  "GNU General Public License for more details.\n"  
                                  "\n"
                                  "You should have received a copy of the GNU General Public License\n"  
                                  "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"  
                                  ));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    
}

void ProjectMainWindow::aboutQtActivated()
{
    QMessageBox::aboutQt(this);
}
