//
//  Stock_Price.hpp
//  MidTerm
//
//  Created by zy L on 10/13/21.
//

#ifndef Stock_Price_hpp
#define Stock_Price_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <boost/random.hpp>
#include <boost/math/distributions.hpp>

using namespace std;
class Stock{
private:
    double S0; //Initial price of the stock
    double mu;
    double sigma; //Volatility
    double d_t;
    double N;
public:
    Stock();
    Stock(double, double, double); //input S0,mu,sigma
    vector<double> Stock_Price(double, double);//input S0, deltat, N and output a series of stock price
    ~Stock();
};

#endif /* Stock_Price_hpp */
