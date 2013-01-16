#ifndef _INPUTWIDGET_H_
#define _INPUTWIDGET_H_

#include <QWidget>
#include <QList>
class CalThread;
class QLineEdit;

class InputWidget : public QWidget
{
    Q_OBJECT;
public:
    InputWidget(QWidget * parent = 0);
    virtual ~InputWidget();
    CalThread * constructThread(QObject * parent = 0);
public slots:
    void changeMode(int index);
private:
    QLineEdit * leftEdit;
    QLineEdit * rightEdit;
    QLineEdit * upEdit;
    QLineEdit * downEdit;
    QLineEdit * densEdit;
    QLineEdit * dxiEdit;
    QLineEdit * detaEdit;
    QLineEdit * dtEdit;
    QLineEdit * reyEdit;
    QLineEdit * totalCycleEdit;
    QLineEdit * singleCycleEdit;
    QLineEdit * spotCycleEdit;

};

#endif /* _INPUTWIDGET_H_ */
