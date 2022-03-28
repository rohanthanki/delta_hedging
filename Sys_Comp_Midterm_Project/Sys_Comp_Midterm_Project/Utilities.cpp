/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

// This file contains functions which are used in the main file. This ensures that the main file is not cluttered

#include "Hedging_Portfolio.hpp"
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/random.hpp>
#include <boost/math/distributions.hpp>
#include <boost/date_time.hpp>

using namespace std;

/**************************************** GENERIC UTIL FUNCTIONS ****************************************************/

// split a string into substrings based on a delimiter
inline vector<string> string_splitter(const string& str, char delimiter)
{
    vector<string> str_vec;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter))
    {
        str_vec.push_back(token);
    }
    return str_vec;
}

// Writing a 2D Vector to a CSV file
inline void write2DVectorToCSV(const vector<vector<double>>& vect, const string& filepath)
{
    std::ofstream out(filepath);
    for (int i = 0; i <= vect.size() - 1; i++)
    {
        for (int j = 0; j <= vect[0].size() - 1; j++)
        {
            out << vect[i][j] << ",";
        }
        out << "\n";
    }
}

// Get CDF of Standard Normal
inline double N(const double& x)
{
    boost::math::normal_distribution<> stdNormal(0.0, 1.0);
    return(cdf(stdNormal, x));
}

// print a 2D vector to console
template <typename T>
inline void printVect(vector<vector<T>> vect)
{
    for (int i = 0; i <= vect[0].size() - 1; i++)
    {
        for (int j = 0; j <= vect.size() - 1; j++)
        {
            cout << vect[j][i] << "\t";
        }
        cout << "\n";
    }
}

// convert vector of string to vector of double
inline vector<double> stringToDoubleVect(const vector<string>& inVect)
{
    vector<double> outVect;
    for (int i = 0; i <= inVect.size() - 1; i++)
    {
        outVect.push_back(stod(inVect[i]));
    }
    return(outVect);
}

// convert vector of string to vector of dates
inline vector<boost::gregorian::date> stringtoDateVect(const vector<string>& inVect)
{
    vector<boost::gregorian::date> outVect;
    for (int i = 0; i <= inVect.size() - 1; i++)
    {
        outVect.push_back(boost::gregorian::from_simple_string(inVect[i]));
    }
    return(outVect);
}

// function to read a CSV where each column is returned as a vector
inline vector<vector<string>> readCSV(const string& filepath, const unsigned int& ignoreLines, const int& numCols)
{
    ifstream infile(filepath);
    string line;

    // ignore the first few lines of the file
    for (int lineCount = 1; lineCount <= ignoreLines && infile.good(); lineCount++)
        getline(infile, line);

    // initialising vectors which will be populated when the file is read
    vector<vector<string>> csvData;
    for (int i = 0; i <= numCols - 1; i++)
    {
        vector<string> temp;
        csvData.push_back(temp);
    }

    // Read the data line by line
    while (getline(infile, line))
    {
        vector<string> rowData = string_splitter(line, ',');
        for (int i = 0; i <= numCols - 1; i++)
        {
            csvData[i].push_back(rowData[i]);
        }
    }

    infile.close();
    return(csvData);
}

/************************************ FUNCTIONS SPECIFIC TO PART 1 ****************************************************/

// Simulate stock prices
inline vector<vector<double>> simulateStockPrices(unsigned int numPaths, unsigned int N, double S0, double T, double u, double sigma)
{
    double dt = T / N;          // time step
    vector < vector<double> > stockPrices;          // creating a vector to store stock prices for all paths

    boost::random::mt19937 rng;     // creating a random number generator
    //rng.seed(static_cast<unsigned int> (std::time(0)));     // setting the seed of the random number generator
    rng.seed(123);
    boost::random::normal_distribution<double> stdNormal(0, 1);     // creating the distribution object

    for (int i = 0; i <= numPaths - 1; i++)
    {
        vector<double> pathStockPrices;             // creating a vector to store stock prices of the current path
        double S = S0;
        pathStockPrices.push_back(S);

        // add simulated stock prices of the current path to the  current path vector
        for (int j = 1; j <= N; j++)
        {
            S = (S)+(S * u * dt) + (S * sigma * sqrt(dt) * stdNormal(rng));
            pathStockPrices.push_back(S);
        }

        // add the stock prices of the current path to the overall stock price vector
        stockPrices.push_back(pathStockPrices);
    }

    return(stockPrices);
}

// Computing Hedging Error of Hedged Portfolio
inline vector<vector<Hedging_Portfolio>> computeHedgingErrors(const vector<vector<double>>& stockPrices, const double& K, const double& r, const double& T, const double& sigma, const char& optionFlag)
{
    unsigned int numPaths = stockPrices.size(); // number of paths
    unsigned int pathLength = stockPrices[0].size() - 1;
    double dt = T / pathLength;     // time step

    vector<vector<Hedging_Portfolio>> allHedgingPortfolio;     // creating a vector to store the details of the hedging error

    for (int i = 0; i <= numPaths - 1; i++)
    {
        vector<Hedging_Portfolio> pathHedgingPortfolio;
        for (int j = 0; j <= pathLength; j++)
        {
            // creating variables
            double S = stockPrices[i][j];
            double TMat = T - (j * dt);
            double deltaPrev, BPrev, rPrev;
            double delta, V, B, HE;

            // computing d1 and d2 for Black Scholes model
            double d1 = (log(S / K) + ((r + (pow(sigma, 2) / 2)) * TMat)) / (sigma * sqrt(TMat));
            double d2 = d1 - (sigma * sqrt(TMat));

            // creating an object of the hedged portfolio at the current state
            Hedging_Portfolio portfolio(K, S, r, TMat, sigma, optionFlag);

            // setting delta
            delta = N(d1);
            portfolio.setDelta(delta);

            // setting option Price
            if (optionFlag == 'c' || optionFlag == 'C')
                V = (S * N(d1)) - (K * exp(-r * TMat) * N(d2));
            else if (optionFlag == 'p' || optionFlag == 'P')
                V = (K * exp(-r * TMat) * N(-d2)) - (S * N(-d1));
            else
            {
                cout << "Flag " << optionFlag << "is incorrect. Flag must be either 'c', 'C', 'p' or 'P'" << endl;
                throw(10);
            }
            portfolio.setOptionPrice(V);

            // setting B
            if (j == 0)
            {
                B = V - delta * S;
            }
            else
            {
                rPrev = pathHedgingPortfolio[j - 1].getRiskFreeRate();
                deltaPrev = pathHedgingPortfolio[j - 1].getDelta();
                BPrev = pathHedgingPortfolio[j - 1].getB();
                B = ((deltaPrev - delta) * S) + BPrev * exp(rPrev * dt);
            }
            portfolio.setB(B);

            // setting Hedging Error
            if (j == 0)
            {
                HE = 0;
            }
            else
            {
                HE = deltaPrev * S + BPrev * exp(rPrev * dt) - V;
            }
            portfolio.setHedgingError(HE);

            // adding current state of the portfolio to the vector of states of the portfolio for current path
            pathHedgingPortfolio.push_back(portfolio);
        }

        // adding the path of portfolio to the vector containinf portfolio states of all paths
        allHedgingPortfolio.push_back(pathHedgingPortfolio);
    }
    return(allHedgingPortfolio);
}

// function to create a 2D Vector of a specific attribute from the hedged portfolio state vector
inline vector<vector<double>> getHedgedPortfolioParam(const vector<vector<Hedging_Portfolio>>& allPortfolioStates, const string param)
{
    vector<vector<double>> allVect;
    int numPaths = allPortfolioStates.size();
    int pathSize = allPortfolioStates[0].size() - 1;


    for (int i = 0; i <= numPaths - 1; i++)
    {
        vector<double> pathVect;
        for (int j = 0; j <= pathSize; j++)
        {
            double val;

            if (param == "S")
                val = allPortfolioStates[i][j].getSpotPrice();
            else if (param == "DELTA")
                val = allPortfolioStates[i][j].getDelta();
            else if (param == "V")
                val = allPortfolioStates[i][j].getOptionPrice();
            else if (param == "B")
                val = allPortfolioStates[i][j].getB();
            else if (param == "HE")
                val = allPortfolioStates[i][j].getHedgingError();
            else
                val = 0;

            pathVect.push_back(val);
        }
        allVect.push_back(pathVect);
    }

    return(allVect);
}

/************************************** FUNCTIONS SPECIFIC TO PART 2 ****************************************************/

// Count the number of weekdays between 2 days
inline long countWeekDays(string d0str, string d1str)
{
    boost::gregorian::date d0(boost::gregorian::from_simple_string(d0str));
    boost::gregorian::date d1(boost::gregorian::from_simple_string(d1str));
    long ndays = (d1 - d0).days() + 1; // +1 for inclusive
    long nwkends = 2 * ((ndays + d0.day_of_week()) / 7); // 2*Saturdays
    if (d0.day_of_week() == boost::date_time::Sunday) ++nwkends;
    if (d1.day_of_week() == boost::date_time::Saturday) --nwkends;
    return ndays - nwkends;
}

// finding corresponding value from a vector given the date
inline double findVal(vector<string> dateVec, vector<double> valVec, string date)
{
    auto it = find(dateVec.begin(), dateVec.end(), date);
    return valVec.at(it - dateVec.begin());  // this will throw an exception if array is out of bounds
}
