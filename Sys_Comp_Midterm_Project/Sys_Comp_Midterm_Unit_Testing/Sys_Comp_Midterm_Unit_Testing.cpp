#include "pch.h"
#include "CppUnitTest.h"
#include "../Sys_Comp_Midterm_Project/Option.hpp"
#include "../Sys_Comp_Midterm_Project/Hedging_Portfolio.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SysCompMidtermUnitTesting
{
	TEST_CLASS(Hedging_Portfolio)
	{
	public:

		TEST_METHOD(Test_Delta)
		{
			const double K = 250;
			const double S = 248;
			const double r = 0.03;
			const double T = 1;
			const double sigma = 1;
			const double mktPrice = 17.52;

			const double callPriceTrue = 8.864;
			const double putPriceTrue = 16.686;
			
			Hedging_Portfolio hpobj1;
			hpobj1

			
			
			//Hedging_Portfolio hpobj2(K, S, r, T, sigma, 'p');
			
			//Assert::IsTrue(abs(callPrice - callPriceTrue) < 0.01);
			//Assert::IsTrue(abs(putPrice - putPriceTrue) < 0.01);
			Assert::IsTrue(true);
		}
	};
}
