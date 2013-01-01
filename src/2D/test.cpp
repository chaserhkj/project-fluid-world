#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char ** argv)
{
    int i,j;
    cylinderProject * testproject;
    testproject =
        new cylinderProject(-500, 2000, 500, -500, 1, 0.02, 0.02, 0.02, 40, 1);
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
        cout << node.b0 << ' ' << node.b1 << ' ' <<node.b2 << ' ' <<node.b3 << ' ' <<node.b4<<endl;
        testproject->run();
        cout << "One step:" << testproject->getT() << endl;
    }

    delete testproject;
    return 0;
}
