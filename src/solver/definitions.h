/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#pragma once

#include <vector>

#include "Matrix.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* Typedefs
*********************************************************************/
using DVec = std::vector<double>;
using IVec = std::vector<int>;

using DMat = Matrix<double>;
using IMat = Matrix<int>;


/*********************************************************************
* Global variables
*********************************************************************/
constexpr size_t N_MAX_VARS { 100 }; 

/*********************************************************************
* All available boundary types
*********************************************************************/
enum class BdryType
{
  INVALID,
  PERIODIC,
  SYMMETRY,
  INLET,
  OUTLET,
  WALL,
};

} // namespace Solver
} // namespace IncomFlow
