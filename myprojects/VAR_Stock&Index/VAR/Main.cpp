#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdint>
#include <cmath>
#include <string>
#include "CSVparser.hpp"
#include <typeinfo>
#include "VaR.h""
using namespace std;

int main()
{
	// Try different data sets in both following lines
	const string filename = "SPY Historical Data.csv"; 
	long quantity = 1;
	double rf = 1.44; // rf is 3-month T-bill rate, i.e. current date risk-free rate today, we use 12/29/2017 in the example
	VaR VaR(filename, quantity,rf);
	VaR.historical();
	VaR.MC_GBM();
	VaR.analytical();
	system("pause");
}

