#include "combodelegate.h"
#include "sudokumodel.h"
#include <QtGui>

ComboDelegate::ComboDelegate(QObject * parent): QItemDelegate(parent)
{
}

ComboDelegate::~ComboDelegate()
{
}


QWidget * ComboDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QComboBox * box = new QComboBox(parent);
    QStringList list;
    list<<""<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9";
    box->addItems(list);
    return box;
}

void ComboDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QComboBox * box =qobject_cast<QComboBox *>(editor);
    box->setCurrentIndex(index.data(Qt::EditRole).toInt());
}

void ComboDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QComboBox * box = qobject_cast<QComboBox *>(editor);
    SudokuModel * m = qobject_cast<SudokuModel *>(model);
    
    int v = box->currentIndex();

    if (v == 0)
        m->deleteData(index);
    else if (!m->setData(index, box->currentIndex(), Qt::EditRole))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText("<h3>Invalid value!</h3>");
        msgBox.setInformativeText("You've entered an invalid value that conflicts with the rules.\n\n"
                                  "That's all we know.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

