/*
* This file is part of the IncomFlow library.  
* This code was written by Florian Setzwein in 2022, 
* and is covered under the MIT License
* Refer to the accompanying documentation for details
* on usage and license.
*/

#pragma once

#include <map>

#include "PrimaryGrid.h"
#include "definitions.h"

namespace IncomFlow {
namespace Solver {

using namespace CppUtils;

/*********************************************************************
* This class is used to map specific boundary markers to their
* associated boundary types
*********************************************************************/
class BoundaryDef
{
public:

  using MarkerTypeMap  = std::map<int, BdryType>;
  using iterator       = typename MarkerTypeMap::iterator;
  using const_iterator = typename MarkerTypeMap::const_iterator;

  iterator begin() { return marker_to_types_.begin(); }
  iterator end() { return marker_to_types_.end(); }
  
  const_iterator begin() const { return marker_to_types_.begin(); }
  const_iterator end() const { return marker_to_types_.end(); }

  /*------------------------------------------------------------------
  | Constructor
  ------------------------------------------------------------------*/
  BoundaryDef() {}

  /*------------------------------------------------------------------
  | Copy
  ------------------------------------------------------------------*/
  BoundaryDef(const BoundaryDef& bd) 
  : marker_to_types_ { bd.marker_to_types_ }
  {}

  /*------------------------------------------------------------------
  | Move
  ------------------------------------------------------------------*/
  BoundaryDef(BoundaryDef&& bd) 
  : marker_to_types_ { std::move(bd.marker_to_types_) }
  {}

  /*------------------------------------------------------------------
  | Get the associated boundary type of a marker.
  | Returns BdryType::INVALID, if marker was not found 
  | in the boundary definition
  ------------------------------------------------------------------*/
  BdryType get_boundary_type(int marker)
  {
    
    if ( marker_to_types_.find(marker) == marker_to_types_.end() )
      return BdryType::INVALID;
    return marker_to_types_.at(marker);
  }

  /*------------------------------------------------------------------
  | Add a marker and its associated boundary type
  | It is not allowed to add negative markers (will be ignored)
  ------------------------------------------------------------------*/
  void add_marker(int marker, BdryType type)
  { 
    if ( marker < 0 )
      return;

    marker_to_types_[marker] = type;
  }

  /*------------------------------------------------------------------
  | Remove a marker from the boundary definiton
  ------------------------------------------------------------------*/
  void remove_marker(int marker)
  { marker_to_types_.erase( marker ); }

  /*------------------------------------------------------------------
  | Get the total number of defined markers in the boundary 
  | definition
  ------------------------------------------------------------------*/
  int size() { return marker_to_types_.size(); }
  int size() const { return marker_to_types_.size(); }


private:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  MarkerTypeMap marker_to_types_;

}; // BoundaryDef

} // namespace Solver
} // namespace IncomFlow
