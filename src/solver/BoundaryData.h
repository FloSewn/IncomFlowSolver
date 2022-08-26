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

class Boundary;

/*********************************************************************
* This class is a simple container for all field variables 
* stored at a boundary
*********************************************************************/
class BoundaryData
{
  friend Boundary;

public:
  using VecArray = std::array<DVec, N_MAX_VARS>;
  using MatArray = std::array<DMat, N_MAX_VARS>;
  
  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  BoundaryData() {}

  /*------------------------------------------------------------------
  | Getters
  ------------------------------------------------------------------*/
  DVec& var(int ivar) { return var_[ivar]; }
  const DVec& var(int ivar) const { return var_[ivar]; }

  DVec& mflux(int ivar) { return mflux_[ivar]; }
  const DVec& mflux(int ivar) const { return mflux_[ivar]; }

  DVec& dep_var(int ivar) { return dep_var_[ivar]; }
  const DVec& dep_var(int ivar) const { return dep_var_[ivar]; }

  DMat& grad(int ivar) { return grad_[ivar]; }
  const DMat& grad(int ivar) const { return grad_[ivar]; }

  DMat& hess(int ivar) { return hess_[ivar]; }
  const DMat& hess(int ivar) const { return hess_[ivar]; }

protected:
  /*------------------------------------------------------------------
  | Initialize memory
  ------------------------------------------------------------------*/
  void init_structure(int n_bdry_elements)
  {
    for ( int ivar = 0; ivar < N_MAX_VARS; ++ivar ) 
    {
      var_[ivar].resize( n_bdry_elements );
      mflux_[ivar].resize( n_bdry_elements );
      dep_var_[ivar].resize( n_bdry_elements );

      grad_[ivar].resize( n_bdry_elements, 2 );
      hess_[ivar].resize( n_bdry_elements, 3 );
    }
  } // init_structure()

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

} // namespace Solver
} // namespace IncomFlow
