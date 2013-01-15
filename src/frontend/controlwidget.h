#ifndef _CONTROLWIDGET_H_
#define _CONTROLWIDGET_H_

#include <QWidget>

class ControlWidget : public QWidget
{
    Q_OBJECT;
public:
    ControlWidget(QWidget * parent = 0);
    virtual ~ControlWidget();
signals:
    void modeChose(int index);
};

#endif /* _CONTROLWIDGET_H_ */
