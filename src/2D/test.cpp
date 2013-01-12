#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char ** argv)
{
    int i,j;
    cylinderProject * testproject;
    testproject =
        new cylinderProject(-50, 300, 50, -50, 0.2, 0.2, 0.2, 0.2, 20);
    testproject->initialize();
    cylinderSpotStainSource* source = testproject->getSource();
    cylinderNode& node = testproject->getCoordinate()->access(-5,-19);
    cylinderSpotStain* spot;
    i = 0;
    while (1) {
        /*for (j=-500;j<=500;j++) {
            for (i=-500;i<=2000;i++) {
                if (testproject->getCoordinate()->access(i, j).psi>1000) {
                    cout << '(' << i << ", " << j << ')' <<endl;
                }
            }
        }*/
        //cout << node.psi<<endl;
        //cout <<node.c1<<' '<<node.c2<<' '<<node.c3<<' '<<node.c4<<' '<<endl;
        //cout << testproject->getCoordinate()->access(-5,19).uxi << endl;
        testproject->run();
        testproject->spotstainrun();
        spot = source->getchain(11);
        i = 1;
        while (spot)
        {
            cout << "Point" << i <<endl;
            spot->calculateXY();
            cout << "xi=" <<spot->xi <<' ' <<"eta=" << spot->eta<<endl;
            cout << "x=" <<spot->x <<' ' <<"y=" << spot->y<<endl;
            spot = spot->next;
            i += 1;
        }
        //testproject->calculateNewZeta();
        //testproject->calculateNewPsi();
        cout << "One step:" << testproject->getT() << endl;
    }

    delete testproject;
    return 0;
}
