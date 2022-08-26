/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#pragma once

#include "Log.h"
#include "Helpers.h"

#include "PrimaryGrid.h"
#include "BoundaryData.h"
#include "definitions.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* This class describes a boundary of the median dual grid
*
* Boundary structure:
* -------------------
*
*               bdry_norm[1] -> here is bdry_mflux[1] stored
*
*                  ^       ^
*             e1   :       :    e2          
*     x-------o--------x--------o--------x
*  v1 |\      :         v2                v3
*     |  \    :   
*     |    \  :               
*     |______\:                                                    
*     |                      
*
*  bdry_points = [v1, v2, v3, ...]
*  bdry_edges  = [(v1,v2), (v2,v3), ...]
*
*********************************************************************/
class Boundary
{
public:
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  Boundary(const PrimaryGrid& pgrid, int marker, BdryType type) 
  : marker_          { marker     }
  , type_            { type       }
  {
    init_structure( pgrid );
    compute_normals( pgrid );
  }


  /*------------------------------------------------------------------
  | Getter
  ------------------------------------------------------------------*/
  int marker() { return marker_; }
  int marker() const { return marker_; }

  BdryType type() { return type_; }
  BdryType type() const { return type_; }

  int n_dual_elements() { return n_dual_elements_; }
  int n_dual_elements() const { return n_dual_elements_; }

  int n_prim_edges() { return n_prim_edges_; }
  int n_prim_edges() const { return n_prim_edges_; }

  IVec& dual_elements() { return dual_elements_; }
  const IVec& dual_elements() const { return dual_elements_; }

  IMat& prim_edges() { return prim_edges_; }
  const IMat& prim_edges() const { return prim_edges_; }

  DMat& dual_normals() { return dual_normals_; }
  const DMat& dual_normals() const { return dual_normals_; }

  BoundaryData& bdry_data() { return bdry_data_; }
  const BoundaryData& bdry_data() const { return bdry_data_; }


private:
  /*------------------------------------------------------------------
  | Initialize the boundary structure 
  ------------------------------------------------------------------*/
  void init_structure(const PrimaryGrid& pgrid)
  {
    // Obtain the number of primary grid boundary edges, that are
    // actually associated to the marker of this boundary
    IVec associated_markers ( pgrid.n_vertices(), 0 );

    const IMat& bdry_edges        = pgrid.bdry_edges();
    const IVec& bdry_edge_markers = pgrid.bdry_edge_markers();

    n_prim_edges_ = 0;

    for (int i_edge = 0; i_edge < pgrid.n_bdry_edges(); ++i_edge )
    {
      if (bdry_edge_markers[i_edge] != marker_)
        continue;

      int i0 = bdry_edges[i_edge][0];
      int i1 = bdry_edges[i_edge][1];

      ASSERT( i0 < pgrid.n_vertices(), 
      "Boundary edge exceeeds maximum number of primary grid vertices.");

      ASSERT( i1 < pgrid.n_vertices(), 
      "Boundary edge exceeeds maximum number of primary grid vertices.");

      ++n_prim_edges_;
      associated_markers[ i0 ] += 1;
      associated_markers[ i1 ] += 1;
    }


    // Count the actual number of median dual elements that are
    // associated to the marker of this boundary.
    n_dual_elements_ = 0;

    for ( int i_elem = 0; i_elem < pgrid.n_vertices(); ++i_elem ) 
      if ( associated_markers[i_elem] > 0 )
        ++n_dual_elements_;

    // Allocate memory for the boundary data structure
    dual_elements_.resize( n_dual_elements_ );
    prim_edges_local_.resize( n_prim_edges_, 2 );
    prim_edges_.resize( n_prim_edges_, 2 );
    dual_normals_.resize( n_dual_elements_, 2);
    bdry_data_.init_structure( n_dual_elements_ );


    // Obtain the indices of associated  median dual elements  
    // of the current boundary
    int i_bdry_elem = 0;
    for ( int i_elem = 0; i_elem < pgrid.n_vertices(); ++i_elem )
    {
      if ( associated_markers[i_elem] < 1 )
        continue;

      dual_elements_[i_bdry_elem] = i_elem;

      ++i_bdry_elem;
    }

    // Obtain the connectivity of associated primary grid 
    // boundary edges of the current boundary
    int i_bdry_edge = 0;
    for ( int i_edge = 0; i_edge < pgrid.n_bdry_edges(); ++i_edge )
    {
      if ( bdry_edge_markers[i_edge] != marker_ )
        continue;

      int i0 = bdry_edges[i_edge][0];
      int i1 = bdry_edges[i_edge][1];

      prim_edges_[i_bdry_edge][0] = i0;
      prim_edges_[i_bdry_edge][1] = i1;

      ++i_bdry_edge;
    }


    // Estimate the local indices (bounary frame) of the 
    // associated primary grid vertices 
    IVec global_to_local ( pgrid.n_vertices(), -1 );

    for ( int i_elem = 0; i_elem < n_dual_elements_; ++i_elem )
    {
      const int i = dual_elements_[i_elem];
      global_to_local[i] = i_elem;
    }

    for ( int i_edge = 0; i_edge < n_prim_edges_; ++i_edge )
    {
      const int i0 = prim_edges_[i_edge][0];
      const int i1 = prim_edges_[i_edge][1];

      prim_edges_local_[i_edge][0] = global_to_local[ i0 ];
      prim_edges_local_[i_edge][1] = global_to_local[ i1 ];
    }

  } // init_structure()

  /*------------------------------------------------------------------
  | Compute the boundary face normals that are associated to 
  | median dual elements
  | -> Normals are defined such that they point into the domain's
  |    interior!
  ------------------------------------------------------------------*/
  void compute_normals(const PrimaryGrid& pgrid)
  {
    const DMat& coords = pgrid.vertex_coords();

    for ( int i_elem = 0; i_elem < n_dual_elements_; ++i_elem )
    {
      dual_normals_[i_elem][0] = 0.0;
      dual_normals_[i_elem][1] = 0.0;
    }

    
    for ( int i_edge = 0; i_edge < n_prim_edges_; ++i_edge )
    {
      const int p0 = prim_edges_[i_edge][0];
      const int p1 = prim_edges_[i_edge][1];

      const double nx = 0.5 * (coords[p0][1] - coords[p1][1]);
      const double ny = 0.5 * (coords[p1][0] - coords[p0][0]);

      const int i0 = prim_edges_local_[i_edge][0];
      const int i1 = prim_edges_local_[i_edge][1];

      dual_normals_[i0][0] += nx;
      dual_normals_[i0][1] += ny;

      dual_normals_[i1][0] += nx;
      dual_normals_[i1][1] += ny;
    }

    std::cout << "BOUNDARY: " << marker_ << std::endl;
    for ( int i_elem = 0; i_elem < n_dual_elements_; ++i_elem )
      std::cout << dual_elements_[i_elem] << " - ("
        << dual_normals_[i_elem][0] << ", " 
        << dual_normals_[i_elem][1] << ")\n"; 
    std::cout << "-----------------\n\n";

  } // compute_normals()

  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  int          marker_;
  BdryType     type_;

  int          n_dual_elements_;
  int          n_prim_edges_;

  IVec         dual_elements_;
  IMat         prim_edges_local_;  // Edges in local frame
  IMat         prim_edges_;        // Edges in global frame

  DMat         dual_normals_;

  BoundaryData bdry_data_;



}; //Boundary

} // namespace Solver
} // namespace IncomFlow
