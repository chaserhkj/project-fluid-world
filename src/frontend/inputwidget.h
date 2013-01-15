#ifndef _INPUTWIDGET_H_
#define _INPUTWIDGET_H_

#include <QWidget>
#include <QList>
class QLabel;

class InputWidget : public QWidget
{
    Q_OBJECT;
public:
    InputWidget(QWidget * parent = 0);
    virtual ~InputWidget();
signals:
    void startClicked();
public slots:
    void changeMode(int index);
private:
    QList<QLabel *> labelList;
    QList<QWidget *> inputList;
};

#endif /* _INPUTWIDGET_H_ */
