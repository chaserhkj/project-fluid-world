#ifndef _PROJECTMAINWINDOW_H_
#define _PROJECTMAINWINDOW_H_

#include <QMainWindow>
class QAction;
class QWidget;
class DisplayWidget;

class ProjectMainWindow: public QMainWindow
{
    Q_OBJECT;
public:
    ProjectMainWindow(QWidget * parent = 0);
    virtual ~ ProjectMainWindow();
private:
    //Central display widget.
    DisplayWidget * displayWidget;

    //Actions.
    //File menu actions.
    QAction * quitAct;
    
    //Help menu actions.
#ifdef SUDOKU_ENABLED
    QAction * sudokuGameAct;
#endif /* SUDOKU_ENABLED */
    QAction * aboutAct;
    QAction * aboutQtAct;
private slots:
    void aboutActivated();
    void aboutQtActivated();
#ifdef SUDOKU_ENABLED
    void startSudokuGame();
#endif /* SUDOKU_ENABLED */
};

#endif /* _PROJECTMAINWINDOW_H_ */
