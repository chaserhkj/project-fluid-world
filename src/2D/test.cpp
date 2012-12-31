#include <iostream>
#include "cylinder.h"

using namespace std;

int main(int argc, char **argv)
{
    int i;
    cylinderProject *testproject;
    testproject =
	new cylinderProject(-1000, 4000, 1000, -1000, 0.01, 0.01, 0.01, 40,
			    1.618);
    testproject->initialize();
    for (i = 0; i < 10; i++) {
	testproject->run();
	cout << "One step:" << testproject->getT() << endl;
    }
    delete testproject;
    return 0;
}
