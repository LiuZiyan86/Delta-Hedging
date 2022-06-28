//
//  Option_Price.hpp
//  MidTerm
//
//  Created by zy L on 10/13/21.
//

#ifndef Option_Price_hpp
#define Option_Price_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <boost/random.hpp>
#include <boost/math/distributions/normal.hpp>
using namespace std;
class Option_Price{
private:
    double K; //strike price
    vector<double> S; //Stock price series
    double t; //time to maturity
    vector<double> r; //risk free rate
    double sigma; //volatility
    vector<double> implied_sigma; //implied volatility
    vector<double> time_to_maturity;//time to maturity
    static const double A1;
    static const double A2;
    static const double A3;
    static const double A4;
    static const double A5;
    static const double B;
    static const double NORMALIZER;

    
public:
    vector<double> Delta;
    Option_Price();
    string flag; // c for call option; p for put option
    Option_Price(double,double,double,string);//Input strike price,time to maturity, volatility, type
    double N(double x) const;
    vector<double> BS_Option_Price_Serie(vector<double>,vector<double>);//Input underlying stock price series and risk free rate
    vector<double> Get_Delta();
    vector<double> Get_IV(vector<double>,vector<double>,vector<double>,double,double,vector<double>); //stock price and option price,risk free return,initial upper limit and lower limit
    vector<double> Get_Delta_Real(); //  stock price, time to maturity, risk free return
    ~Option_Price();
};

#endif /* Option_Price_hpp */
