#ifndef _CONTROLWIDGET_H_
#define _CONTROLWIDGET_H_

#include <QWidget>
class QComboBox;
class QDoubleSpinBox;
class QPushButton;

class ControlWidget : public QWidget
{
    Q_OBJECT;
public:
    ControlWidget(QWidget * parent = 0);
    virtual ~ControlWidget();
    enum EnableStatus {Normal, Running, Paused};
    void setEnable(EnableStatus s);
signals:
    void modeChose(int index);
    void factorChanged(double factor);
    void nextGraphClicked();
    void previousGraphClicked();
    void startClicked();
    void pauseClicked();
    void stopClicked();
private:
    QComboBox * modeChooser;
    QDoubleSpinBox * spinBox;
    QPushButton * previousBtn;
    QPushButton * nextBtn;
    QPushButton * startBtn;
    QPushButton * pauseBtn;
    QPushButton * stopBtn;
};

#endif /* _CONTROLWIDGET_H_ */
