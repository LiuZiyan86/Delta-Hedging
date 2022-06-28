//
//  Delta_Hedging.hpp
//  MidTerm
//
//  Created by zy L on 10/16/21.
//

#ifndef Delta_Hedging_hpp
#define Delta_Hedging_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <math.h>
#include <algorithm>

using namespace std;
class Delta_Hedging{
private:
    double n;
    double d_t;
    vector<double> S;//Stock prices series
    vector<double> Delta;//Delta series
    vector<double> r; //Risk free rate series
    vector<double> OP; //Option prices series
    vector<double> Bond_Price;
    vector<double> PNL_V;
    vector<double> PNL_withHE;
    vector<double> Hedging_Error;
public:
    Delta_Hedging();
    Delta_Hedging(double);//deltat
    vector<double> Bond(vector<double>,vector<double>,vector<double>,vector<double>);
    vector<double> Cumulative_HE();//Cumulative Hedging Error, also PNL with hedging
    vector<double> PNL();
    vector<double> HE(); //hedging error
    ~Delta_Hedging();
};
#endif /* Delta_Hedging_hpp */
