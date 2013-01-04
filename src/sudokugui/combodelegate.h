#ifndef _COMBODELEGATE_H_
#define _COMBODELEGATE_H_

#include <QItemDelegate>

class ComboDelegate: public QItemDelegate
{
    Q_OBJECT;
public:
    ComboDelegate(QObject * parent = 0);
    virtual ~ ComboDelegate();
    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option,
                           const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model,
                      const QModelIndex & index) const;
};

#endif /* _COMBODELEGATE_H_ */
