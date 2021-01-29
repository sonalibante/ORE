#include "BSModel.h"
#include <cmath>
const double pi = 4.0*atan(1.0);
double Gauss2()
{
	double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
	double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
	return sqrt(-2.0*log(U1)) * cos(2.0*pi*U2);
}
void BSModel::MC_Simulation(double deltaT, long m, vector<double>& S)
{
	S[0] = S0;
	for (int k = 1; k<m; k++)
	{
		S[k] = S0 * exp((rf / 100 - sigma * sigma*0.5)*deltaT + sigma * sqrt(deltaT)*Gauss2());
	}
}

