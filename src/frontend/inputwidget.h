#ifndef _INPUTWIDGET_H_
#define _INPUTWIDGET_H_

#include <QWidget>
class QLineEdit;

class InputWidget : public QWidget
{
public:
    InputWidget(QWidget * parent = 0);
    virtual ~InputWidget();
private:
    QLineEdit * edit;
};

#endif /* _INPUTWIDGET_H_ */
