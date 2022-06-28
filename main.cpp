//
//  main.cpp
//  MidTerm
//
//  Created by zy L on 10/13/21.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <math.h>
#include <boost/random.hpp>
//#include <ql/quantlib.hpp>
#include <boost/foreach.hpp>
//#include <ql/time/calendar.hpp>
#include "Stock_Price.hpp"
#include "Option_Price.hpp"
#include "Delta_Hedging.hpp"


using namespace std;

//Function for finding element in vectors
int finddatei(vector<string> v,string date)
{
    vector<string>::iterator it;
    it = find(v.begin(),v.end(),date);
    if(it != v.end()){return std::distance(v.begin(),it);}
    else return -1;
}

//Unit test function
void test(double expected, double actual, string testname){
    stringstream actualstream;
    actualstream<<fixed<<setprecision(4)<<actual;
    actualstream>>actual;
    if(expected == actual){
        cout<<testname<<" passed."<<endl;
    }
    else{
        cout<<testname<<" did not pass."<<endl;
        cout<<"Expected: "<<expected<<" , but actually: "<<actual<<endl;
    }
}

int main() {
// Task 1
    double T = 0.4; //Time horizon [0,T]
    double N = 100; //Number of time points our time horizon [0,T]
    double d_t = T/N; //Time length for each stage
    string type="c"; //European call option
    double S0 = 100;
    double K = 105; //Strike price
    double sigma = 0.24; //volatility
    double mu = 0.05;
    double ttm = 0.4; //time to maturity
    vector<double> Risk_free(101,0.025);//risk free rate vector
    //Simulate sample stock price pathes
    Stock stocksample(S0,mu,sigma);
    Option_Price optionsample(K,sigma,ttm,type);
    Delta_Hedging hedgingsample(d_t);

    vector<double> StockPricesPaths(101*1000);
    vector<double> OptionPricesPaths(101*1000);
    vector<double> PNL_Simulated(101*1000);
    vector<double> PNLwithHedging_Simulated(101*1000);
    vector<double> HedgingError_Simulated(101*1000);
    vector<double> BondPricesPaths(101*1000);

    for(int n=0;n<1000;n++){
        vector<double> StockPrices;
        vector<double> OptionPrices;
        vector<double> BondPrices;
        vector<double> PNL;
        vector<double> PNLwithHedging;
        vector<double> HedgingError;
        vector<double> Delta;

        StockPrices = stocksample.Stock_Price(d_t, N);//One path of stock prices
        OptionPrices = optionsample.BS_Option_Price_Serie(StockPrices, Risk_free);//Corresponding path of option prices
        Delta = optionsample.Get_Delta(); //Corresponding Delta
        BondPrices = hedgingsample.Bond(StockPrices,Delta,Risk_free,OptionPrices);
        PNL = hedgingsample.PNL(); //Profit or loss
        PNLwithHedging = hedgingsample.Cumulative_HE(); //Profit or loss with hedging = cumulative hedging error
        HedgingError = hedgingsample.HE(); //Daily hedging error

        for(int i=0; i<N+1;i++){
            StockPricesPaths[i+n*101] = StockPrices[i];
            OptionPricesPaths[i+n*101] = OptionPrices[i];
            PNL_Simulated[i+n*101] = PNL[i];
            PNLwithHedging_Simulated[i+n*101] = PNLwithHedging[i];
            HedgingError_Simulated[i+n*101] = HedgingError[i];
            BondPricesPaths[i+n*101] = BondPrices[i];
        }
    }

    cout<<"Output simulated stock prices paths."<<endl;
    std::ofstream Stock_Prices_Path("/Users/zyl/Desktop/StockPricesPaths_s.csv",ios::out);
    for (int n=0;n<1000;n++){
        for(int i=0; i<N+1;i++){
            Stock_Prices_Path<<StockPricesPaths[i+n*101]<<",";
        }
        Stock_Prices_Path<<endl;
    }
    Stock_Prices_Path.close();
    cout<<"Stock prices paths output finished."<<endl;

    cout<<"Output simulated cumulative heding errors simulated."<<endl;
    std::ofstream CumuHE("/Users/zyl/Desktop/CumulativeHedgingError_s.csv",ios::out);
    for (int n=0;n<1000;n++){
        for(int i=0; i<N+1;i++){
            CumuHE<<PNLwithHedging_Simulated[i+n*101]<<",";
        }
        CumuHE<<endl;
    }
    CumuHE.close();
    cout<<"Cumulative simulated heding errors output finished."<<endl;

//    std::ofstream OP("/Users/zyl/Desktop/Op.csv",ios::out);
//    for (int n=0;n<1000;n++){
//        for(int i=0; i<N+1;i++){
//            OP<<OptionPricesPaths[i+n*101]<<",";
//        }
//        OP<<endl;
//    }
//    OP.close();
//
//    std::ofstream bd("/Users/zyl/Desktop/bd.csv",ios::out);
//    for (int n=0;n<1000;n++){
//        for(int i=0; i<N+1;i++){
//            bd<<BondPricesPaths[i+n*101]<<",";
//        }
//        bd<<endl;
//    }
//    bd.close();
    
    
    //task2
    //Read real data from files
    vector<string> rf_date;
    vector<string> stock_date;
    vector<double> rf;
    vector<double> stockprices;
    vector<string> startdate;
    vector<string> exdate;
    vector<string> cpflag; //Whether it is call option or put option
    vector<double> strikeprices;
    vector<double> optionprices; //mean of best_bid and best_offer

    ifstream rinfile;//risk free rate
    rinfile.open("/Users/zyl/Desktop/interest.csv", ios::in);
    string line;
    if(rinfile.fail()){cout<<" File1 failed to open."<<endl;
        return 0;}
    else{
        vector<string> dall;
        while(getline(rinfile,line))
        {
            string d;
            istringstream readdata(line);
            while(getline(readdata,d,','))
            {
                dall.push_back(d);
            }
        }
        dall.erase(begin(dall),begin(dall)+2);//delete the text (1 row, 2 col) of the data
        for(int i = 0; i<dall.size(); i++)// seperate date and rate data into two vectors
         {
             if(i%2==1) rf.push_back(stod(dall[i]));
             else rf_date.push_back(dall[i]);
         }
    }
    rinfile.close();
    cout<<" File1 closed, "<<endl;

    ifstream sinfile;//stock price
    sinfile.open("/Users/zyl/Desktop/sec_GOOG.csv", ios::in);
    string sline;
    if(sinfile.fail()){cout<<" File2 failed to open."<<endl;
        return 0;}
    else{
        vector<string> dall;
        while(getline(sinfile,sline))
        {
            string d;
            istringstream readdata(sline);
            while(getline(readdata,d,','))
            {
                dall.push_back(d);
            }
        }
        dall.erase(begin(dall),begin(dall)+2);//delete the text (1 row, 2 col) of the data
        for(int i = 0; i<dall.size(); i++)// seperate date and rate data into two vectors
         {
             if(i%2==1) stockprices.push_back(stod(dall[i]));
             else stock_date.push_back(dall[i]);
         }
    }
    sinfile.close();
    cout<<" File2 closed, "<<endl;

    ifstream oinfile;//stock price
    oinfile.open("/Users/zyl/Desktop/op_GOOG.csv", ios::in);
    string oline;
    if(oinfile.fail()){cout<<" File1 failed to open."<<endl;
        return 0;}
    else{
        vector<string> dall;
        while(getline(oinfile,oline))
        {
            string d;
            istringstream readdata(oline);
            while(getline(readdata,d,','))
            {
                dall.push_back(d);
            }
        }
        dall.erase(begin(dall),begin(dall)+6);//delete the text (1 row, 6 col) of the data
        for(int i = 0; i<dall.size(); i++)// seperate date and rate data into two vectors
         {
             if(i%6==0) startdate.push_back(dall[i]);
             if(i%6==1) exdate.push_back(dall[i]);
             if(i%6==2) cpflag.push_back(dall[i]);
             if(i%6==3) strikeprices.push_back(stod(dall[i]));
             if(i%6==4) optionprices.push_back(0.5* (stod(dall[i])+stod(dall[i+1])) );
         }
    }
    oinfile.close();
    cout<<" File3 closed, "<<endl;

//    //cout<<rf_date[1]<<endl;///2011-01-04

    string t0 = "2011-07-05";
    string tN = "2011-07-29";
    string TT = "2011-09-17";
    K = 500; //strike price

    vector<string> flag;
    vector<string> start_date;
    vector<string> exp_date;
    vector<double> strike_prices_select;
    vector<double> option_prices_select;
    
    vector<double> stock_prices;
    vector<double> option_prices;
    vector<double> risk_free_rate;
    vector<string> Dates;
//
//    QuantLib::Date Expiration_Date(17,QuantLib::Sep,2011);
//    //QuantLib::Date Expiration_Date = QuantLib::DateParser::parseFormatted(TT,"%Y-%m-%d"); does not work
//    cout<<Expiration_Date<<endl;
//
//    //Find corresponding type, expiration date and strike price
    for(int i=0;i<startdate.size();i++){
        if(cpflag[i]=="C" && exdate[i]==TT && strikeprices[i]==K){
//            cout<<i<<"/"<<optionprices[i]<<endl;
//            cout<<cpflag[i]<<endl;
            flag.push_back(cpflag[i]);
            start_date.push_back(startdate[i]);
            exp_date.push_back(exdate[i]);
            strike_prices_select.push_back(strikeprices[i]);
            option_prices_select.push_back(optionprices[i]);

        }
        else continue;
    }
//
    //Hedging date start from t0 to tN, find corresponding date in option trading
    int start = finddatei(start_date, t0);
    int pause = finddatei(start_date, tN);
    int stop = start_date.size() - 1; //option starts later than this expdate would be filtered out in the previous step

    int startstock = finddatei(stock_date, t0);
    int pausetock = finddatei(stock_date, tN);
    
    for(int i=startstock;i<pausetock+1;i++){
            stock_prices.push_back(stockprices[i]);
            risk_free_rate.push_back(rf[i]);
    }
  
    vector<double> Time_To_Maturity;
    for(int i=start;i<pause+1;i++){
            option_prices.push_back(option_prices_select[i]);
            risk_free_rate.push_back(rf[i]);
            Dates.push_back(start_date[i]);
            double ti= (stop-i)*1.0/252.0;
            Time_To_Maturity.push_back(ti);
    }
    
    Option_Price Option(K,0.0,0.0,type);//the sigma and time to maturity will be overlapped in the successive steps
    vector<double> Implied_Volatility;
    vector<double> Delta_Real;
    Implied_Volatility = Option.Get_IV(stock_prices, option_prices, risk_free_rate, 0.0, 10.0, Time_To_Maturity);
    Delta_Real = Option.Get_Delta_Real();
    
//    QuantLib::Calendar OptionCalender = QuantLib::TARGET();
//    vector<QuantLib::Date> Date_Start;
//    for(int i=0;i<Dates.size()+1;i++){
//        QuantLib::Date d(stoi(Dates[i].substr(8,2)),QuantLib::Jul,stoi(Dates[i].substr(0,4)));
//        Time_To_Maturity.push_back(OptionCalender.businessDaysBetween(d,Expiration_Date,true,false)); }
    
    double d_t_real = 1.0/252.0;
    Delta_Hedging OptionHedging(d_t_real);
    OptionHedging.Bond(stock_prices, Delta_Real, risk_free_rate, option_prices);
    vector<double> PNL_With_HE_Option = OptionHedging.Cumulative_HE();
    vector<double> PNL_Option = OptionHedging.PNL();
    vector<double> Hedging_Error_Option = OptionHedging.HE();

    cout<<"Out put Result of Real Option"<<endl;
    std::ofstream Result("/Users/zyl/Desktop/Result.csv",ios::out);
    Result<<"Date"<<","<<"S"<<","<<"V"<<","<<"Implied_Volatility"<<","<<"Delta"<<","<<"Hedging Error"<<","<<"PNL"<<","<<"PNL with HE"<<endl;
    for(int i=0; i<Dates.size();i++){
        Result<<Dates[i]<<","<<stock_prices[i]<<","<<option_prices[i]<<","<<Implied_Volatility[i]<<","<<Delta_Real[i]<<","<<Hedging_Error_Option[i]<<","<<PNL_Option[i]<<","<<PNL_With_HE_Option[i]<<endl;
        }
    Result.close();
    cout<<"Result of real option output finished."<<endl;
    
    //Unit test
    test(0.2758, Implied_Volatility[0], "Test of calculating implied volatility");
    test(0.8099, Delta_Real[0], "Test of calculating delta");
    test(-0.6271, PNL_With_HE_Option[1], "Test of PNL with hedging error");
    
    return 0;
}


