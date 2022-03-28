/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

/*
This contains the implementation of the Hedging_Portfolio class
*/

#include "Hedging_Portfolio.hpp"
#include <string>
#include <boost/math/distributions.hpp>

using namespace std;

/******************************* Constructors and Destructors ***************************************/

// Default constructor
Hedging_Portfolio::Hedging_Portfolio() : Option() { }

// Parametrised constructor
Hedging_Portfolio::Hedging_Portfolio(const double& K1, const double& S1, const double& r1, const double& T1, const double& vol1, const char& optionType) : Option(K1, S1, r1, T1, vol1, optionType)
{
	if (!(optionType == 'c' || optionType == 'C' || optionType == 'p' || optionType == 'P'))
		throw(20);
}

// Destructor
Hedging_Portfolio::~Hedging_Portfolio(void) { }

/******************************* Getters ***************************************/

double Hedging_Portfolio::getDelta(void) const { return delta; }
double Hedging_Portfolio::getOptionPrice(void) const { return optionPrice; }
double Hedging_Portfolio::getB(void) const { return B; }
double Hedging_Portfolio::getHedgingError(void) const { return HE; }

string Hedging_Portfolio::getDate(void) const { return date; }
string Hedging_Portfolio::getExpDate(void) const { return expDate; }
double Hedging_Portfolio::getpnlNaked(void) const { return pnlNaked; }
double Hedging_Portfolio::getpnlHedged(void) const { return pnlHedged; }

/******************************* Setters ***************************************/

void Hedging_Portfolio::setDelta(const double& delta1) { delta = delta1; }
void Hedging_Portfolio::setOptionPrice(const double& optionPrice1) { optionPrice = optionPrice1; }
void Hedging_Portfolio::setB(const double& B1) { B = B1; }
void Hedging_Portfolio::setHedgingError(const double& HE1) { HE = HE1; }

void Hedging_Portfolio::setDate(const string& date1) { date = date1; }
void Hedging_Portfolio::setExpDate(const string& expDate1) { expDate = expDate1; }
void Hedging_Portfolio::setpnlNaked(const double& pnlNaked1) { pnlNaked = pnlNaked1; }
void Hedging_Portfolio::setpnlHedged(const double& pnlHedged1) { pnlHedged = pnlHedged1; }

/******************************* Other Functions ***************************************/

// Get CDF of Standard Normal
inline double N(const double& x)
{
	boost::math::normal_distribution<> stdNormal(0.0, 1.0);
	return(cdf(stdNormal, x));
}

double Hedging_Portfolio::computeDelta(void) const
{
	char flag = getFlag();
	double K = getStrikePrice();
	double S = getSpotPrice();
	double r = getRiskFreeRate();
	double T = getTimeToMaturity();
	double sigma = getVolatility();
	double d1 = (log(S / K) + ((r + (pow(sigma, 2) / 2)) * T)) / (sigma * sqrt(T));
	if (flag == 'c' || flag == 'C')
		return (N(d1));
	else if (flag == 'p' || flag == 'P')
		return (N(d1));
	else
		throw(10);
}

// Compute option price using Black Scholes formula
double Hedging_Portfolio::computeBlackScholesOptionPrice(const double& sigma) const
{
	char flag = getFlag();
	double K = getStrikePrice();
	double S = getSpotPrice();
	double r = getRiskFreeRate();
	double T = getTimeToMaturity();

	Option opt1(K, S, r, T, sigma, flag);

	double d1 = (log(S / K) + ((r + (pow(sigma, 2) / 2)) * T)) / (sigma * sqrt(T));
	double d2 = d1 - (sigma * sqrt(T));

	if (flag == 'c' || flag == 'C')
		return (S * N(d1)) - (K * exp(-r * T) * N(d2));
	else if (flag == 'p' || flag == 'P')
		return (K * exp(-r * T) * N(-d2)) - (S * N(-d1));
	else
		throw(10);
}

//double getImpliedVol(const double&, const double&, const double&, 
double Hedging_Portfolio:: computeImpliedVol(double& volMax) const
{
	double flag = getFlag();
	double volMin = 0;
	double impVol;
	double TMat = getTimeToMaturity();
	double epsilon = 0.001;
	double modelPrice;
	string temp;
	
	while (true)
	{
		impVol = 0.5 * (volMin + volMax);
		modelPrice = computeBlackScholesOptionPrice(impVol);

		if (modelPrice > optionPrice + epsilon)
			volMax = impVol;
		else if (modelPrice < optionPrice - epsilon)
			volMin = impVol;
		else
			return(impVol);
	}
}