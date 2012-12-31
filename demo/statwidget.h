#ifndef _STATWIDGET_H_
#define _STATWIDGET_H_

#include <QWidget>

class QVBoxLayout;
class QLabel;
class QPushButton;
class QCloseEvent;

class StatWidget : public QWidget
{
    Q_OBJECT

public:
    StatWidget(QWidget * parent = 0);
    ~StatWidget();
    void setCloseFlag();
public slots:
    void setPosition(double value);
    void setVelocity(double value);
    void setTime(double value);
protected:
    void closeEvent(QCloseEvent * event);
private:
    QVBoxLayout * mainlo;
    QLabel * lposition;
    QLabel * vposition;
    QLabel * lvelocity;
    QLabel * vvelocity;
    QLabel * ltime;
    QLabel * vtime;
    QPushButton * stopbtn;
    QPushButton * pausebtn;
    bool closeFlag;
signals:
    void stopRequest();
    void togglePauseRequest();
};

#endif /* _STATWIDGET_H_ */
