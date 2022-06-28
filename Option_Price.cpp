//
//  Option_Price.cpp
//  MidTerm
//
//  Created by zy L on 10/13/21.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <boost/random.hpp>
#include <boost/math/distributions.hpp>
#include "Option_Price.hpp"

using namespace std;
const unsigned int seed = 12411;
boost::mt19937 rng2(seed);
boost::math::normal_distribution<> Norm2(0,1);

//Constructor
Option_Price::Option_Price(double inputK,double inputsigma,double inputt,string f){
    K = inputK;
    t = inputt;
    sigma = inputsigma;
    flag = f;
}

//Standard Normal CDF
double Option_Price::N(double x) const {
    double z = 1/(1+B*x);
    double Rz = A1*z + A2*pow(z,2) + A3*pow(z,3) + A4*pow(z,4) + A5*pow(z,5);
    double n = 1 - NORMALIZER*exp(-pow(x,2)*0.5)*Rz;
    return n;
}
const double Option_Price::A1 = 0.319381530;
const double Option_Price::A2 = -0.35653782;
const double Option_Price::A3 = 1.781477937;
const double Option_Price::A4 = -1.821255978;
const double Option_Price::A5 = 1.330274429;
const double Option_Price::B = 0.2316419;
const double Option_Price::NORMALIZER = 0.39894228;


vector<double> Option_Price::BS_Option_Price_Serie(vector<double> inputS,vector<double> inputr)
{
    S = inputS;
    r = inputr;
    double n = S.size();
    vector<double> BS_Option_Prices(n);
    double deltat = t/(n-1);
    vector<double> Deltai;
    for(int i=0;i<n;i++){
        double ttm = t - i*deltat;
        if(flag=="c"){
            double d1 = (log(S[i]/K)+(r[i]+0.5*pow(sigma,2))*ttm)/(sigma*sqrt(ttm));
            double d2 = d1-sigma*sqrt(ttm);
            BS_Option_Prices[i] = S[i]*cdf(Norm2,d1)-K*exp(-r[i]*ttm)*cdf(Norm2,d2);
            Deltai.push_back(cdf(Norm2,d1));
//            cout<<BS_Option_Prices[i]<<endl;
        }
        if(flag=="p"){
            double d1 = (log(S[i]/K)+r[i]*ttm)/(sigma*sqrt(ttm))+0.5*sigma*sqrt(ttm);
            double d2 = d1-sigma*sqrt(ttm);
            BS_Option_Prices[i] = K*exp(-r[i]*ttm)*cdf(Norm2,d2)-S[i]*cdf(Norm2,-d1);
            Deltai.push_back(cdf(Norm2,d1));
        };
        Delta=Deltai;
    }
    
    return BS_Option_Prices;
}

vector<double> Option_Price::Get_Delta(){
    return Delta;
}

vector<double> Option_Price::Get_IV(vector<double> iptS, vector<double> iptO, vector<double> iptr, double iptlowerbound, double iptupperbound,vector<double> iptttm){
    S = iptS;
    r = iptr;
    time_to_maturity = iptttm;
    double n = S.size();
    for(int i=0;i<n;i++){
//        cout<<"!!!"<<i<<endl;
//        cout<<iptO[i]<<endl;
        double upperbound = iptupperbound;
        double lowerbound = iptlowerbound;
        double binarypoint = (upperbound + lowerbound)*0.5;
//        cout<<binarypoint<<endl;
        double ti = iptttm[i];//here
        double price_tempt = 0.0;
        if(flag=="c"){
            double d1 = (log(S[i]/K)+(r[i]+0.5*pow(binarypoint,2))*ti)/(binarypoint*sqrt(ti));
            double d2 = d1-binarypoint*sqrt(ti);
            price_tempt = S[i]*cdf(Norm2,d1)-K*exp(-r[i]*t)*cdf(Norm2,d2);
        }
        if(flag=="p"){
            double d1 = (log(S[i]/K)+r[i]*ti)/(binarypoint*sqrt(ti))+0.5*binarypoint*sqrt(ti);
            double d2 = d1-binarypoint*sqrt(ti);
            price_tempt = K*exp(-r[i]*ti)*cdf(Norm2,d2)-S[i]*cdf(Norm2,-d1);
        }
        int dwcount = 0;
        do {
//            cout<<"cout"<<dwcount<<":"<<price_tempt<<".'"<<binarypoint<<endl;
            
            if(price_tempt < iptO[i]){
                lowerbound = binarypoint;
//                cout<<lowerbound<<endl;
            }
            else if(price_tempt > iptO[i]){
                upperbound = binarypoint;
            }
            binarypoint = (upperbound + lowerbound)*0.5;
            if(flag=="c"){
                double d1 = (log(S[i]/K)+(r[i]+0.5*pow(binarypoint,2))*ti)/(binarypoint*sqrt(ti));
                double d2 = d1-binarypoint*sqrt(ti);
                price_tempt = S[i]*cdf(Norm2,d1)-K*exp(-r[i]*t)*cdf(Norm2,d2);
//                cout<<iptO[i]<<endl;
//                cout<<price_tempt<<endl;
            }
            if(flag=="p"){
                double d1 = (log(S[i]/K)+r[i]*ti)/(binarypoint*sqrt(ti))+0.5*binarypoint*sqrt(ti);
                double d2 = d1-binarypoint*sqrt(ti);
                price_tempt = K*exp(-r[i]*ti)*cdf(Norm2,d2)-S[i]*cdf(Norm2,-d1);
            }
            dwcount = dwcount+1;
            if(dwcount == 100)
            {
                break;
            }
        } while(fabs(price_tempt-S[i]) > 0.01);
//        cout<<iptO[i]<<"/"<<binarypoint<<endl;
        implied_sigma.push_back(binarypoint);
    }
    return implied_sigma;
}

vector<double> Option_Price::Get_Delta_Real(){
    double n = S.size();
    for(int i=0;i<n;i++){
        double d1 = (log(S[i]/K)+r[i]*time_to_maturity[i])/(implied_sigma[i]*sqrt(time_to_maturity[i]))+0.5*implied_sigma[i]*sqrt(time_to_maturity[i]);
        Delta.push_back(cdf(Norm2,d1));
//        cout<<S[i]<<"/"<<r[i]<<"/"<<time_to_maturity[i]<<"/"<<implied_sigma[i]<<"/"<<cdf(Norm2,d1)<<endl;
    }
    return Delta;
}

//Destructing class option_price
Option_Price::~Option_Price(){   }
