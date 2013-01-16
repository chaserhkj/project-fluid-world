#include "projectmainwindow.h"
#include <QtGui>
#include "displaywidget.h"
#include "controlwidget.h"
#include "inputwidget.h"
#include "calthread.h"

#ifdef SUDOKU_ENABLED
#include <FWsudokuGui.h>
#endif /* SUDOKU_ENABLED */

ProjectMainWindow::ProjectMainWindow(QWidget * parent): QMainWindow(parent),thread(NULL)
{
    //Setting window title.
    this->setWindowTitle("Fluid World");
    //Setting up window geometry.
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,QSize(900,600),qApp->desktop()->availableGeometry()));
    
    //Initializing central display widget.
    displayWidget = new DisplayWidget;
    this->setCentralWidget(displayWidget);

    //Initializing control widget.
    QDockWidget * controlDock = new QDockWidget(tr("Control"));
    controlWidget = new ControlWidget;
    controlDock->setWidget(controlWidget);
    controlDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    controlDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    controlDock->setFixedWidth(150);
    //Adding it to the main window.
    this->addDockWidget(Qt::LeftDockWidgetArea, controlDock);

    //Initialzing input widget.
    QDockWidget * inputDock = new QDockWidget(tr("Input"));
    inputWidget = new InputWidget;
    inputDock->setWidget(inputWidget);
    inputDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    inputDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    inputDock->setFixedWidth(150); 
    //Adding it to the main window.
    this->addDockWidget(Qt::RightDockWidgetArea, inputDock);

    //Connecting signals for Input Widget, Control Widget and Display Widget
    QObject::connect(inputWidget, SIGNAL(startClicked()), this, SLOT(startCalculate()));
    
    //Initializing actions.
    QAction * quitAct = new QAction(QIcon::fromTheme("application-exit"),tr("&Quit"), this);
#ifdef SUDOKU_ENABLED
    QAction * sudokuGameAct = new QAction(tr("&Sudoku Game"), this);
#endif /* SUDOKU_ENABLED */
    QAction * aboutAct = new QAction(QIcon::fromTheme("help-about"), tr("&About"), this);
    QAction * aboutQtAct = new QAction(QIcon::fromTheme("help-about"), tr("About &Qt"),  this);

    //Connecting slots for actions.
    QObject::connect(quitAct,SIGNAL(triggered()), this, SLOT(close()));
#ifdef SUDOKU_ENABLED
    QObject::connect(sudokuGameAct,SIGNAL(triggered()),this,SLOT(startSudokuGame()));
#endif /* SUDOKU_ENABLED */
    QObject::connect(aboutAct,SIGNAL(triggered()),this,SLOT(aboutActivated()));
    QObject::connect(aboutQtAct,SIGNAL(triggered()), this, SLOT(aboutQtActivated()));
    
    //Initializing menu bar; 
    QMenuBar * menuBar = this->menuBar();

    //Initializing menus.
    //File menu.
    QMenu * fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(quitAct);
    //Help menu.
    QMenu * helpMenu = menuBar->addMenu(tr("&Help"));
#ifdef SUDOKU_ENABLED
    helpMenu->addAction(sudokuGameAct);
    helpMenu->addSeparator();
#endif /* SUDOKU_ENABLED */
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

ProjectMainWindow::~ProjectMainWindow()
{
}

CalThread * ProjectMainWindow::getThread()
{
    return thread;
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

void ProjectMainWindow::startCalculate()
{
    if (thread != NULL)
        delete thread;
    thread = new CalThread;
    QObject::connect(thread,SIGNAL(dataGenerated()),
                     displayWidget,SLOT(updateGraph()),Qt::DirectConnection);
    thread->start();
    emit calculateStarted();
}

#ifdef DEBUG
#include <debug/QDumpObjectTree.cpp>
#endif /* DEBUG */


#ifdef SUDOKU_ENABLED
void ProjectMainWindow::startSudokuGame()
{
    //Initializing sudoku widget.
    SudokuWidget * sudoku = new SudokuWidget(this);
    //Explicitly setting it to behave as a window.
    sudoku->setWindowFlags(Qt::Window);
    //Setting its position.
    sudoku->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,sudoku->size(),qApp->desktop()->availableGeometry()));
    //Show it.
    sudoku->show();
#ifdef DEBUG
    dumpObjTree((QObject *)sudoku);
#endif /* DEBUG */

}
#endif /* SUDOKU_ENABLED */
