#ifndef _STATWIDGET_H_
#define _STATWIDGET_H_

#include <QWidget>

class QVBoxLayout;
class QLabel;
class QPushButton;
class QCloseEvent;

class StatWidget : public QWidget
{
public:
    StatWidget(QWidget *parent = 0);
    ~StatWidget();
    void setCloseFlag();
public slots:
    void setStatus(double position, double velocity);
protected:
    void closeEvent(QCloseEvent *event);
private:
    QVBoxLayout *mainlo;
    QLabel *lposition;
    QLabel *vposition;
    QLabel *lvelocity;
    QLabel *vvelocity;
    QPushButton *stopbtn;
    bool closeFlag;
signals:
    void stopRequest();
};

#endif /* _STATWIDGET_H_ */
