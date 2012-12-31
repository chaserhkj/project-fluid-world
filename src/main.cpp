#include <QApplication>
#include "frontend/projectmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    ProjectMainWindow win;
    win.show();
    return app.exec();
}
