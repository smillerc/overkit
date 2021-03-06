// Copyright (c) 2020 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#include "ovk/core/ConnectivityN.hpp"

#include "ovk/core/Array.hpp"
#include "ovk/core/Comm.hpp"
#include "ovk/core/Context.hpp"
#include "ovk/core/Debug.hpp"
#include "ovk/core/Editor.hpp"
#include "ovk/core/Event.hpp"
#include "ovk/core/FloatingRef.hpp"
#include "ovk/core/Global.hpp"
#include "ovk/core/Grid.hpp"
#include "ovk/core/Logger.hpp"

#include <mpi.h>

#include <memory>
#include <utility>

namespace ovk {

namespace connectivity_n_internal {

connectivity_n_base::connectivity_n_base(std::shared_ptr<context> &&Context, const grid &Grid,
  grid_info &&SourceGridInfo):
  Context_(std::move(Context)),
  Grid_(&Grid),
  SourceGridInfo_(std::move(SourceGridInfo)),
  Comm_(Grid_->Comm())
{
  MPI_Barrier(Comm_);
}

connectivity_n_base::~connectivity_n_base() noexcept {

  if (Context_) {
    MPI_Barrier(Comm_);
    core::logger &Logger = Context_->core_Logger();
    Logger.LogStatus(Comm_.Rank() == 0, "Destroyed N side of connectivity (%s,%s).",
      SourceGridInfo_.Name(), Grid_->Name());
  }

}

}

connectivity_n::connectivity_n(std::shared_ptr<context> &&Context, const grid &Grid, grid_info
  &&SourceGridInfo):
  connectivity_n_base(std::move(Context), Grid, std::move(SourceGridInfo)),
  NumDims_(Grid_->Dimension()),
  NumReceivers_(0),
  Points_({{MAX_DIMS,0}}),
  Sources_({{MAX_DIMS,0}}),
  SourceRanks_({0})
{

  MPI_Barrier(Comm_);

  core::logger &Logger = Context_->core_Logger();
  Logger.LogStatus(Comm_.Rank() == 0, "Created N side of connectivity (%s,%s).",
    SourceGridInfo_.Name(), Grid_->Name());

}

connectivity_n::~connectivity_n() noexcept {

  if (Context_) {
    // Barrier before cleaning up
    MPI_Barrier(Comm_);
  }

}

connectivity_n connectivity_n::internal_Create(std::shared_ptr<context> &&Context, const grid &Grid,
  grid_info &&SourceGridInfo) {

  return {std::move(Context), Grid, std::move(SourceGridInfo)};

}

namespace core {

connectivity_n CreateConnectivityN(std::shared_ptr<context> Context, const grid &Grid, grid_info
  SourceGridInfo) {

  OVK_DEBUG_ASSERT(Context, "Invalid context.");

  return connectivity_n::internal_Create(std::move(Context), Grid, std::move(SourceGridInfo));

}

}

void connectivity_n::Resize(long long NumReceivers) {

  OVK_DEBUG_ASSERT(NumReceivers >= 0, "Invalid num receivers value.");

  MPI_Barrier(Comm_);

  OVK_DEBUG_ASSERT(!PointsEditor_.Active(), "Cannot resize while editing points.");
  OVK_DEBUG_ASSERT(!SourcesEditor_.Active(), "Cannot resize while editing sources.");
  OVK_DEBUG_ASSERT(!SourceRanksEditor_.Active(), "Cannot resize while editing source ranks.");

  NumReceivers_ = NumReceivers;

  Points_.Resize({{MAX_DIMS,NumReceivers}});
  Sources_.Resize({{MAX_DIMS,NumReceivers}});
  SourceRanks_.Resize({NumReceivers});

  for (long long iPoint = 0; iPoint < NumReceivers_; ++iPoint) {
    for (int iDim = 0; iDim < NumDims_; ++iDim) {
      Points_(iDim,iPoint) = Grid_->GlobalRange().Begin(iDim)-1;
    }
    for (int iDim = NumDims_; iDim < MAX_DIMS; ++iDim) {
      Points_(iDim,iPoint) = 0;
    }
    for (int iDim = 0; iDim < NumDims_; ++iDim) {
      Sources_(iDim,iPoint) = SourceGridInfo_.CellGlobalRange().Begin(iDim)-1;
    }
    for (int iDim = NumDims_; iDim < MAX_DIMS; ++iDim) {
      Sources_(iDim,iPoint) = 0;
    }
    SourceRanks_(iPoint) = -1;
  }

  MPI_Barrier(Comm_);

  ResizeEvent_.Trigger();
  PointsEvent_.Trigger();
  SourcesEvent_.Trigger();
  SourceRanksEvent_.Trigger();

  MPI_Barrier(Comm_);

}

bool connectivity_n::EditingPoints() const {

  return PointsEditor_.Active();

}

edit_handle<array<int,2>> connectivity_n::EditPoints() {

  if (!PointsEditor_.Active()) {
    MPI_Barrier(Comm_);
    floating_ref<connectivity_n> FloatingRef = FloatingRefGenerator_.Generate(*this);
    auto DeactivateFunc = [FloatingRef] {
      connectivity_n &ConnectivityN = *FloatingRef;
      MPI_Barrier(ConnectivityN.Comm_);
      ConnectivityN.PointsEvent_.Trigger();
      MPI_Barrier(ConnectivityN.Comm_);
    };
    PointsEditor_.Activate(std::move(DeactivateFunc));
  }

  return PointsEditor_.Edit(Points_);

}

void connectivity_n::RestorePoints() {

  OVK_DEBUG_ASSERT(PointsEditor_.Active(), "Unable to restore points; not currently being edited.");

  PointsEditor_.Restore();

}

bool connectivity_n::EditingSources() const {

  return SourcesEditor_.Active();

}

edit_handle<array<int,2>> connectivity_n::EditSources() {

  if (!SourcesEditor_.Active()) {
    MPI_Barrier(Comm_);
    floating_ref<connectivity_n> FloatingRef = FloatingRefGenerator_.Generate(*this);
    auto DeactivateFunc = [FloatingRef] {
      connectivity_n &ConnectivityN = *FloatingRef;
      MPI_Barrier(ConnectivityN.Comm_);
      ConnectivityN.SourcesEvent_.Trigger();
      MPI_Barrier(ConnectivityN.Comm_);
    };
    SourcesEditor_.Activate(std::move(DeactivateFunc));
  }

  return SourcesEditor_.Edit(Sources_);

}

void connectivity_n::RestoreSources() {

  OVK_DEBUG_ASSERT(SourcesEditor_.Active(), "Unable to restore sources; not currently being "
    "edited.");

  SourcesEditor_.Restore();

}

bool connectivity_n::EditingSourceRanks() const {

  return SourceRanksEditor_.Active();

}

edit_handle<array<int>> connectivity_n::EditSourceRanks() {

  if (!SourceRanksEditor_.Active()) {
    MPI_Barrier(Comm_);
    floating_ref<connectivity_n> FloatingRef = FloatingRefGenerator_.Generate(*this);
    auto DeactivateFunc = [FloatingRef] {
      connectivity_n &ConnectivityN = *FloatingRef;
      MPI_Barrier(ConnectivityN.Comm_);
      ConnectivityN.SourceRanksEvent_.Trigger();
      MPI_Barrier(ConnectivityN.Comm_);
    };
    SourceRanksEditor_.Activate(std::move(DeactivateFunc));
  }

  return SourceRanksEditor_.Edit(SourceRanks_);

}

void connectivity_n::RestoreSourceRanks() {

  OVK_DEBUG_ASSERT(SourceRanksEditor_.Active(), "Unable to restore source ranks; not currently "
    "being edited.");

  SourceRanksEditor_.Restore();

}

}
