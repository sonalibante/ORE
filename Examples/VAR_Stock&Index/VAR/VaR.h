#ifndef VaR_h
#define VaR_h
#include "CSVparser.hpp"



class VaR {
private:
	Parser data;
	string filename;
	long quantity;
	double Mu = 0, Sigma = 0; // mean and sample sd of return
	double rf;
public:
	VaR(const string &filename_, long quantity_, double rf_) :data(Parser(filename_))
	{
		filename.assign(filename_);
		quantity = quantity_;
		for (int i = 0; i < data.rowCount(); i++) Mu += stod(data[i]["Return"]);
		Mu /= data.rowCount();
		for (int i = 0; i < data.rowCount(); i++) Sigma += pow((stod(data[i]["Return"]) - Mu), 2);
		Sigma = sqrt(Sigma / (data.rowCount() - 1));
		rf = rf_;
	}
	//given return vector and percentile, get VaR
	double risk_VaR(const vector<double> & rtn, double percentile);
	//calculate VaR by historical method
	void historical();
	//calculate VaR by Monte Carlo method
	void MC_GBM();
	//calculate VaR by analytical method
	void analytical();

};

#endif
#pragma once
