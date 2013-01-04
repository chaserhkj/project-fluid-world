#include "sudokumodel.h"
#include <QtGui>

SudokuModel::SudokuModel(QObject * parent): QAbstractTableModel(parent)
{
    tb = sudoku_init();
}

SudokuModel::SudokuModel(int count,
                         QObject * parent): QAbstractTableModel(parent)
{
    tb = sudoku_random_init(count);
}

SudokuModel::~SudokuModel()
{
    sudoku_clear(tb);
}

int SudokuModel::rowCount(const QModelIndex & parent) const
{
    return 9;
}

int SudokuModel::columnCount(const QModelIndex & parent) const
{
    return 9;
}

QVariant SudokuModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return (int) Qt::AlignHCenter | Qt::AlignVCenter;

    if (role == Qt::DisplayRole) {
        int value = sudoku_get_value(tb, index.column() + 1, index.row() + 1);

        if (value == 0)
            return "";

        return QString::number(value);
    }

    if (role == Qt::EditRole) {
        return sudoku_get_value(tb, index.column() + 1, index.row() + 1);
    }

    return QVariant();
}

Qt::ItemFlags SudokuModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool SudokuModel::setData(const QModelIndex & index, const QVariant & value,
                          int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole) {
        int v = value.toInt();

        if (sudoku_check_value(tb, index.column() + 1, index.row() + 1, v) ==
            0)
            return false;

        sudoku_set_value(tb, index.column() + 1, index.row() + 1, v);
        emit dataChanged(index, index);
        if (sudoku_check_complete(tb))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Congratulations!"));
            msgBox.setText(tr("<h3>Congratulations!</h3>"));
            msgBox.setInformativeText("You've finished!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
        return true;
    }

    return false;
}

void SudokuModel::deleteData(const QModelIndex & index)
{
    sudoku_delete_value(tb, index.column() + 1, index.row() + 1);
    emit dataChanged(index, index);
}

void SudokuModel::reinitialize()
{
    sudoku_clear(tb);
    tb = sudoku_init();
    emit dataChanged(this->createIndex(0, 0), this->createIndex(8, 8));
}

void SudokuModel::reinitialize(int count)
{
    sudoku_clear(tb);
    tb = sudoku_random_init(count);
    emit dataChanged(this->createIndex(0, 0), this->createIndex(8, 8));
}
