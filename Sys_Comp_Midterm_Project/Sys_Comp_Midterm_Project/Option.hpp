/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

/*
This contains the signature of the Option class
*/

#include <string>
#include <iostream>

using namespace std;

#ifndef Option_HPP
#define Option_HPP

class Option
{
private:
	char flag;		// to denote whether call or put option
	double K;		// strike price
	double S;		// spot price
	double r;		// risk free rate
	double T;		// time to maturity
	double vol;		// annualised volatility

public:
	// Contructors and Destructors
	Option(void);											// default constructor
	Option(const Option&);									// copy constructor
	Option(const double&, const double&, const double&, const double&, const double&, const char&);		// parametrised constructor
	virtual ~Option();										// destructor

	// Getters
	char getFlag(void) const;
	double getStrikePrice(void) const;
	double getSpotPrice(void) const;
	double getRiskFreeRate(void) const;
	double getTimeToMaturity(void) const;
	double getVolatility(void) const;

	// Setters
	void setFlag(const char&);
	void setStrikePrice(const double&);
	void setSpotPrice(const double&);
	void setRiskFreeRate(const double&);
	void setTimeToMaturity(const double&);
	void setVolatility(const double&);
};

#endif