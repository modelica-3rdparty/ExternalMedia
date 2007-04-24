#include <cstdlib>
#include <iostream>

using namespace std;

#include "externaltwophasemedium.h"

int main(int argc, char *argv[])
{
    int uniqueID = createMedium_("TestMedium", "TestMedium", "TestMedium", 0);
    cout << "UniqueID:\t" << uniqueID << endl;
    setState_pT_(1e5, 300.0, 0, uniqueID, 0, 0, 0, 0, 0);
    cout << "p:\t" << pressure_(uniqueID) << endl;
    cout << "h:\t" << specificEnthalpy_(uniqueID) << endl;
    cout << "s:\t" << specificEntropy_(uniqueID) << endl;

    system("PAUSE");
    return EXIT_SUCCESS;
}
