// Copyright (c) 2019 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#ifndef OVK_CORE_DISTRIBUTED_FIELD_OPS_HPP_INCLUDED
#define OVK_CORE_DISTRIBUTED_FIELD_OPS_HPP_INCLUDED

#include <ovk/core/Comm.hpp>
#include <ovk/core/DistributedField.hpp>
#include <ovk/core/Global.hpp>
#include <ovk/core/Partition.hpp>
#include <ovk/core/Range.hpp>

#include <mpi.h>

namespace ovk {
namespace core {

enum class edge_type {
  INNER,
  OUTER
};

enum class mask_bc {
  FALSE,
  TRUE,
  MIRROR
};

long long CountDistributedMask(const distributed_field<bool> &Mask);

void DetectEdge(const distributed_field<bool> &Mask, edge_type EdgeType, mask_bc BoundaryCondition,
  bool IncludeExteriorPoint, distributed_field<bool> &EdgeMask, const partition_pool
  *MaybePartitionPool=nullptr);

void DilateMask(distributed_field<bool> &Mask, int Amount, mask_bc BoundaryCondition);
void ErodeMask(distributed_field<bool> &Mask, int Amount, mask_bc BoundaryCondition);

}}

#endif
