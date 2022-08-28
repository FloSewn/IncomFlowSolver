/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#pragma once

#include <vector>

#include "Log.h"

#include "definitions.h"
#include "PrimaryGrid.h"
#include "BoundaryList.h"
#include "BoundaryDef.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;


/***********************************************************************
* Function to compute the center of a primary grid triangle 
***********************************************************************/
static inline void calc_tri_centroid(const double coords[3][2],
                                     double       center[2]);

/***********************************************************************
* Function to compute the edge centroids of a primary grid triangle 
***********************************************************************/
static inline void calc_tri_edge_centroids(const double coords[3][2],
                                           double       centroids[3][2]);

/***********************************************************************
* Function to compute the center of a primary grid quad 
***********************************************************************/
static inline void calc_quad_centroid(const double coords[4][2],
                                      double       center[2]);

/***********************************************************************
* Function to compute the edge centroids of a primary grid quad 
***********************************************************************/
static inline void calc_quad_edge_centroids(const double coords[4][2],
                                            double       centroids[4][2]);

/***********************************************************************
* 
***********************************************************************/
static inline void calc_tri_centroid(const double coords[3][2],
                                     double       center[2]) 
{
  center[0] = (coords[0][0] + coords[1][0] + coords[2][0]) / 3.0;
  center[1] = (coords[0][1] + coords[1][1] + coords[2][1]) / 3.0;
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_tri_edge_centroids(const double coords[3][2],
                                           double       centroids[3][2])
{
  centroids[0][0] = 0.5 * ( coords[0][0] + coords[1][0] );
  centroids[0][1] = 0.5 * ( coords[0][1] + coords[1][1] );

  centroids[1][0] = 0.5 * ( coords[1][0] + coords[2][0] );
  centroids[1][1] = 0.5 * ( coords[1][1] + coords[2][1] );

  centroids[2][0] = 0.5 * ( coords[2][0] + coords[0][0] );
  centroids[2][1] = 0.5 * ( coords[2][1] + coords[0][1] );
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_quad_centroid(const double coords[4][2],
                                      double       center[2])
{
  center[0] = 0.25 * ( coords[0][0] + coords[1][0] 
                     + coords[2][0] + coords[3][0] );
  center[1] = 0.25 * ( coords[0][1] + coords[1][1] 
                     + coords[2][1] + coords[3][1] );
}

/***********************************************************************
* 
***********************************************************************/
static inline void calc_quad_edge_centroids(const double coords[4][2],
                                            double       centroids[4][2])
{
  centroids[0][0] = 0.5 * ( coords[0][0] + coords[1][0] );
  centroids[0][1] = 0.5 * ( coords[0][1] + coords[1][1] );

  centroids[1][0] = 0.5 * ( coords[1][0] + coords[2][0] );
  centroids[1][1] = 0.5 * ( coords[1][1] + coords[2][1] );

  centroids[2][0] = 0.5 * ( coords[2][0] + coords[3][0] );
  centroids[2][1] = 0.5 * ( coords[2][1] + coords[3][1] );

  centroids[3][0] = 0.5 * ( coords[3][0] + coords[0][0] );
  centroids[3][1] = 0.5 * ( coords[3][1] + coords[0][1] );
}


/*********************************************************************
* This class represents a median dual grid
*********************************************************************/
class DualGrid
{
public:
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  DualGrid(const PrimaryGrid& pg, const BoundaryDef& bd) 
  : n_elements_     { pg.n_vertices()                          }
  , n_faces_        { pg.n_intr_edges() + pg.n_bdry_edges()    }
  , coords_         ( pg.n_vertices(),   2                     )
  , face_normals_   ( pg.n_intr_edges() + pg.n_bdry_edges(), 2 )
  , face_neighbors_ ( pg.n_intr_edges() + pg.n_bdry_edges(), 2 )
  , volumes_        ( pg.n_vertices(), 0.0                     )
  , boundaries_     { pg, bd }
  {
    // Initialize dual grid element center coordinates
    const DMat& vertex_coords = pg.vertex_coords();

    for (int i_elem = 0; i_elem < n_elements_; ++i_elem)
    {
      coords_[i_elem][0] = vertex_coords[i_elem][0];
      coords_[i_elem][1] = vertex_coords[i_elem][1];
    }

    // Initialize dual grid element face neighbor connectivity 
    const IMat& intr_edges = pg.intr_edges();
    const IMat& bdry_edges = pg.bdry_edges();
    int ni = pg.n_intr_edges();

    for ( int i_face = 0; i_face < pg.n_intr_edges(); ++i_face )
    {
      const int p0 = intr_edges[i_face][0];
      const int p1 = intr_edges[i_face][1];

      if ( p0 < p1 )
      {
        face_neighbors_[i_face][0] = p0;
        face_neighbors_[i_face][1] = p1;
      }
      else
      {
        face_neighbors_[i_face][0] = p1;
        face_neighbors_[i_face][1] = p0;
      }
    }

    for ( int i_face = 0; i_face < pg.n_bdry_edges(); ++i_face )
    {
      const int p0 = bdry_edges[i_face][0];
      const int p1 = bdry_edges[i_face][1];

      if ( p0 < p1 )
      {
        face_neighbors_[ni+i_face][0] = p0;
        face_neighbors_[ni+i_face][1] = p1;
      }
      else
      {
        face_neighbors_[ni+i_face][0] = p1;
        face_neighbors_[ni+i_face][1] = p0;
      }
    }

    // Initialize face normals and element volumes
    init_normals_and_volumes(pg);

  }

  /*------------------------------------------------------------------
  | Getters
  ------------------------------------------------------------------*/
  int n_elements() { return n_elements_; }
  int n_elements() const { return n_elements_; }

  int n_faces() { return n_faces_; }
  int n_faces() const { return n_faces_; }

  DMat& coords() { return coords_; }
  const DMat& coords() const { return coords_; }

  DMat& face_normals() { return face_normals_; }
  const DMat& face_normals() const { return face_normals_; }

  IMat& face_neighbors() { return face_neighbors_; }
  const IMat& face_neighbors() const { return face_neighbors_; }

  DVec& volumes() { return volumes_; }
  const DVec& volumes() const { return volumes_; }

  BoundaryList& boundaries() { return boundaries_; }
  const BoundaryList& boundaries() const { return boundaries_; }


private:
  /*------------------------------------------------------------------
  | Initialize the dual grid element volumes and the interior 
  | face normals
  ------------------------------------------------------------------*/
  void init_normals_and_volumes(const PrimaryGrid& pg)
  {
    const IMat& tris   = pg.tris();
    const IMat& quads  = pg.quads();
    const DMat& coords = pg.vertex_coords();

    // Count the total number of joint dualgrid faces of each median
    // dual element
    IVec face_per_elem ( n_elements_, 0 );

    for ( int i_face = 0; i_face < n_faces_; ++i_face )
    {
      const int p0 = face_neighbors_[i_face][0];
      const int p1 = face_neighbors_[i_face][1];

      ++face_per_elem[p0];
      ++face_per_elem[p1];
    }

    // Count maximum occuring face number of all elements
    // --> Needed for memory allocation
    int n_max_faces = 0;

    for ( int i_elem = 0; i_elem < n_elements_; ++i_elem )
      n_max_faces = MAX( n_max_faces, face_per_elem[i_elem] );

    // Create connectivity list between dual elements and their 
    // corresponding joint median dual faces
    IMat elem_to_face ( n_elements_, n_max_faces );

    for ( int i_face = 0; i_face < n_faces_; ++i_face )
    {
      const int p0 = face_neighbors_[i_face][0];
      const int p1 = face_neighbors_[i_face][1];

      const int off_p0 = elem_to_face[p0][0] + 1;
      const int off_p1 = elem_to_face[p1][0] + 1;

      elem_to_face[p0][off_p0] =  i_face;
      elem_to_face[p1][off_p1] =  i_face;

      ++elem_to_face[p0][0];
      ++elem_to_face[p1][0];
    }

    // Initialize arrays for element volume and face normals
    for ( int i_elem = 0; i_elem < n_elements_; ++i_elem )
      volumes_[i_elem] = 0.0;

    for ( int i_face = 0; i_face < n_faces_; ++i_face )
      face_normals_[i_face][0] = face_normals_[i_face][1] = 0.0;

    // Compute dualgrid metrics that arise from triangular elements
    for ( int i_tri = 0; i_tri < pg.n_tris(); ++i_tri )
    {
      const int *tri = tris[i_tri];

      double tri_coords[3][2] = {
        { coords[tri[0]][0], coords[tri[0]][1] },
        { coords[tri[1]][0], coords[tri[1]][1] },
        { coords[tri[2]][0], coords[tri[2]][1] },
      };

      double tri_centroid[2]      = { 0.0 };
      double edge_centroids[3][2] = { 0.0 };

      calc_tri_centroid(tri_coords, tri_centroid);
      calc_tri_edge_centroids(tri_coords, edge_centroids);

      // Loop over all tri edgs and compute forward sub-triangles
      for ( int i_edge = 0; i_edge < 3; ++i_edge )
      {
        // Local vertex indices -> range from 0 to 2 
        const int p0_loc = i_edge;
        const int p1_loc = MOD(i_edge+1,3);

        // Global vertex indices 
        int p0 = tri[p0_loc];
        int p1 = tri[p1_loc];

        const double a0[2] = {
          edge_centroids[i_edge][0] - tri_coords[p0_loc][0],
          edge_centroids[i_edge][1] - tri_coords[p0_loc][1],
        };

        const double b0[2] = {
          tri_centroid[0] - tri_coords[p0_loc][0],
          tri_centroid[1] - tri_coords[p0_loc][1],
        };

        const double a1[2] = {
          edge_centroids[i_edge][0] - tri_coords[p1_loc][0],
          edge_centroids[i_edge][1] - tri_coords[p1_loc][1],
        };

        const double b1[2] = {
          tri_centroid[0] - tri_coords[p1_loc][0],
          tri_centroid[1] - tri_coords[p1_loc][1],
        };

        // Sub-triangle area 
        const double area0 = 0.5 * ( a0[0] * b0[1] - a0[1] * b0[0] ) ;
        const double area1 = 0.5 * ( a1[0] * b1[1] - a1[1] * b1[0] ) ;

        volumes_[p0] += area0;
        volumes_[p1] -= area1;


        // Normal contribution of sub-triangle interface 
        // --> Rotation in CCW 
        const double norm[2] = {
          -edge_centroids[i_edge][1] + tri_centroid[1],
           edge_centroids[i_edge][0] - tri_centroid[0],
        };

        
        // Find global index of current face and compute face normal 
        // --> Normal points from p0 to p1 
        int *faces_p0 = elem_to_face[p0];

        for ( int k = 0; k < faces_p0[0]; k++ )
        {
          int i_face = faces_p0[k+1];

          if ( face_neighbors_[i_face][0] == p1 )
          {
            face_normals_[i_face][0] -= norm[0];
            face_normals_[i_face][1] -= norm[1];
            break;
          }

          if ( face_neighbors_[i_face][1] == p1 )
          {
            face_normals_[i_face][0] += norm[0];
            face_normals_[i_face][1] += norm[1];
            break;
          }

        } // for ( k = 0 ... )
 
      } // for ( i_edge = 0 ... )

    } // for ( i_tri = 0 ... )



    // Compute dualgrid metrics that arise from quadrilateral elements
    for ( int i_quad = 0; i_quad < pg.n_quads(); ++i_quad )
    {
      const int *quad = quads[i_quad];

      double quad_coords[4][2] = {
        { coords[quad[0]][0], coords[quad[0]][1] },
        { coords[quad[1]][0], coords[quad[1]][1] },
        { coords[quad[2]][0], coords[quad[2]][1] },
        { coords[quad[3]][0], coords[quad[3]][1] },
      };

      double quad_centroid[2]     = { 0.0 };
      double edge_centroids[4][2] = { 0.0 };

      calc_quad_centroid(quad_coords, quad_centroid);
      calc_quad_edge_centroids(quad_coords, edge_centroids);

      // Loop over all quad edges and compute forward sub-triangles 
      for ( int i_edge = 0; i_edge < 4; i_edge++ )
      {
        // Local vertex indices -> range from 0 to 2 
        const int p0_loc = i_edge;
        const int p1_loc = MOD(i_edge+1,4);

        // Global vertex indices 
        int p0 = quad[p0_loc];
        int p1 = quad[p1_loc];

        const double a0[2] = {
          edge_centroids[i_edge][0] - quad_coords[p0_loc][0],
          edge_centroids[i_edge][1] - quad_coords[p0_loc][1],
        };

        const double b0[2] = {
          quad_centroid[0] - quad_coords[p0_loc][0],
          quad_centroid[1] - quad_coords[p0_loc][1],
        };

        const double a1[2] = {
          edge_centroids[i_edge][0] - quad_coords[p1_loc][0],
          edge_centroids[i_edge][1] - quad_coords[p1_loc][1],
        };

        const double b1[2] = {
          quad_centroid[0] - quad_coords[p1_loc][0],
          quad_centroid[1] - quad_coords[p1_loc][1],
        };

        // Sub-triangle area 
        const double area0 = 0.5 * ( a0[0] * b0[1] - a0[1] * b0[0] ) ;
        const double area1 = 0.5 * ( a1[0] * b1[1] - a1[1] * b1[0] ) ;

        volumes_[p0] += area0;
        volumes_[p1] -= area1;


        // Normal contribution of sub-triangle interface 
        // --> Rotation in CCW 
        const double norm[2] = {
          -edge_centroids[i_edge][1] + quad_centroid[1],
           edge_centroids[i_edge][0] - quad_centroid[0],
        };

        // Find global index of current face and compute face normal 
        // --> Normal points from p0 to p1 
        int *faces_p0 = elem_to_face[p0];

        for ( int k = 0; k < faces_p0[0]; k++ )
        {
          int i_face = faces_p0[k+1];

          if ( face_neighbors_[i_face][0] == p1 )
          {
            face_normals_[i_face][0] -= norm[0];
            face_normals_[i_face][1] -= norm[1];
            break;
          }

          if ( face_neighbors_[i_face][1] == p1 )
          {
            face_normals_[i_face][0] += norm[0];
            face_normals_[i_face][1] += norm[1];
            break;
          }
        } // for ( k = 0 ... )

      } // for ( i_edge = ... )

    } // for ( i_quad = 0 ... )

  } // init_normals_and_volumes()

  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  int          n_elements_;
  int          n_faces_;

  DMat         coords_;
  DMat         face_normals_;

  IMat         face_neighbors_;

  DVec         volumes_;

  BoundaryList boundaries_;


}; // DualGrid

} // namespace Solver
} // namespace IncomFlow
