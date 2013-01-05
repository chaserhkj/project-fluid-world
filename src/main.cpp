#include <QApplication>
#include <FWfrontend.h>

#ifdef DEBUG
#include <QDumpObjectTree.cpp>
#endif /* DEBUG */

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    ProjectMainWindow win;
    win.show();
#ifdef DEBUG
    dumpObjTree(&win);
#endif /* DEBUG */
    return app.exec();
}
