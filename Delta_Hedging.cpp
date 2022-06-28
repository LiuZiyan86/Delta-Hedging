//
//  Delta_Hedging.cpp
//  MidTerm
//
//  Created by zy L on 10/16/21.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <math.h>
#include <algorithm>
#include "Delta_Hedging.hpp"
using namespace std;

Delta_Hedging::Delta_Hedging(){}
Delta_Hedging::~Delta_Hedging(){}

Delta_Hedging::Delta_Hedging(double iptd_t){
    d_t = iptd_t;
}

vector<double> Delta_Hedging::Bond(vector<double> iptS,vector<double> iptDelta,vector<double> iptr,vector<double> iptOP){
    S = iptS;
    Delta = iptDelta;
    r = iptr;
    OP = iptOP;
    n  = S.size();
//    cout<<n<<endl;
    vector<double> Bond(n);
    for(int i=0;i<n;i++){
        if(i==0){
            Bond[i] = OP[i] - Delta[i]*S[i];
        }
        else{
            Bond[i] = Delta[i-1]*S[i] + Bond[i-1]*exp(r[i-1]*d_t) - Delta[i]*S[i];
        }
//        cout<<Bond[i]<<endl;
    }
    Bond_Price = Bond;
//    cout<<"?"<<Bond_Price.size()<<endl;
    return Bond;
}

vector<double> Delta_Hedging::Cumulative_HE(){
    n  = S.size();
//    cout<<Delta.size()<<"//"<<Bond_Price.size()<<endl;
    
//    cout<<n<<endl;
    vector<double> Cumu_HE(n);
    for(int i=0;i<n;i++){
        if(i==0){
            Cumu_HE[i] = 0.0;
        }
        else{
            Cumu_HE[i] = Delta[i-1]*S[i] + Bond_Price[i-1]*exp(r[i-1]*d_t) - OP[i];
        }
//        cout<<Cumu_HE[i]<<endl;
    }
    PNL_withHE = Cumu_HE;
    return Cumu_HE;
}

vector<double> Delta_Hedging::PNL(){
    n  = S.size();
    vector<double> PNLw(n);
    for(int i=0;i<n;i++){
        PNLw[i] = OP[0] - OP[i];//For call option
//        cout<<PNLw[i]<<endl;
    }
    PNL_V = PNLw;
    return PNLw;
}

vector<double> Delta_Hedging::HE(){
    n  = S.size();
    vector<double> HE(n);
    for(int i=0;i<n;i++){
        if(i==0){
            HE[i] = 0.0;
        }
        else{
            HE[i] = PNL_withHE[i] - PNL_V[i];
        }
//        cout<<HE[i]<<endl;
    }
    Hedging_Error = HE;
    return HE;
}
