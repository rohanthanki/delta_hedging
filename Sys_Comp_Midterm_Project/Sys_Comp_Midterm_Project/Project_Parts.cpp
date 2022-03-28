/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

// This file contains the 'main' function. Program execution begins and ends there.

#include "Hedging_Portfolio.hpp"
#include "Utilities.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include<boost/tokenizer.hpp>
#include <boost/date_time.hpp>
#include<boost/tokenizer.hpp>
#include <boost/date_time.hpp>

/*********************************************** PART 1 ******************************************************/
inline void run_part_1(void)
{
    cout << "\nStart of part 1\n";
    // creating variables
    unsigned int numPaths = 1000;
    unsigned int N = 100;
    double T = 0.4;
    double u = 0.05;
    double sigma = 0.24;
    double r = 0.025;
    double S0 = 100;
    double K = 105;
    char optionFlag = 'c';

    vector<vector<double>> stockPrices = simulateStockPrices(numPaths, N, S0, T, u, sigma);
    vector<vector<Hedging_Portfolio>> hedgedPortfolioAllStates = computeHedgingErrors(stockPrices, K, r, T, sigma, 'c');

    vector<vector<double>> callOptionPrices = getHedgedPortfolioParam(hedgedPortfolioAllStates, "V");
    vector<vector<double>> deltas = getHedgedPortfolioParam(hedgedPortfolioAllStates, "DELTA");
    vector<vector<double>> B = getHedgedPortfolioParam(hedgedPortfolioAllStates, "B");
    vector<vector<double>> hedgingErrors = getHedgedPortfolioParam(hedgedPortfolioAllStates, "HE");

    write2DVectorToCSV(stockPrices, "Output_Files/Part1/Stock_Prices.csv");
    write2DVectorToCSV(callOptionPrices, "Output_Files/Part1/Call_Option_Prices.csv");
    write2DVectorToCSV(deltas, "Output_Files/Part1/Call_Option_Deltas.csv");
    write2DVectorToCSV(B, "Output_Files/Part1/B.csv");
    write2DVectorToCSV(hedgingErrors, "Output_Files/Part1/Hedging_Errors.csv");

    cout << "End of part 1\n\n";
}

/*********************************************** PART 2 ******************************************************/
inline void run_part_2(void)
{
    // reading interest rate data
    vector<vector<string>> interestRateData = readCSV("Data/interest.csv", 1, 2);
    //vector<boost::gregorian::date> interestRateDateVec = stringtoDateVect(interestRateData[0]);
    vector<string> interestRateDateVec = interestRateData[0];
    vector<double> interestRateVec = stringToDoubleVect(interestRateData[1]);

    // reading stock data
    vector<vector<string>> stockData = readCSV("Data/sec_GOOG.csv", 1, 2);
    //vector<boost::gregorian::date> stockDateVec = stringtoDateVect(stockData[0]);
    vector<string> stockDateVec = stockData[0];
    vector<double> stockPriceVec = stringToDoubleVect(stockData[1]);

    // reading option data
    //vector<vector<string>> optionData = readCSV("Data/op_GOOG.csv", 1, 6);
    string optionFilePath = "Data/op_GOOG.csv";
    ifstream infile(optionFilePath);
    string line;
    vector<vector<string>> csvData;

    // get user input
    string startDateUser, endDateUser, expDateUser;
    char optionFlagUser;
    double strikePriceUser;

    /*startDateUser = "2011-07-05";
    endDateUser = "2011-07-29";
    expDateUser = "2011-09-17";
    optionFlagUser = 'c';
    strikePriceUser = 500;*/

    std::cout << "Enter start date in YYYY-MM-DD format: "; cin >> startDateUser;
    std::cout << "Enter end date in YYYY-MM-DD format: "; cin >> endDateUser;
    std::cout << "Enter maturity date in YYYY-MM-DD format: "; cin >> expDateUser;
    std::cout << "Enter option Flag: "; cin >> optionFlagUser;
    std::cout << "Enter strike price of the option: "; cin >> strikePriceUser;

    // Read options data line by line
    getline(infile, line);                              // skipping the first line
    vector<Hedging_Portfolio> allHedgingPortfolios;     // creating a vector to store the hedging portfolio at each date
    while (getline(infile, line))
    {
        // tokenising the row data
        vector<string> rowData = string_splitter(line, ',');

        // reading data in each row
        string date = rowData[0];
        string expDate = rowData[1];
        char flag = toupper(rowData[2][0]);
        double strikePrice = stod(rowData[3]);
        double bidPrice = stod(rowData[4]);
        double askPrice = stod(rowData[5]);
        double marketPrice = (bidPrice + askPrice) / 2;

        // skipping the loop if the details donot match user input
        if (!(strikePrice == strikePriceUser && expDate == expDateUser && flag == toupper(optionFlagUser) && date >= startDateUser && date <= endDateUser))
            continue;

        // creating a hedging portfolio object
        Hedging_Portfolio rowobj;

        //setting values in the object
        rowobj.setDate(date);                               // setting date
        rowobj.setExpDate(rowData[1]);                      // setting expiration date
        rowobj.setFlag(rowData[2][0]);                      // setting flag of the option
        rowobj.setStrikePrice(stod(rowData[3]));            // setting strike price
        rowobj.setOptionPrice(marketPrice);                 // setting market price        

        // setting the interest rate
        try
        {
            double rate = findVal(interestRateDateVec, interestRateVec, rowobj.getDate()) / 100;
            rowobj.setRiskFreeRate(rate);
        }
        catch (...)
        {
            std::cout << "No interest rate found for date " << rowobj.getDate() << endl;
        }

        // setting the stock price
        try
        {
            double spotPrice = findVal(stockDateVec, stockPriceVec, rowobj.getDate());
            rowobj.setSpotPrice(spotPrice);
        }
        catch (...)
        {
            std::cout << "No interest rate found for date " << rowobj.getDate() << endl;
        }

        // setting time to maturity
        rowobj.setTimeToMaturity(countWeekDays(rowobj.getDate(), rowobj.getDate()) * 1.0 / 252);

        // setting implied volatility
        double maxVol = 100;
        double impliedVol = rowobj.computeImpliedVol(maxVol);
        rowobj.setVolatility(impliedVol);
        //cout << date << "\t" << expDate << "\t" << flag << "\t" << strikePrice << "\t" << rowobj.getRiskFreeRate() << "\t" << rowobj.getSpotPrice() << "\t" << impliedVol << endl;

        // setting delta
        double delta = rowobj.computeDelta();
        rowobj.setDelta(delta);

        // add the object to the vector
        allHedgingPortfolios.push_back(rowobj);
    }
    infile.close();

    // creating a filestream object to write to a csv file
    string outputFilePath = "Output_Files/Part2/Real_Market_Data_Hedging.csv";
    vector<string> outputFileHeaders{ "Date", "Stock Price", "Option Price", "Implied Volatility", "Option Delta", "Hedging Error", "PNL Naked Short Call", "PNL Hedged Portfolio" };
    std::ofstream outfile(outputFilePath);
    for (int i = 0; i <= outputFileHeaders.size() - 1; i++)
        outfile << outputFileHeaders[i] << ",";
    outfile << "\n";

    // iterating through the vector to compute hedging errors
    for (int j = 0; j <= allHedgingPortfolios.size() - 1; j++)
    {
        // creating variables
        double S = allHedgingPortfolios[j].getSpotPrice();
        double TMat = allHedgingPortfolios[j].getTimeToMaturity();
        double delta = allHedgingPortfolios[j].getDelta();
        double V = allHedgingPortfolios[j].getOptionPrice();
        double dt = 1.0 / 252;
        double deltaPrev, BPrev, rPrev, B, HE;

        // setting B
        if (j == 0)
            B = V - delta * S;
        else
        {
            rPrev = allHedgingPortfolios[j - 1].getRiskFreeRate();
            deltaPrev = allHedgingPortfolios[j - 1].getDelta();
            BPrev = allHedgingPortfolios[j - 1].getB();
            B = ((deltaPrev - delta) * S) + BPrev * exp(rPrev * dt);
        }
        allHedgingPortfolios[j].setB(B);

        // setting Hedging Error
        if (j == 0)
            HE = 0;
        else
            HE = deltaPrev * S + BPrev * exp(rPrev * dt) - V;

        allHedgingPortfolios[j].setHedgingError(HE);

        // Setting PNL for naked short call
        double pnlNaked =  allHedgingPortfolios[0].getOptionPrice() - allHedgingPortfolios[j].getOptionPrice();
        allHedgingPortfolios[j].setpnlNaked(pnlNaked);

        // Setting PNL for hedged portfolio
        double pnlHedged = HE;
        allHedgingPortfolios[j].setpnlHedged(pnlHedged);

        // writing to output file
        outfile << allHedgingPortfolios[j].getDate() << ",";
        outfile << allHedgingPortfolios[j].getSpotPrice() << ",";
        outfile << allHedgingPortfolios[j].getOptionPrice() << ",";
        outfile << allHedgingPortfolios[j].getVolatility() << ",";
        outfile << allHedgingPortfolios[j].getDelta() << ",";
        outfile << allHedgingPortfolios[j].getHedgingError() << ",";
        outfile << allHedgingPortfolios[j].getpnlNaked() << ",";
        outfile << allHedgingPortfolios[j].getpnlHedged() << endl;
    }

    outfile.close();
    cout << "\nEnd of part 2\n";
}