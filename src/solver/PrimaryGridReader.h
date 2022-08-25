/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Log.h"

#include "Matrix.h"

#include "PrimaryGrid.h"
#include "solver_utils.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* This class is used to read a PrimaryGrid 
*********************************************************************/
class PrimaryGridReader
{
public:
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  PrimaryGridReader() {}

  /*------------------------------------------------------------------
  | Load grid from file
  ------------------------------------------------------------------*/
  PrimaryGrid read(const std::string& file_path)
  {
    LOG(INFO) <<  "Reading primary grid file: " << file_path;

    // Read grid attributes
    std::ifstream file ( file_path );

    int n_vertices   = read_grid_attribute(file, "VERTICES");
    int n_intr_edges = read_grid_attribute(file, "INTERIOREDGES");
    int n_bdry_edges = read_grid_attribute(file, "BOUNDARYEDGES");
    int n_quads      = read_grid_attribute(file, "QUADS");
    int n_tris       = read_grid_attribute(file, "TRIANGLES");
    
    LOG(INFO) << "Number of vertices: "
              << n_vertices;
    LOG(INFO) << "Number of interior edges: "
              << n_intr_edges;
    LOG(INFO) << "Number of boundary edges: "
              << n_bdry_edges;
    LOG(INFO) << "Number of quads: "
              << n_quads;
    LOG(INFO) << "Number of triangles: "
              << n_tris;

    // Init new primary grid
    PrimaryGrid grid { n_vertices, n_tris, n_quads, 
                       n_intr_edges, n_bdry_edges };

    // Load the actual grid data
    bool state = true;
    state = read_vertex_coords( file_path, grid );
    state = read_tris(file_path, grid);
    state = read_quads(file_path, grid);
    state = read_tri_neighbors(file_path, grid);
    state = read_quad_neighbors(file_path, grid);
    state = read_intr_edges(file_path, grid);
    state = read_bdry_edges(file_path, grid);

    if (!state)
      TERMINATE();

    return std::move(grid);

  } // PrimaryGridReader::read()

private:
  /*------------------------------------------------------------------
  | Read the number of defined entities in a mesh file
  ------------------------------------------------------------------*/
  int read_grid_attribute(std::ifstream& infile, 
                          const std::string& entity_tag)
  {
    std::string line;

    // Obtain the total number of triangles 
    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 
      
      std::string key;
      int val; 

      if ( (ss >> key >> val) && (key == entity_tag) )
        return val;
    }

    return 0;

  } // read_entity_number()

  /*------------------------------------------------------------------
  | Read the grid vertex coordinates 
  ------------------------------------------------------------------*/
  bool read_vertex_coords(const std::string& file_path,
                          PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read vertex coorinates
    std::string line;
    int i_vertex = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until vertex coordinates are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "VERTICES") )
          read_data = true;
        continue;
      }

      double x, y; 
      char d;

      // Read data & handle invalid formatting
      if ( ss >> x >> d >> y )
      {
        grid.vertex_coords_[i_vertex][0] = x;
        grid.vertex_coords_[i_vertex][1] = y;
        ++i_vertex;
      }

      // All vertices loaded
      if ( i_vertex == grid.n_vertices_ )
        break;
    }

    // Error if data was not read
    if ( i_vertex != grid.n_vertices_ )
    {
      LOG(ERROR) << "Failed to read primary grid vertices from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_vertex_coords() 

  /*------------------------------------------------------------------
  | Read the grid triangles  
  ------------------------------------------------------------------*/
  bool read_tris(const std::string& file_path,
                 PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read triangle indices
    std::string line;
    int i_tri = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until triangles are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "TRIANGLES") )
          read_data = true;
        continue;
      }

      int i1, i2, i3, c; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> i3 >> d >> c) )
      {
        grid.tris_[i_tri][0] = i1;
        grid.tris_[i_tri][1] = i2;
        grid.tris_[i_tri][2] = i3;
        ++i_tri;
      }

      // All triangles loaded
      if ( i_tri == grid.n_tris_ )
        break;
    }

    // Error if data was not read
    if ( i_tri != grid.n_tris_ )
    {
      LOG(ERROR) << "Failed to read primary grid triangles from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_tris() 

  /*------------------------------------------------------------------
  | Read the grid quads  
  ------------------------------------------------------------------*/
  bool read_quads(const std::string& file_path,
                 PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read quad indices
    std::string line;
    int i_quad = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until quads are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "QUADS") )
          read_data = true;
        continue;
      }

      int i1, i2, i3, i4, c; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> i3 >> d >> i4 >> d >> c) )
      {
        grid.quads_[i_quad][0] = i1;
        grid.quads_[i_quad][1] = i2;
        grid.quads_[i_quad][2] = i3;
        grid.quads_[i_quad][3] = i4;
        ++i_quad;
      }

      // All quads loaded
      if ( i_quad == grid.n_quads_ )
        break;
    }

    // Error if data was not read
    if ( i_quad != grid.n_quads_ )
    {
      LOG(ERROR) << "Failed to read primary grid quads from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_quads() 

  /*------------------------------------------------------------------
  | Read the grid triangle neighbor connectivity  
  ------------------------------------------------------------------*/
  bool read_tri_neighbors(const std::string& file_path,
                          PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read triangle neighbor indices
    std::string line;
    int i_tri = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until triangles are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "TRIANGLENEIGHBORS") )
          read_data = true;
        continue;
      }

      int i1, i2, i3; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> i3) )
      {
        grid.tri_neighbors_[i_tri][0] = i1;
        grid.tri_neighbors_[i_tri][1] = i2;
        grid.tri_neighbors_[i_tri][2] = i3;
        ++i_tri;
      }

      // All triangles loaded
      if ( i_tri == grid.n_tris_ )
        break;
    }

    // Error if data was not read
    if ( i_tri != grid.n_tris_ )
    {
      LOG(ERROR) << "Failed to read primary grid triangles from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_tri_neighbors() 

  /*------------------------------------------------------------------
  | Read the grid quad neighbor connectivity  
  ------------------------------------------------------------------*/
  bool read_quad_neighbors(const std::string& file_path,
                          PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read quad indices
    std::string line;
    int i_quad = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until quads are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "QUADNEIGHBORS") )
          read_data = true;
        continue;
      }

      int i1, i2, i3, i4; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> i3 >> d >> i4) )
      {
        grid.quad_neighbors_[i_quad][0] = i1;
        grid.quad_neighbors_[i_quad][1] = i2;
        grid.quad_neighbors_[i_quad][2] = i3;
        grid.quad_neighbors_[i_quad][3] = i4;
        ++i_quad;
      }

      // All quads loaded
      if ( i_quad == grid.n_quads_ )
        break;
    }

    // Error if data was not read
    if ( i_quad != grid.n_quads_ )
    {
      LOG(ERROR) << "Failed to read primary grid quads from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_quad_neighbors() 

  /*------------------------------------------------------------------
  | Read the interior edges  
  ------------------------------------------------------------------*/
  bool read_intr_edges(const std::string& file_path,
                       PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read interior edge indices
    std::string line;
    int i_edge = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until triangles are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "INTERIOREDGES") )
          read_data = true;
        continue;
      }

      int i1, i2, n1, n2; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> n1 >> d >> n2) )
      {
        grid.intr_edges_[i_edge][0] = i1;
        grid.intr_edges_[i_edge][1] = i2;
        grid.intr_edge_neighbors_[i_edge][0] = n1;
        grid.intr_edge_neighbors_[i_edge][1] = n2;
        ++i_edge;
      }

      // All triangles loaded
      if ( i_edge == grid.n_intr_edges_ )
        break;
    }

    // Error if data was not read
    if ( i_edge != grid.n_intr_edges_ )
    {
      LOG(ERROR) << "Failed to read interior primary grid edges from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_intr_edges() 

  /*------------------------------------------------------------------
  | Read the boundary edges  
  ------------------------------------------------------------------*/
  bool read_bdry_edges(const std::string& file_path,
                       PrimaryGrid& grid)
  {
    std::ifstream infile;
    infile.open( file_path );

    // Check for invalid files
    if ( infile.fail() )
    {
      LOG(ERROR) << "Failed to open primary grid file:\n"
                    "  \"" << file_path << "\"";
      return false;
    }

    // Read interior edge indices
    std::string line;
    int i_edge = 0;
    bool read_data = false;

    while ( std::getline(infile, line) )
    {
      std::stringstream ss { line }; 

      // Pass through file until triangles are defined
      if ( !read_data )
      {
        std::string key;
        int val; 
        if ( (ss >> key >> val) && (key == "BOUNDARYEDGES") )
          read_data = true;
        continue;
      }

      int i1, i2, n, m; 
      char d;

      // Read data & handle invalid formatting
      if ( (ss >> i1 >> d >> i2 >> d >> n >> d >> m) )
      {
        grid.bdry_edges_[i_edge][0] = i1;
        grid.bdry_edges_[i_edge][1] = i2;
        grid.bdry_edge_neighbors_[i_edge] = n;
        grid.bdry_edge_markers_[i_edge] = m;
        ++i_edge;
      }

      // All triangles loaded
      if ( i_edge == grid.n_bdry_edges_ )
        break;
    }

    // Error if data was not read
    if ( i_edge != grid.n_bdry_edges_ )
    {
      LOG(ERROR) << "Failed to read primary grid boundary edges from the "
                    "provided file:\n  \"" << file_path << "\"";
      return false;
    }

    return true;

  } // PrimaryGridReader::read_bdry_edges() 



  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/

}; // PrimaryGridReader



} // namespace Solver
} // namespace IncomFlow
