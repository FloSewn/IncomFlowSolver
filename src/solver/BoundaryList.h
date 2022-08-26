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

#include "PrimaryGrid.h"
#include "BoundaryDef.h"
#include "Boundary.h"

#include "definitions.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* This class collects all median dual grid boundaries
*********************************************************************/
class BoundaryList
{
public:

  using BoundaryVector = std::vector<Boundary>;
  using iterator       = typename BoundaryVector::iterator;
  using const_iterator = typename BoundaryVector::const_iterator;

  iterator begin() { return boundaries_.begin(); }
  iterator end() { return boundaries_.end(); }

  const_iterator begin() const { return boundaries_.begin(); }
  const_iterator end() const { return boundaries_.end(); }

  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  BoundaryList(const PrimaryGrid& pgrid, const BoundaryDef& bdef) 
  : bdry_def_ { bdef }
  {
    // Iterate over all defined boundary definitions
    // and create new boundaries 
    for ( const auto key_val : bdef ) 
    {
      int marker    = key_val.first;
      BdryType type = key_val.second;
      boundaries_.push_back( { pgrid, marker, type } );
    }
  }

  /*------------------------------------------------------------------
  | Return the total number of boundaries
  ------------------------------------------------------------------*/
  int size() { return boundaries_.size(); }
  int size() const { return boundaries_.size(); }

private:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  BoundaryDef     bdry_def_;

  BoundaryVector  boundaries_;

}; // BoundaryList

} // namespace Solver
} // namespace IncomFlow
