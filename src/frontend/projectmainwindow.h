#ifndef _PROJECTMAINWINDOW_H_
#define _PROJECTMAINWINDOW_H_

#include <QMainWindow>
class QAction;
class QWidget;
class DisplayWidget;
class QMenu;

class ProjectMainWindow : public QMainWindow
{
public:
    ProjectMainWindow(QWidget * parent = 0);
    virtual ~ProjectMainWindow();
private:
    //Central display widget.
    DisplayWidget * displayWidget;

    //Help menu items.
    QMenu * helpMenu;
    QAction * aboutAct;
};

#endif /* _PROJECTMAINWINDOW_H_ */
