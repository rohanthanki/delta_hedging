/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

/*
This contains the signature of the Hedging_Portfolio class
*/

#include "Option.hpp"
#include <string>
#include <iostream>

using namespace std;

#ifndef Hedging_Portfolio_HPP
#define Hedging_Portfolio_HPP

class Hedging_Portfolio : public Option
{
private:
	// Member attributes
	double delta;			// option delta
	double optionPrice;		// option price
	double B;				// B value
	double HE;				// hedging error
	string date;			// pricing date
	string expDate;			// expiry date
	double pnlNaked;		// PNL without hedging
	double pnlHedged;		// PNL with hedging

public:

	// Constructors and Destructors
	Hedging_Portfolio(void);
	Hedging_Portfolio(const double&, const double&, const double&, const double&, const double&, const char&);
	virtual ~Hedging_Portfolio(void);

	// Getters
	double getDelta(void) const;
	double getOptionPrice(void) const;
	double getB(void) const;
	double getHedgingError(void) const;
	string getDate(void) const;
	string getExpDate(void) const;
	double getpnlNaked(void) const;
	double getpnlHedged(void) const;

	// Setters
	void setDelta(const double&);
	void setOptionPrice(const double&);
	void setB(const double&);
	void setHedgingError(const double&);
	void setDate(const string&);
	void setExpDate(const string&);
	void setpnlNaked(const double&);
	void setpnlHedged(const double&);

	// Other functions
	double computeDelta(void) const;
	double computeBlackScholesOptionPrice(const double&) const;
	double computeImpliedVol(double&) const;
};

#endif