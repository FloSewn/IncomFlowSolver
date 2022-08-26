/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#include <iostream>
#include <cassert>

#include <IncomFlowConfig.h>

#include "tests.h"

#include "Testing.h"

#include "PrimaryGrid.h"
#include "PrimaryGridReader.h"
#include "DualGrid.h"
#include "BoundaryDef.h"

#include "definitions.h"

namespace DualGridTests 
{
using namespace CppUtils;
using namespace IncomFlow::Solver;

std::string BASE_DIR { INCOMFLOW_SOURCE_DIR };

/*********************************************************************
*
*********************************************************************/
void boundaries()
{
  LOG(INFO) << "";
  LOG(INFO) << "========== Test: boundaries() ==========";
  LOG(INFO) << "";

  BoundaryDef bdry_def {};

  bdry_def.add_marker( 1, BdryType::INLET  );
  bdry_def.add_marker( 2, BdryType::WALL   );
  bdry_def.add_marker( 3, BdryType::OUTLET );
  bdry_def.add_marker( 4, BdryType::WALL   );

  // ---------------- Test BoundaryDef --------------------
  bdry_def.add_marker( 5, BdryType::PERIODIC );
  bdry_def.remove_marker( 5 );

  CHECK( bdry_def.size() == 4 );
  CHECK( bdry_def.get_boundary_type(5) == BdryType::INVALID );
  CHECK( bdry_def.get_boundary_type(1) == BdryType::INLET );
  CHECK( bdry_def.get_boundary_type(2) == BdryType::WALL );
  CHECK( bdry_def.get_boundary_type(3) == BdryType::OUTLET );
  CHECK( bdry_def.get_boundary_type(4) == BdryType::WALL );

  // Test iteration over BoundaryDef
  int m = 1;
  for ( const auto key_val : bdry_def ) 
  {
    // Check for correct order
    CHECK( key_val.first == m );
    ++m;

    // Check for correct key-value pairs
    if ( key_val.first == 1 )
      CHECK( key_val.second == BdryType::INLET );

    if ( key_val.first == 2 )
      CHECK( key_val.second == BdryType::WALL );

    if ( key_val.first == 3 )
      CHECK( key_val.second == BdryType::OUTLET );

    if ( key_val.first == 4 )
      CHECK( key_val.second == BdryType::WALL );
  }

  // -----------------------------------------------------------------
  // Read the grid
  std::string grid_file_path 
  { BASE_DIR + "/aux/test_data/TestGrid.dat" };

  PrimaryGridReader grid_reader {};

  PrimaryGrid primary_grid = grid_reader.read( grid_file_path );

  DualGrid dual_grid { primary_grid, bdry_def };


} // boundaries()

} // namespace DualGridTests


/*********************************************************************
* Run tests for: PrimaryGrid.h
*********************************************************************/
void run_tests_DualGrid()
{
  // Set logging output file
  std::string log_file_path 
  { DualGridTests::BASE_DIR + "/aux/test_logs/tests_DualGrid.log" };
  CppUtils::LOG_PROPERTIES.set_info_ostream( CppUtils::TO_FILE, log_file_path );
  CppUtils::LOG_PROPERTIES.set_debug_ostream( CppUtils::TO_FILE, log_file_path );

  DualGridTests::boundaries();

  // Reset logging ostream
  CppUtils::LOG_PROPERTIES.set_info_ostream( CppUtils::TO_COUT );
  CppUtils::LOG_PROPERTIES.set_debug_ostream( CppUtils::TO_COUT );

} // run_tests_DualGrid()
