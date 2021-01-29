//#pragma warning( disable: 4819 )
//#include <ql/quantlib.hpp>
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <vector>
//#include <cmath>
//#include <climits>
//#include <cstdint>
//#include <cmath>
//#include <string>
//#include "CSVparser.hpp"
//#include <typeinfo>
//#include "VaR.h""
//
//using namespace QuantLib;
//using namespace std;
//
//int main() {
//	// Portfolio parameter setting
//	double k = 1.65;
//	double TTM = 3.0 / 12;
//	long quantity = 1e7;
//
//	//////////////////// Analytical Method ////////////////////////////////
//	// Collect the daily return data
//	ifstream filein("AnalyticalData.csv");
//	int t;
//	double r_S, r_PVf, r_PVd, weight1, weight2;
//
//	// Construct the sequencestatistics
//	SequenceStatistics ss;
//	vector<Real> daily_return_indexes;
//
//	for (string line; getline(filein, line);)
//	{
//		stringstream lineStream(line);
//		string cell;
//		int cell_idx = 0;
//
//		while (getline(lineStream, cell, ',')) {
//			// Classify the cell into different fields
//			switch (cell_idx) {
//			case 0:
//				t = atoi(cell.c_str());
//				if (t<1 || t>500) t = -1; // Mark the invalid entry as -1
//				break;
//			case 9:      r_S = atof(cell.c_str());      break;
//			case 10:      r_PVf = atof(cell.c_str());      break;
//			case 11:      r_PVd = atof(cell.c_str());      break;
//			case 13:	  weight1 = atof(cell.c_str());      break;
//			case 14:	  weight2 = -k*atof(cell.c_str());      break;
//			}
//			cell_idx++;
//		}
//		// Skip the invalid data
//		if (t == -1) continue;
//		daily_return_indexes.clear();
//		daily_return_indexes.push_back(r_S);
//		daily_return_indexes.push_back(r_PVf);
//		daily_return_indexes.push_back(r_PVd);
//		ss.add(daily_return_indexes);
//	}
//
//	// Calculate the covariance
//	cout << endl;
//	cout << "Covariance Matrix with equally weighting of the daily returns\n";
//	cout.setf(ios_base::right, ios_base::adjustfield);
//	cout.setf(ios_base::showpoint);
//	cout.setf(ios_base::fixed, ios_base::floatfield);
//	cout.precision(10);
//	cout << ss.covariance() << "\n";
//
//	//// Calculate the portfolio variance using normalized weight (sum of weights is 1)
//	//Matrix omega(3, 1); // Investment weighting
//	//omega[0][0] = quantity * weight1 / (2*weight1 + weight2); // S
//	//omega[1][0] = quantity * weight1 / (2*weight1 + weight2); // PVf
//	//omega[2][0] = quantity * weight2 / (2*weight1 + weight2); // PVd
//
//	// Calculate the portfolio variance using unnormalized weight
//	Matrix omega(3, 1); // Investment weighting
//	omega[0][0] = quantity * weight1; // S
//	omega[1][0] = quantity * weight1; // PVf
//	omega[2][0] = quantity * weight2; // PVd
//
//	Matrix R(3, 1); // Investment weighting
//	R[0][0] = ss.mean()[0]; 
//	R[1][0] = ss.mean()[1]; 
//	R[2][0] = ss.mean()[2]; 
//
//	Matrix mean_portfolio(1, 1);
//	mean_portfolio = transpose(omega) * R;
//	cout << "Portfolio mean: " << mean_portfolio[0][0] << "\n";
//
//	Matrix variance_portfolio(1, 1);
//	variance_portfolio = transpose(omega) * ss.covariance() * omega;
//	cout << "Portfolio variance: " << variance_portfolio[0][0] << "\n";
//		
//	// Obtain z statistics at 95%
//	boost::math::normal_distribution<> d(0, 1);
//	double z_stat = quantile(d, 0.95);
//
//	// Calculate the one day 95% VaR
//	cout << "The one day 95% VaR based on Analytical method is: " << mean_portfolio[0][0] - z_stat * sqrt(variance_portfolio[0][0]) << "\n\n";
//	
//	//////////////////// Historical Method ////////////////////////////////
//	cout << "Historical method result is:" << endl;
//	const string s = "Data.csv";
//	VaR VaR(s, k, TTM, quantity);
//	VaR.historical();
//	system("pause");
//}
//
