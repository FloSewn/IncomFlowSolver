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

namespace PrimaryGridTests 
{
using namespace CppUtils;
using namespace IncomFlow::Solver;

/*********************************************************************
*
*********************************************************************/
void read_grid()
{
  // Set logging output file
  std::string base_dir { INCOMFLOW_SOURCE_DIR };
  std::string log_file_path 
  { base_dir + "/aux/test_logs/PrimaryGrid.read_grid.log" };
  LOG_PROPERTIES.set_info_ostream( TO_FILE, log_file_path );
  LOG_PROPERTIES.set_debug_ostream( TO_FILE, log_file_path );

  // Read the grid
  std::string grid_file_path 
  { base_dir + "/aux/test_data/TestGrid.dat" };

  PrimaryGridReader grid_reader {};

  PrimaryGrid primary_grid = grid_reader.read( grid_file_path );

  // Check grid attributes & array lengths
  CHECK( primary_grid.n_vertices() == 24 );
  CHECK( primary_grid.n_tris() == 6 );
  CHECK( primary_grid.n_quads() == 12 );
  CHECK( primary_grid.n_intr_edges() == 25 );
  CHECK( primary_grid.n_bdry_edges() == 16 );

  CHECK( primary_grid.vertex_coords().rows() == 24 );

  CHECK( primary_grid.tris().rows() == 6 );
  CHECK( primary_grid.quads().rows() == 12 );

  CHECK( primary_grid.tri_neighbors().rows() == 6 );
  CHECK( primary_grid.quad_neighbors().rows() == 12 );

  CHECK( primary_grid.intr_edges().rows() == 25 );
  CHECK( primary_grid.bdry_edges().rows() == 16 );

  CHECK( primary_grid.intr_edge_neighbors().rows() == 25 );
  CHECK( primary_grid.bdry_edge_neighbors().size() == 16 );


  // Check primary grid element connectivites
  CHECK( primary_grid.tris()[0][0] == 16 );
  CHECK( primary_grid.tris()[1][1] == 18 );
  CHECK( primary_grid.tris()[2][2] == 21 );

  CHECK( primary_grid.quads()[0][0] == 1  );
  CHECK( primary_grid.quads()[1][1] == 3  );
  CHECK( primary_grid.quads()[2][2] == 19 );
  CHECK( primary_grid.quads()[3][3] == 19 );

  CHECK( primary_grid.tri_neighbors()[0][0] == 16 );
  CHECK( primary_grid.tri_neighbors()[1][1] == 16 );
  CHECK( primary_grid.tri_neighbors()[2][2] == 3  );

  CHECK( primary_grid.quad_neighbors()[0][0] == 1  );
  CHECK( primary_grid.quad_neighbors()[1][1] == 13 );
  CHECK( primary_grid.quad_neighbors()[2][2] == 8  );
  CHECK( primary_grid.quad_neighbors()[3][3] == -1 );

  CHECK( primary_grid.intr_edges()[0][0] == 17 );
  CHECK( primary_grid.intr_edges()[1][1] == 3  );

  CHECK( primary_grid.intr_edge_neighbors()[2][0] == 2 );
  CHECK( primary_grid.intr_edge_neighbors()[3][1] == 2 );

  CHECK( primary_grid.bdry_edges()[0][0] == 0  );
  CHECK( primary_grid.bdry_edges()[1][1] == 2  );

  CHECK( primary_grid.bdry_edge_neighbors()[2] == 1 );

  CHECK( primary_grid.bdry_edge_markers()[3] == 1 );

} // read_grid()

} // namespace PrimaryGridTests


/*********************************************************************
* Run tests for: PrimaryGrid.h
*********************************************************************/
void run_tests_PrimaryGrid()
{
  PrimaryGridTests::read_grid();

  // Reset logging ostream
  CppUtils::LOG_PROPERTIES.set_info_ostream( CppUtils::TO_COUT );
  CppUtils::LOG_PROPERTIES.set_debug_ostream( CppUtils::TO_COUT );

} // run_tests_PrimaryGrid()
