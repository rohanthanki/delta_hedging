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

void testDelta ()
{
	const double K = 250;
	const double S = 248;
	const double r = 0.03;
	const double T = 1;
	const double sigma = 0.3;
	const double trueDelta = 0.5882;
	Hedging_Portfolio hpobj1(K, S, r, T, sigma, 'c');
	double computedDelta = hpobj1.computeDelta();
	if (abs(trueDelta - computedDelta) <= 0.01)
		cout << "Delta test case passed" << endl;
	else
		cout << "Delta test case failed" << endl;
}

void testImpliedVol()
{
	const double K = 250;
	const double S = 248;
	const double r = 0.03;
	const double T = 1;
	const double sigma = 0;

	const double marketPrice = 46.41;
	double maxVol = 100;
	const double trueImpliedVol = 0.45;

	Hedging_Portfolio hpobj1(K, S, r, T, sigma, 'c');
	hpobj1.setOptionPrice(marketPrice);
	double computedImpliedVol = hpobj1.computeImpliedVol(maxVol);

	if (abs(trueImpliedVol - computedImpliedVol) <= 0.01)
		cout << "Implied volatility test case passed" << endl;
	else
		cout << "Implied volatility test case failed" << endl;
}

void runAllTests()
{
	testDelta();
	testImpliedVol();
}