// Copyright (c) 2020 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#ifndef OVK_CORE_COLLECT_MAX_HPP_INCLUDED
#define OVK_CORE_COLLECT_MAX_HPP_INCLUDED

#include <ovk/core/Array.hpp>
#include <ovk/core/ArrayView.hpp>
#include <ovk/core/Cart.hpp>
#include <ovk/core/CollectBase.hpp>
#include <ovk/core/CollectMap.hpp>
#include <ovk/core/Context.hpp>
#include <ovk/core/Global.hpp>
#include <ovk/core/Profiler.hpp>
#include <ovk/core/Range.hpp>
#include <ovk/core/ScalarOps.hpp>

#include <mpi.h>

#include <limits>
#include <memory>
#include <utility>

namespace ovk {
namespace core {
namespace collect_internal {

template <typename T, array_layout Layout> class collect_max : public collect_base_for_type<T,
  Layout> {

protected:

  using parent_type = collect_base_for_type<T, Layout>;

  using parent_type::Context_;
  using parent_type::CollectMap_;
  using parent_type::Count_;
  using parent_type::FieldValues_;
  using parent_type::PackedValues_;
  using parent_type::REDUCE_TIME;

public:

  using typename parent_type::value_type;

  collect_max(std::shared_ptr<context> &&Context, comm_view Comm, const cart &Cart, const range
    &LocalRange, const collect_map &CollectMap, int Count, const range &FieldValuesRange):
    parent_type(std::move(Context), Comm, Cart, LocalRange, CollectMap, Count, FieldValuesRange)
  {

    parent_type::AllocateRemoteValues_(RemoteValues_);

    VertexValues_.Resize({{Count_,CollectMap_->MaxVertices()}});

  }

  collect_max(const collect_max &Other) = delete;
  collect_max(collect_max &&Other) noexcept = default;

  collect_max &operator=(const collect_max &Other) = delete;
  collect_max &operator=(collect_max &&Other) noexcept = default;

  void Collect(const void *FieldValuesVoid, void *PackedValuesVoid) {

    profiler &Profiler = Context_->core_Profiler();

    parent_type::SetBufferViews_(FieldValuesVoid, PackedValuesVoid);
    parent_type::RetrieveRemoteValues_(FieldValues_, RemoteValues_);

    Profiler.Start(REDUCE_TIME);

    for (long long iCell = 0; iCell < CollectMap_->Count(); ++iCell) {

      range CellRange = parent_type::GetCellRange_(iCell);
      range_indexer<int,Layout> CellIndexer(CellRange);
      int NumVertices = CellRange.Count<int>();

      parent_type::AssembleVertexValues_(FieldValues_, RemoteValues_, iCell, CellRange, CellIndexer,
        VertexValues_);

      for (int iCount = 0; iCount < Count_; ++iCount) {
        PackedValues_(iCount)(iCell) = std::numeric_limits<value_type>::min();
        for (int iVertex = 0; iVertex < NumVertices; ++iVertex) {
          PackedValues_(iCount)(iCell) = Max(PackedValues_(iCount)(iCell), VertexValues_(iCount,
            iVertex));
        }
      }

    }

    Profiler.Stop(REDUCE_TIME);

  }

private:

  array<array<value_type,2>> RemoteValues_;
  array<value_type,2> VertexValues_;

};

}}}

#endif
