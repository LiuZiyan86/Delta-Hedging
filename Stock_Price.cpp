//
//  Stock_Price.cpp
//  MidTerm
//
//  Created by zy L on 10/13/21.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <boost/random.hpp>
#include <boost/math/distributions/normal.hpp>
#include "Stock_Price.hpp"

using namespace std;
//Generate random variables
const unsigned int seed = 12411;
boost::mt19937 rng(seed);
boost::normal_distribution<> Norm(0,1);
boost::variate_generator<boost::mt19937&, boost::normal_distribution<>>unnorm(rng, Norm);

//Default Constructor
Stock::Stock(){  };

//Constructor of Stock
Stock::Stock(double iptS0, double iptmu, double iptsigma){
    S0 = iptS0;
    mu = iptmu;
    sigma = iptsigma;
    cout<<"The Stock starts with an initial value of"<<S0<<endl<<"Its sigma and mu is: "<<sigma<<","<<mu<<endl;
}

//Function to calculate stock price
vector<double> Stock::Stock_Price(double iptdeltat, double iptN){
    N = iptN;
    vector<double> S(N+1);
    d_t = iptdeltat;
    for(int i=0;i<N+1;i++){
        if(i==0){
        S[i] = S0;
        }
        else{
        double Z = unnorm();
        S[i] = S[i-1] + mu*S[i-1]*d_t + sigma*S[i-1]*sqrt(d_t)*Z;
        }
//        cout<<"st"<<i<<endl;
    }//i=[0,1,2,...,100]
    return S; //St+deltat
}

//Destructor of the class
Stock::~Stock(){  }
