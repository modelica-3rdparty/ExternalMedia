#include <stdio.h>
#include "..\Sources\externaltwophasemedium.h"

int main(void)
{
	int uniqueID = createMedium_("TestMedium","TestMedium","R744",0);
	printf("UniqueID:\t%i.\n", uniqueID);
	printf("molarMass:\t%f.\n", getMolarMass_("TestMedium","TestMedium","R744"));
	setState_ph_(30e5, 300e3, 0, uniqueID, 0, 0, 0, 0, 0);
	printf("density:\t%f.\n", density_(uniqueID));
	
	int uniqueID2 = createMedium_("TestMedium","TestMedium","R744",0);
	printf("UniqueID:\t%i.\n", uniqueID2);

	getchar();

	return 0;
}
