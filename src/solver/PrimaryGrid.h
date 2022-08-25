/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#pragma once

#include <string>
#include <fstream>

#include "Log.h"

#include "Matrix.h"

namespace IncomFlow {
namespace Solver {

class PrimaryGridReader;

using namespace CppUtils;

/*********************************************************************
* This class represents the PrimaryGrid
*********************************************************************/
class PrimaryGrid
{
  friend PrimaryGridReader;

public:
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  PrimaryGrid(int n_vertices, int n_tris, int n_quads,
              int n_intr_edges, int n_bdry_edges) 
  : n_vertices_          { n_vertices      }
  , n_tris_              { n_tris          }
  , n_quads_             { n_quads         }
  , n_intr_edges_        { n_intr_edges    }
  , n_bdry_edges_        { n_bdry_edges    }
  , vertex_coords_       ( n_vertices, 2   )
  , tris_                ( n_tris, 3       )
  , quads_               ( n_quads, 4      )
  , tri_neighbors_       ( n_tris, 3       )
  , quad_neighbors_      ( n_quads, 4      )
  , intr_edges_          ( n_intr_edges, 2 )
  , bdry_edges_          ( n_bdry_edges, 2 )
  , intr_edge_neighbors_ ( n_intr_edges, 2 )
  , bdry_edge_neighbors_ ( n_bdry_edges    )
  , bdry_edge_markers_   ( n_bdry_edges    )
  {}

  /*------------------------------------------------------------------
  | Copy 
  ------------------------------------------------------------------*/
  PrimaryGrid(const PrimaryGrid& pg)
  : n_vertices_          { pg.n_vertices_          }
  , n_tris_              { pg.n_tris_              }
  , n_quads_             { pg.n_quads_             }
  , n_intr_edges_        { pg.n_intr_edges_        }
  , n_bdry_edges_        { pg.n_bdry_edges_        }
  , vertex_coords_       { pg.vertex_coords_       }
  , tris_                { pg.tris_                }
  , quads_               { pg.quads_               }
  , tri_neighbors_       { pg.tri_neighbors_       }
  , quad_neighbors_      { pg.quad_neighbors_      }
  , intr_edges_          { pg.intr_edges_          }
  , bdry_edges_          { pg.bdry_edges_          }
  , intr_edge_neighbors_ { pg.intr_edge_neighbors_ }
  , bdry_edge_neighbors_ { pg.bdry_edge_neighbors_ }
  , bdry_edge_markers_   { pg.bdry_edge_markers_   }
  {}

  /*------------------------------------------------------------------
  | Move 
  ------------------------------------------------------------------*/
  PrimaryGrid(PrimaryGrid&& pg)
  : n_vertices_          { pg.n_vertices_                     }
  , n_tris_              { pg.n_tris_                         }
  , n_quads_             { pg.n_quads_                        }
  , n_intr_edges_        { pg.n_intr_edges_                   }
  , n_bdry_edges_        { pg.n_bdry_edges_                   }
  , vertex_coords_       { std::move(pg.vertex_coords_)       }
  , tris_                { std::move(pg.tris_ )               }
  , quads_               { std::move(pg.quads_)               }
  , tri_neighbors_       { std::move(pg.tri_neighbors_)       }
  , quad_neighbors_      { std::move(pg.quad_neighbors_)      }
  , intr_edges_          { std::move(pg.intr_edges_)          }
  , bdry_edges_          { std::move(pg.bdry_edges_)          }
  , intr_edge_neighbors_ { std::move(pg.intr_edge_neighbors_) }
  , bdry_edge_neighbors_ { std::move(pg.bdry_edge_neighbors_) }
  , bdry_edge_markers_   { std::move(pg.bdry_edge_markers_)   }
  {}

  /*------------------------------------------------------------------
  | Getters
  ------------------------------------------------------------------*/
  int n_vertices() { return n_vertices_; }
  int n_vertices() const { return n_vertices_; }

  int n_tris() { return n_tris_; }
  int n_tris() const { return n_tris_; }

  int n_quads() { return n_quads_; }
  int n_quads() const { return n_quads_; }

  int n_intr_edges() { return n_intr_edges_; }
  int n_intr_edges() const { return n_intr_edges_; }

  int n_bdry_edges() { return n_bdry_edges_; }
  int n_bdry_edges() const { return n_bdry_edges_; }


  Matrix<double>& vertex_coords() { return vertex_coords_; }
  const Matrix<double>& vertex_coords() const { return vertex_coords_; }

  Matrix<int>& tris() { return tris_; }
  const Matrix<int>& tris() const { return tris_; }

  Matrix<int>& quads() { return quads_; }
  const Matrix<int>& quads() const { return quads_; }

  Matrix<int>& tri_neighbors() { return tri_neighbors_; }
  const Matrix<int>& tri_neighbors() const { return tri_neighbors_; }

  Matrix<int>& quad_neighbors() { return quad_neighbors_; }
  const Matrix<int>& quad_neighbors() const { return quad_neighbors_; }

  Matrix<int>& intr_edges() { return intr_edges_; }
  const Matrix<int>& intr_edges() const { return intr_edges_; }

  Matrix<int>& bdry_edges() { return bdry_edges_; }
  const Matrix<int>& bdry_edges() const { return bdry_edges_; }

  Matrix<int>& intr_edge_neighbors() { return intr_edge_neighbors_; }
  const Matrix<int>& intr_edge_neighbors() const { return intr_edge_neighbors_; }

  std::vector<int>& bdry_edge_neighbors() { return bdry_edge_neighbors_; }
  const std::vector<int>& bdry_edge_neighbors() const { return bdry_edge_neighbors_; }

  std::vector<int>& bdry_edge_markers() { return bdry_edge_markers_; }
  const std::vector<int>& bdry_edge_markers() const { return bdry_edge_markers_; }


protected:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  int n_vertices_   { 0 };
  int n_tris_       { 0 };
  int n_quads_      { 0 };
  int n_intr_edges_ { 0 };
  int n_bdry_edges_ { 0 };

  Matrix <double>  vertex_coords_;

  Matrix<int>      tris_;
  Matrix<int>      quads_;

  Matrix<int>      tri_neighbors_;
  Matrix<int>      quad_neighbors_;

  Matrix<int>      intr_edges_;
  Matrix<int>      bdry_edges_;

  Matrix<int>      intr_edge_neighbors_;
  std::vector<int> bdry_edge_neighbors_;

  std::vector<int> bdry_edge_markers_;

};


} // namespace Solver
} // namespace IncomFlow
