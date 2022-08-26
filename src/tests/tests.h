/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#pragma once

#include <vector>
#include <string>

/*********************************************************************
* The main test function
*********************************************************************/
int run_tests(const std::string& library);

/*********************************************************************
* Test functions
*********************************************************************/
void run_tests_PrimaryGrid();
void run_tests_DualGrid();
