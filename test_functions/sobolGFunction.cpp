#include <iostream>
#include <math.h>

using namespace std;

int main (int argc, char *argv[])
{
	// Expects 8 parameters in cin file. Print objective value to screen or pipe elsewhere.
	double x[8];
	double a[] = {0, 1, 4.5, 9, 99, 99, 99, 99};
	double Y;
	
	while(!cin.eof()) 
	{
		Y = 1.0;
		
		for(int i = 0; i < 8; i++)
		{
			cin >> x[i];
			Y = Y * (fabs(4*x[i] - 2) + a[i]) / (1 + a[i]);
		}
		
		if(!cin.eof())
			cout << Y << endl;
	}


	return 0;
}
