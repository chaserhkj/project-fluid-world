#include "configwidget.h"
#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    ConfigWidget * w = new ConfigWidget;
    w->show();
    return app.exec();
}

