#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char ** argv)
{
    int i;
    Project * testproject;
    DataVariant * data;
    /*testproject =
        new cylinderProject(-50, 300, 50, -50, 0.2, 0.2, 0.2, 0.2, 20);

    for (i = 0; i < 40; i++) {
        testproject->run();
        data = testproject->getData(Project::TimeType);
        cout << data->getTime() << endl;
        delete data;
    }

    testproject->dumptofile("dump"); */

    testproject = new cylinderProject("dump");
    for (i = 0; i < 1000; i++) {
        testproject->spotstainrun();
    }

    data = testproject->getData(Project::NumberType);
    int n = data->getNumber();
    delete data;

    for (i=0;i<n;i++){
        data = testproject->getData(Project::SpotType, i);
        cout << "#######################" << endl;
        cout << "i=" << i <<endl;
        do {
            cout << '(' << data->getX() << ", " << data->getY() << ')' <<
                 endl;
        } while (data->next());
        delete data;
    }
    return 0;
}
