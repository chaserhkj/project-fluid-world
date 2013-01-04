#ifndef _SUDOKUWIDGET_H_
#define _SUDOKUWIDGET_H_

#include <QWidget>
class SudokuModel;

class SudokuWidget : public QWidget
{
    Q_OBJECT;
public:
    SudokuWidget(QWidget * parent = 0);
    virtual ~SudokuWidget();
private:
    SudokuModel * model;
private slots:
    void newActivated();
#ifdef SUDOKU_SHOW_ABOUT
    void aboutActivated();
#endif /* SUDOKU_SHOW_ABOUT */
};

#endif /* _SUDOKUWIDGET_H_ */
