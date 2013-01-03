#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char ** argv)
{
    int i,j;
    cylinderProject * testproject;
    testproject =
        new cylinderProject(-100, 600, 100, -100, 1, 0.1, 0.1, 0.1, 40, 1);
    testproject->initialize();
    cylinderNode& node = testproject->getCoordinate()->access(-5,-19);
    while (1) {
        /*for (j=-500;j<=500;j++) {
            for (i=-500;i<=2000;i++) {
                if (testproject->getCoordinate()->access(i, j).psi>1000) {
                    cout << '(' << i << ", " << j << ')' <<endl;
                }
            }
        }*/
        cout << node.psi<<endl;
        cout << testproject->getCoordinate()->access(-5,19).psi << endl;
        testproject->run();
        cout << "One step:" << testproject->getT() << endl;
    }

    delete testproject;
    return 0;
}
