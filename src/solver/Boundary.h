/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#pragma once

#include <array>

#include "Log.h"

#include "definitions.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* This class is a simple container for all field variables 
* stored at a boundary
*********************************************************************/
class BoundaryData
{
public:
  using VecArray = std::array<DVec, N_MAX_VARS>;
  using MatArray = std::array<DMat, N_MAX_VARS>;
  
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  BoundaryData(int n_bdry_elements)
  {
    // Initialize data
    for ( int ivar = 0; ivar < N_MAX_VARS; ++ivar ) 
    {
      var_[ivar].resize( n_bdry_elements );
      mflux_[ivar].resize( n_bdry_elements );
      dep_var_[ivar].resize( n_bdry_elements );

      grad_[ivar].resize( n_bdry_elements, 2 );
      hess_[ivar].resize( n_bdry_elements, 3 );
    }
  }

  /*------------------------------------------------------------------
  | Getters
  ------------------------------------------------------------------*/
  DVec& var(int ivar) { return var_[ivar]; }
  DVec& mflux(int ivar) { return mflux_[ivar]; }
  DVec& dep_var(int ivar) { return dep_var_[ivar]; }

  DMat& grad(int ivar) { return grad_[ivar]; }
  DMat& hess(int ivar) { return hess_[ivar]; }


private:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  VecArray  var_     {};
  VecArray  mflux_   {};
  VecArray  dep_var_ {};

  MatArray  grad_    {};
  MatArray  hess_    {};

}; // BoundaryData


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
  Boundary(int marker, BdryType type, int n_elems, int n_edges) 
  : marker_          { marker     }
  , type_            { type       }
  , n_dual_elements_ { n_elems    }
  , n_prim_edges_    { n_edges    }
  , dual_elements_   ( n_elems    )
  , prim_edges_      ( n_edges, 2 )
  , dual_normals_    ( n_elems, 2 )
  , bdry_data_       { n_elems    }
  {}

private:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  int          marker_;
  BdryType     type_;

  int          n_dual_elements_;
  int          n_prim_edges_;

  IVec         dual_elements_;
  IMat         prim_edges_;

  DMat         dual_normals_;
  BoundaryData bdry_data_;



}; //Boundary

} // namespace Solver
} // namespace IncomFlow
