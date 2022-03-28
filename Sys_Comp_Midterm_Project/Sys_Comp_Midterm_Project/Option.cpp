/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

/*
This contains the implementation of the Option class
*/

#include "Option.hpp"
#include <string>
#include <sstream>

using namespace std;

/******************************* Constructors and Destructors ***************************************/

// Default constructor
Option::Option() : K(0), S(0), r(0), T(0), vol(0) { }

// Copy constructor
Option::Option(const Option& obj) : K(obj.getStrikePrice()), S(obj.getSpotPrice()), r(obj.getRiskFreeRate()), T(obj.getTimeToMaturity()), vol(obj.getVolatility()) { }

// Parametrised constructor
Option::Option(const double& K1, const double& S1, const double& r1, const double& T1, const double& vol1, const char& flag1) : K(K1), S(S1), r(r1), T(T1), vol(vol1), flag(flag1) { }

// Destructor
Option::~Option(void) { }

/************************************ Getters  ***********************************************/

char Option::getFlag(void) const { return flag; }
double Option::getStrikePrice(void) const { return K; }
double Option::getSpotPrice(void) const { return S; }
double Option::getRiskFreeRate(void) const { return r; }
double Option::getTimeToMaturity(void) const { return T; }
double Option::getVolatility(void) const { return vol; }


/************************************ Setters  ***********************************************/

void Option::setFlag(const char& _flag) { flag = _flag; }
void Option::setStrikePrice(const double& strikePrice) { K = strikePrice; }
void Option::setSpotPrice(const double& spotPrice) { S = spotPrice; }
void Option::setRiskFreeRate(const double& riskFreeRate) { r = riskFreeRate; }
void Option::setTimeToMaturity(const double& timeToMaturity) { T = timeToMaturity; }
void Option::setVolatility(const double& volatility) { vol = volatility; }