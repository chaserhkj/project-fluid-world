#ifndef _SUDOKUMODEL_H_
#define _SUDOKUMODEL_H_

#include <FWsudoku.h>
#include <QAbstractTableModel>

class SudokuModel : public QAbstractTableModel
{
    Q_OBJECT;
public:
    SudokuModel(QObject * parent = 0);
    SudokuModel(int count, QObject * parent = 0);
    virtual ~SudokuModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    void deleteData(const QModelIndex & index);
    void reinitialize();
    void reinitialize(int count);
private:
    sudoku_table tb;
};

#endif /* _SUDOKUMODEL_H_ */
