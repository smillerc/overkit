// Copyright (c) 2019 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#include "ovk/core/DisperseBase.hpp"

#include "ovk/core/Array.hpp"
#include "ovk/core/ArrayView.hpp"
#include "ovk/core/Context.hpp"
#include "ovk/core/Global.hpp"
#include "ovk/core/Indexer.hpp"
#include "ovk/core/Range.hpp"

namespace ovk {
namespace core {

template <array_layout Layout> disperse_base<Layout>::disperse_base(std::shared_ptr<context>
  &&Context, const array<int,2> &Points, int Count, const range &FieldValuesRange):
  Context_(std::move(Context)),
  Points_(Points),
  Count_(Count),
  FieldValuesRange_(FieldValuesRange),
  FieldValuesIndexer_(FieldValuesRange)
{}

template class disperse_base<array_layout::ROW_MAJOR>;
template class disperse_base<array_layout::COLUMN_MAJOR>;

template <typename T, array_layout Layout> disperse_base_for_type<T, Layout>::
  disperse_base_for_type(std::shared_ptr<context> &&Context, const array<int,2> &Points, int Count,
  const range &FieldValuesRange):
  parent_type(std::move(Context), Points, Count, FieldValuesRange)
{
  PackedValues_.Resize({Count_});
  FieldValues_.Resize({Count_});
}

template <typename T, array_layout Layout> void disperse_base_for_type<T, Layout>::SetBufferViews(
  const void *PackedValuesVoid, void *FieldValuesVoid) {

  long long NumPoints = Points_.Size(1);

  auto PackedValuesRaw = static_cast<const value_type * const *>(PackedValuesVoid);
  auto FieldValuesRaw = static_cast<value_type **>(FieldValuesVoid);

  OVK_DEBUG_ASSERT(PackedValuesRaw || Count_ == 0, "Invalid packed values pointer.");
  OVK_DEBUG_ASSERT(FieldValuesRaw || Count_ == 0, "Invalid field values pointer.");

  for (int iCount = 0; iCount < Count_; ++iCount) {
    OVK_DEBUG_ASSERT(PackedValuesRaw[iCount] || NumPoints == 0, "Invalid packed values pointer.");
    PackedValues_(iCount) = {PackedValuesRaw[iCount], {NumPoints}};
  }

  for (int iCount = 0; iCount < Count_; ++iCount) {
    OVK_DEBUG_ASSERT(FieldValuesRaw[iCount] || NumPoints == 0, "Invalid field values pointer.");
    FieldValues_(iCount) = {FieldValuesRaw[iCount], {FieldValuesRange_.Count()}};
  }

}

template class disperse_base_for_type<bool, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<bool, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<unsigned char, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<unsigned char, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<int, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<int, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<long, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<long, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<long long, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<long long, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<unsigned int, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<unsigned int, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<unsigned long, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<unsigned long, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<unsigned long long, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<unsigned long long, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<float, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<float, array_layout::COLUMN_MAJOR>;
template class disperse_base_for_type<double, array_layout::ROW_MAJOR>;
template class disperse_base_for_type<double, array_layout::COLUMN_MAJOR>;

}}
