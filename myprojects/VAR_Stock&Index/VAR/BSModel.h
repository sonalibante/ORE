#pragma once
#ifndef BSModel_h
#define BSModel_h
using namespace std;
#include <vector>
#include <cstdlib> // for typedef
#include <ctime>		// for srand()
class BSModel
{
public:
	double S0, rf, sigma;
	BSModel(double S0_, double rf_, double sigma_)
	{
		S0 = S0_; rf = rf_; sigma = sigma_;
		srand(time(NULL));
	}
	void MC_Simulation(double deltaT, long m, vector<double>& S); // call by reference to update S
};
#endif

#pragma once
