#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char ** argv)
{
    int i;
    cylinderProject * testproject;
    testproject =
        new cylinderProject(-100, 400, 100, -100, 0.1, 0.1, 0.1, 0.1, 40, 1.618);
    testproject->initialize();

    for (i = 0; i < 10; i++) {
        testproject->run();
        testproject->spotstainrun();
        cout << "One step:" << testproject->getT() << endl;
    }

    delete testproject;
    return 0;
}
