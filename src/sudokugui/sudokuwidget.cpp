#include "sudokuwidget.h"
#include "sudokumodel.h"
#include "combodelegate.h"
#include <QtGui>

SudokuWidget::SudokuWidget(QWidget * parent): QWidget(parent)
{
    //Setting window title.
    this->setWindowTitle(tr("Sudoku Game"));
    //Setting up fixed geometry.
    this->setFixedSize(500, 500);

    //Setting up layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    //Initializing menu bar.
    QMenuBar * bar = new QMenuBar;
    //Initializing game menu.
    QMenu * gameMenu = bar->addMenu(tr("&Game"));
    gameMenu->addAction(tr("&New"), this, SLOT(newActivated()));
    gameMenu->addSeparator();
    gameMenu->addAction(tr("&Quit"), this, SLOT(close()));
#ifdef SUDOKU_SHOW_ABOUT
    //Initializing help menu.
    QMenu * helpMenu = bar->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, SLOT(aboutActivated()));
#endif /* SUDOKU_SHOW_ABOUT */
    //Adding it to the layout.
    layout->addWidget(bar);

    //Initializing table view widget.
    view = new QTableView;

    //Initializing sudoku model.
    //Specify the model's parent explicitly.
    model = new SudokuModel(this);
    //Attach it to the view.
    view->setModel(model);
    
    //Initializing item delegate.
    //Specify the model's parent explicitly.
    ComboDelegate * delegate = new ComboDelegate(this);
    //Attach it to the view.
    view->setItemDelegate(delegate);

    //Setting up table view widget for other properties
    view->setDragEnabled(false);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    view->setShowGrid(true);
    view->setGridStyle(Qt::SolidLine);
    view->setCornerButtonEnabled(false);
    view->setSortingEnabled(false);
    view->setWordWrap(false);
    view->verticalHeader()->hide();
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->hide();
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    //Adding it to the layout.
    layout->addWidget(view);
}

SudokuWidget::~SudokuWidget()
{
}

void SudokuWidget::newActivated()
{
    QMessageBox msgBox;
    QPushButton * easy = msgBox.addButton(tr("&Easy"), QMessageBox::AcceptRole);
    QPushButton * normal = msgBox.addButton(tr("&Normal"), QMessageBox::AcceptRole);
    QPushButton * hard = msgBox.addButton(tr("&Hard"), QMessageBox::AcceptRole);
    msgBox.addButton(tr("&Cancel"), QMessageBox::RejectRole);
    msgBox.setText(tr("Please choose a level:"));
    msgBox.setWindowTitle(tr("Choose Level"));
    msgBox.exec();

    QAbstractButton * clicked = msgBox.clickedButton();
    if(clicked==easy)
        model->reinitialize(sudoku_easy_count);
    else if(clicked==normal)
        model->reinitialize(sudoku_normal_count);
    else if (clicked==hard)
        model->reinitialize(sudoku_hard_count);
    msgBox.dumpObjectTree();
}

#ifdef SUDOKU_SHOW_ABOUT
void SudokuWidget::aboutActivated()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About Sudoku Game"));
    msgBox.setText(tr("<h3>Sudoku Game</h3>"));
    msgBox.
    setInformativeText(tr
                       ("A sudoku game with a C backend and a Qt C++ frontend\n"
                        "Copyright (C) 2012 Project Fluid World\n" "\n"
                        "This program is free software: you can redistribute it and/or modify\n"
                        "it under the terms of the GNU General Public License as published by\n"
                        "the Free Software Foundation, either version 3 of the License, or\n"
                        "(at your option) any later version.\n" "\n"
                        "This program is distributed in the hope that it will be useful,\n"
                        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                        "GNU General Public License for more details.\n" "\n"
                        "You should have received a copy of the GNU General Public License\n"
                        "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
#endif /* SUDOKU_SHOW_ABOUT */
