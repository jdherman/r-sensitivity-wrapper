#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

double Ackley(vector <double> params);
double Bohachevsky(vector <double> params);
double Griewank(vector <double> params);
double Katsuura(vector <double> params);
double Rastigrin(vector <double> params);
double Rosenbrock(vector <double> params);
double Schaffer(vector <double> params);
double Schwefel(vector <double> params);
const double PI = 3.1415926;

int main (int argc, char *argv[])
{	
	double (*fxn)(vector <double> params);
	vector <double> params;
	string line;
	double p;
	
	if(argc < 2)
	{
		cerr << "Error: One input argument 0-7 required to specify the test problem" << endl;
		cerr << "In order: Ackley, Bohachevsky, Griewank, Katsuura, Rastigrin, Rosenbrock, Schaffer, Schwefel" << endl;
		exit(-1);
	}

	switch(atoi(argv[1]))
	{
		case 0:
			fxn = &Ackley; break;
		case 1: 
			fxn = &Bohachevsky; break;
		case 2:
			fxn = &Griewank; break;
		case 3:
			fxn = &Katsuura; break;
		case 4:
			fxn = &Rastigrin; break;
		case 5:
			fxn = &Rosenbrock; break;
		case 6:
			fxn = &Schaffer; break;
		case 7:
			fxn = &Schwefel; break;
		default:
			cerr << "Error: problem value 0-7 required" << endl;
			exit(-1);
	}
	
	while(getline(cin, line))
	{
		params.clear();
		stringstream lineIn(line);
		
		while(lineIn >> p)
			params.push_back(p);
		
		cout << fxn(params) << endl;
	}

	return 0;
}

double Ackley(vector <double> params)
{
	// Init parameters on [1, 30]
	
	int N = params.size();
	double sumSquares = 0;
	
	for(int i = 0; i < N; i++)
		sumSquares += pow(params[i], 2);
		
	return (20 - 20*exp(-0.2*sqrt((1/(double)N)*sumSquares)));
}

double Bohachevsky(vector <double> params)
{
	// Init parameters on [1, 15]
	
	int N = params.size();
	double total = 0;
	
	for(int i = 0; i < N-1; i++)
	{
		total += pow(params[i], 2)
				+ 2*pow(params[i+1], 2)
				- 0.3*cos(3*PI*params[i])
				- 0.4*cos(4*PI*params[i+1])
				+ 0.7;
	}
	
	return total;
}

double Griewank(vector <double> params)
{
	// Init parameters on [10, 600]
	
	int N = params.size();
	double sumSquares = 0;
	double multCosines = 1;
	
	for(int i = 0; i < N; i++)
	{
		sumSquares += pow(params[i], 2);
		multCosines *= cos(params[i]/sqrt(i+1));
	}
	
	return ((1/4000)*sumSquares - multCosines + 1);
}

double Katsuura(vector <double> params)
{
	// Init parameters on [-5 5]
	
	int N = params.size();
	double outerProduct = 1;
	double innerSum;
	
	for(int i = 0; i < N; i++)
	{
		innerSum = 0;
		for(int k = 0; k <= 32; k++)
			innerSum += fabs(pow(2.0, k)*params[i] - rint(pow(2.0, k)*params[i]))*pow(2.0, -1*k);
		
		outerProduct *= 1 + i*innerSum;
	}
	
	return outerProduct;
}

double Rastigrin(vector <double> params)
{
	// Init parameters on [-5, 5]
	
	int N = params.size();
	double sum = 0;
	
	for(int i = 0; i < N; i++)
		sum += pow(params[i], 2) - 10*cos(2*PI*params[i]);
		
	return (10*N + sum);
}

double Rosenbrock(vector <double> params)
{
	// Init parameters on [-5, 5]
	
	int N = params.size();
	double sum = 0;
	
	for(int i = 0; i < N-1; i++)
		sum += 100*pow(pow(params[i], 2) - params[i+1], 2) + pow(params[i] - 1, 2);
		
	return sum;
}

double Schaffer(vector <double> params)
{
	// Init parameters on [10, 100]
	
	int N = params.size();
	double sum = 0;
	double innerThing = 0;
	
	for(int i = 0; i < N-1; i++)
	{
		innerThing = pow(params[i], 2) + pow(params[i+1], 2);
		sum += pow(innerThing, 0.25) * (1 + pow(sin(50*pow(innerThing, 0.1)), 2));
	}
	
	return sum;
}

double Schwefel(vector <double> params)
{
	// Init params on [-500 500]
	// Global min when all x = 420.9687
	
	int N = params.size();
	double sum = 0;
	
	for(int i = 0; i < N; i++)
		sum += params[i]*sin(sqrt(fabs(params[i])));
		
	return (418.9828872724339*N - sum);
}
