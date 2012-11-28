#ifndef _CONFIGWIDGET_H_
#define _CONFIGWIDGET_H_

#include <QWidget>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class CalThread;
class DisplayWidget;
class StatWidget;

class ConfigWidget : public QWidget
{
Q_OBJECT
public:
    ConfigWidget(QWidget *parent = 0);
    virtual ~ConfigWidget();
private:
    QVBoxLayout *mainlo;
    QLabel *plabel;
    QLineEdit *pedit;
    QLabel *vlabel;
    QLineEdit *vedit;
    QLabel *llabel;
    QLineEdit *ledit;
    QLabel *slabel;
    QLineEdit *sedit;
    QHBoxLayout *btnlo;
    QPushButton *submitbtn;
    QPushButton *quitbtn;
    CalThread *cal;
    DisplayWidget *dw;
    StatWidget *sw;
private slots:
    void start();
    void stop();
};

#endif /* _CONFIGWIDGET_H_ */
