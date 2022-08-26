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
  : n_elements_     { pg.n_vertices()      }
  , n_intr_faces_   { pg.n_intr_edges()    }
  , coords_         ( pg.n_vertices(),   2 )
  , face_normals_   ( pg.n_intr_edges(), 2 )
  , face_neighbors_ ( pg.n_intr_edges(), 2 )
  , volumes_        ( pg.n_vertices()      )
  , boundaries_     { pg, bd }
  {}

private:
  /*------------------------------------------------------------------
  | Attributes
  ------------------------------------------------------------------*/
  int          n_elements_;
  int          n_intr_faces_;

  DMat         coords_;
  DMat         face_normals_;

  IMat         face_neighbors_;

  DVec         volumes_;

  BoundaryList boundaries_;


}; // DualGrid

} // namespace Solver
} // namespace IncomFlow
