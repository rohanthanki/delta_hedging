/*
Author: Rohan Chetan Thanki
Date created: 16-Oct-2021
*/

// This file contains the 'main' function. Program execution begins and ends there.

#include "Project_Parts.cpp"
#include "test.hpp"
#include <iostream>

using namespace std;

// main function
int main()
{
    runAllTests();

    cout << "Start of main" << endl;
    
    run_part_1();
    run_part_2();

    cout << "End of main" << endl;
    return 0;
}