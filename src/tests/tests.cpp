/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#include <iostream>
#include <vector>
#include <string>

#include "tests.h"
#include "Log.h"
#include "Testing.h"

/*********************************************************************
* Log utils
*********************************************************************/
using CppUtils::LOG;
using CppUtils::LogLevel::INFO;
using CppUtils::LogColor::GREEN;
using CppUtils::LogColor::RED;

/*********************************************************************
* The main test function
*********************************************************************/
int run_tests(const std::string& test_case)
{
  /*------------------------------------------------------------------
  | Print header
  ------------------------------------------------------------------*/
  LOG(INFO) << "";
  LOG(INFO) << "   -------------------------   ";
  LOG(INFO) << "   |  IncomFlow - Test suite  |   ";
  LOG(INFO) << "   -------------------------   ";
  LOG(INFO) << "";

  /*------------------------------------------------------------------
  | Run all tests
  ------------------------------------------------------------------*/
  if ( !test_case.compare("PrimaryGrid") )
  {
    LOG(INFO) << "  Running tests for \"PrimaryGrid\" class...";
    run_tests_PrimaryGrid();
  }
  else
  {
    LOG(INFO) << "";
    LOG(INFO, RED) << "  No test case \"" << test_case  << "\" found";
    LOG(INFO) << "";
    return EXIT_FAILURE;
  }

  /*------------------------------------------------------------------
  | Check for failed tests
  ------------------------------------------------------------------*/
  std::vector<CppUtils::TestData>& test_data 
    = CppUtils::TestDataSingleton::instance();

  bool   state = true;
  size_t error_count = 0;
  size_t total_tests = test_data.size();

  for (auto data : test_data )
  {
    if ( !data.state() )
    {
      ++error_count;
      LOG(INFO,RED) << "[ERROR] Test (" << error_count 
              << "/" << total_tests << ") failed.";
      LOG(INFO) << "        --> " << data;
    }
    state &= data.state();
  }

  /*------------------------------------------------------------------
  | Succeess / fail
  ------------------------------------------------------------------*/
  LOG(INFO) << "";
  if (!state)
  {
    LOG(INFO, RED) << "  --> (" << error_count << "/" 
            << total_tests << ") tests failed." ;
  }
  else
  {
    LOG(INFO, GREEN) << "  --> (" << total_tests-error_count << "/" 
            << total_tests << ") tests succeeded.";
  }
  LOG(INFO) << "";
  LOG(INFO) << "";

  if (!state)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;

} // run_tests()

