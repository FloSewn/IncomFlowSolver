/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/
#pragma once

#include <functional>
#include <cassert>
#include <float.h>
#include <limits.h>
#include <cstdlib>

#include "Helpers.h"

namespace IncomFlow {
namespace Solver {

/*********************************************************************
* This class is a container for all constants that are used 
* in the solver code
*********************************************************************/
class SolverConstants
{
public:
  /*------------------------------------------------------------------
  | Default constructor
  ------------------------------------------------------------------*/
  SolverConstants() {}

  /*------------------------------------------------------------------
  | Setters 
  ------------------------------------------------------------------*/
  inline void dummy(double s)  
  { dummy_ = s; }

  /*------------------------------------------------------------------
  | Getters 
  ------------------------------------------------------------------*/
  inline double dummy() const 
  { return dummy_; }

  inline double dummy_fixed() const 
  { return dummy_fixed_; }


private:
  /*------------------------------------------------------------------
  | Adjustable attributes 
  ------------------------------------------------------------------*/
  double dummy_  = 0.0;

  /*------------------------------------------------------------------
  | Fixed attributes 
  ------------------------------------------------------------------*/
  int dummy_fixed_ = 0.0;

}; // SolverConstants

inline SolverConstants CONSTANTS;

/*********************************************************************
* CONSTANTS
*********************************************************************/
constexpr double INCOMFLOW_SMALL  = 1.0E-13; //DBL_EPSILON;
constexpr double INCOMFLOW_MAX    = DBL_MAX;
constexpr double INCOMFLOW_MIN    = DBL_MIN;

/*********************************************************************
* Helpful functions
*********************************************************************/
inline void TERMINATE()
{
  LOG(ERROR) << "TERMINATE PROGRAM \n";
  std::exit(EXIT_FAILURE);
}

} // namespace Solver
} // namespace IncomFlow 
