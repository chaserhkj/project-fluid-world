#ifndef _PROJECTMAINWINDOW_H_
#define _PROJECTMAINWINDOW_H_

#include <QMainWindow>
class QAction;
class QWidget;
class DisplayWidget;
class InputWidget;
class ControlWidget;
class CalThread;

class ProjectMainWindow : public QMainWindow
{
    Q_OBJECT;
public:
    ProjectMainWindow(QWidget * parent = 0);
    virtual ~ProjectMainWindow();
    CalThread * getThread();
signals:
    void calculateStarted();
private:
    //Thread for calculating.
    CalThread * thread;

    //Central display widget.
    DisplayWidget * displayWidget;
    InputWidget * inputWidget;
    ControlWidget * controlWidget;

    //Pause flag
    bool isPaused;
private slots:
    void aboutActivated();
    void aboutQtActivated();
    void startCalculate();
    void pauseCalculate();
    void stopCalculate();
    void afterCalculate();
    void getPreviousGraph();
    void getNextGraph();
#ifdef SUDOKU_ENABLED
    void startSudokuGame();
#endif /* SUDOKU_ENABLED */
};

#endif /* _PROJECTMAINWINDOW_H_ */
