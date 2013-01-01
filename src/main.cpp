#include <QApplication>
#include <FWfrontend.h>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    ProjectMainWindow win;
    win.show();
    return app.exec();
}
