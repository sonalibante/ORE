#include <iostream>
#include <vector>
#include <cmath>
#include "VaR.h"
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <ql/quantlib.hpp>
#include <string>
#include "BSModel.h"
#include <climits>
#include <cstdint>
#include <typeinfo>

#pragma warning( disable : 4819)
using namespace std;
using namespace QuantLib;
double VaR::risk_VaR(const vector<double> & pl, double percentile)
{
	/*
	this function returns the Value-at-Risk at certain percentile,
	and print out the expected shortfall, given the vector of return distribution
	typically, when there is a loss, VaR is reported as a negative number
	input:
	pl: profit&loss distribution
	percentile: the 0-100 percentile Domer chooses
	(eg: 99% VaR: percentile=1; 95% VaR: percentile=5)
	*/
	vector<double> v = pl;
	sort(v.begin(), v.end()); // sort P&L from loss to profit
	int ind = floor(percentile / 100 * pl.size()); // find the index according to percentile
	vector<double>::const_iterator first = v.begin();
	vector<double>::const_iterator last = v.begin() + ind - 1;
	vector<double> ES_vector(first, last); // use a new vector to store losses that are beyond VaR
	double ES = std::accumulate(ES_vector.begin(), ES_vector.end(), 0.0) / ES_vector.size(); // caculate expected shortfall (average of ES_vector)
	cout << "Expected shortfall in " << (100 - percentile) << "% is " << ES << endl; // print out ES
	return v[ind]; // return the value of VaR
}


void VaR::historical() {
	cout << "Calculate VaR by historical method:" << endl;
	vector<double> rtn;
	double SpotPrice = stod(data[data.rowCount() - 1]["Price"]); // Price at today
	double OldPrice = stod(data[0]["Price"]);
	double oldPV = SpotPrice * quantity;

	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << "Price" << setw(15) << "Portfolio" << setw(15) << "P&L" << endl;
	for (int i = 1; i < data.rowCount() - 1; i++) {
		double NewPrice = stod(data[i]["Price"]);
		double SimulatedPrice = NewPrice - OldPrice + SpotPrice;
		OldPrice = NewPrice;
		double port = SimulatedPrice * quantity;
		double retn = port - oldPV;
		rtn.push_back(retn);
		cout << SimulatedPrice << setw(15) << port << setw(15) << retn << endl;
	}
	cout << "The one day 95% VaR based on Historical method is: " << risk_VaR(rtn, 5) << endl << endl;
}

void VaR::MC_GBM() {
	cout << "calculate VaR by Monte Carlo" << endl;
	vector<double> rtn;
	double SpotPrice = stod(data[data.rowCount() - 1]["Price"]);
	double oldPV = SpotPrice * quantity;
	double OldPrice = stod(data[0]["Price"]);
	//GBM
	double delta_T = 1.0 / 252;
	long m = 1e5; // Simulation times

	double s0 = SpotPrice;
	double sigma_GBM = Sigma * sqrt(data.rowCount()); // historcial volatility

	BSModel Equity(s0, rf, sigma_GBM);
	vector<double> SimulatedPricePath(m);
	Equity.MC_Simulation(delta_T, m, SimulatedPricePath);
	cout << setiosflags(ios::fixed) << setprecision(6);
	for (int i = 1; i < m; i++) {
		double retn = SimulatedPricePath[i] * quantity - oldPV;
		rtn.push_back(retn);
	}
	cout << "The one day 95% VaR based on Monte Carlo with only GBM model is: " << risk_VaR(rtn, 5) << endl << endl;
}

void VaR::analytical() {
	//////////////////// Analytical Method ////////////////////////////////
	double SpotPrice = stod(data[data.rowCount() - 1]["Price"]);
	double oldPV = SpotPrice * quantity;

	cout << "calculate VaR by analytical method" << endl;
	// Obtain z statistics at 95%
	boost::math::normal_distribution<> d(0, 1);
	double z_stat = quantile(d, 0.95);
	// Calculate the one day 95% VaR
	double VaR = oldPV * (Mu - z_stat * Sigma);
	cout << "The one day 95% VaR based on Analytical method is: " << VaR << endl << endl;
}



