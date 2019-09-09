// Copyright (c) 2019 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

namespace ovk {

inline cart::cart(int NumDims):
  NumDims_(NumDims),
  Range_(MakeEmptyRange(NumDims)),
  Periodic_(false,false,false),
  PeriodicStorage_(periodic_storage::UNIQUE)
{}

inline cart::cart(int NumDims, const range &Range, const tuple<bool> &Periodic, periodic_storage
  PeriodicStorage):
  NumDims_(NumDims),
  Range_(Range),
  Periodic_(Periodic),
  PeriodicStorage_(PeriodicStorage)
{}

inline tuple<int> cart::GetPeriod(const tuple<int> &Tuple) const {

  tuple<int> Period;

  switch (PeriodicStorage_) {
  case periodic_storage::UNIQUE:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      if (Periodic_(iDim)) {
        int PeriodSize = Range_.Size(iDim);
        int Offset = Tuple(iDim) - Range_.Begin(iDim);
        Period(iDim) = Offset/PeriodSize - int(Offset % PeriodSize < 0);
      } else {
        Period(iDim) = 0;
      }
    }
    break;
  case periodic_storage::DUPLICATED:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      if (Periodic_(iDim)) {
        int PeriodSize = Range_.Size(iDim)-1;
        int Offset = Tuple(iDim) - Range_.Begin(iDim);
        Period(iDim) = Offset/PeriodSize - int(Offset % PeriodSize < 0);
      } else {
        Period(iDim) = 0;
      }
    }
    break;
  default:
    OVK_DEBUG_ASSERT(false, "Unhandled enum value.");
    Period = {0,0,0};
    break;
  }

  return Period;

}

inline tuple<int> cart::PeriodicAdjust(const tuple<int> &Tuple) const {

  tuple<int> AdjustedTuple = Tuple;

  switch (PeriodicStorage_) {
  case periodic_storage::UNIQUE:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      if (Periodic_(iDim)) {
        int PeriodSize = Range_.Size(iDim);
        int Mod = (Tuple(iDim) - Range_.Begin(iDim)) % PeriodSize;
        AdjustedTuple(iDim) = Range_.Begin(iDim) + Mod + PeriodSize * (Mod < 0);
      }
    }
    break;
  case periodic_storage::DUPLICATED:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      if (Periodic_(iDim)) {
        int PeriodSize = Range_.Size(iDim)-1;
        int Mod = (Tuple(iDim) - Range_.Begin(iDim)) % PeriodSize;
        AdjustedTuple(iDim) = Range_.Begin(iDim) + Mod + PeriodSize * (Mod < 0);
      }
    }
    break;
  default:
    OVK_DEBUG_ASSERT(false, "Unhandled enum value.");
    break;
  }

  return AdjustedTuple;

}

inline optional<tuple<int>> cart::MapToRange(const range &Range, const tuple<int> &Tuple) const {

  tuple<int> BeginPeriod = GetPeriod(Range.Begin());
  tuple<int> TuplePeriod = GetPeriod(Tuple);

  tuple<int> MappedTuple;

  switch (PeriodicStorage_) {
  case periodic_storage::UNIQUE:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      int PeriodSize = Range_.Size(iDim);
      MappedTuple(iDim) = Tuple(iDim) + PeriodSize * (BeginPeriod(iDim) - TuplePeriod(iDim));
      if (MappedTuple(iDim) < Range.Begin(iDim)) MappedTuple(iDim) += PeriodSize;
    }
    break;
  case periodic_storage::DUPLICATED:
    for (int iDim = 0; iDim < MAX_DIMS; ++iDim) {
      int PeriodSize = Range_.Size(iDim)-1;
      MappedTuple(iDim) = Tuple(iDim) + PeriodSize * (BeginPeriod(iDim) - TuplePeriod(iDim));
      if (MappedTuple(iDim) < Range.Begin(iDim)) MappedTuple(iDim) += PeriodSize;
    }
    break;
  default:
    OVK_DEBUG_ASSERT(false, "Unhandled enum value.");
    MappedTuple = {0,0,0};
    break;
  }

  if (Range.Contains(MappedTuple)) {
    return MappedTuple;
  } else {
    return {};
  }

}

inline bool operator==(const cart &Left, const cart &Right) {

  return
    Left.NumDims_ == Right.NumDims_ &&
    Left.Range_ == Right.Range_ &&
    Left.Periodic_ == Right.Periodic_ &&
    Left.PeriodicStorage_ == Right.PeriodicStorage_;

}

inline bool operator!=(const cart &Left, const cart &Right) {

  return !(Left == Right);

}

inline cart MakeEmptyCart(int NumDims) {

  return {NumDims, MakeEmptyRange(NumDims), MakeUniformTuple<int>(NumDims, false),
    periodic_storage::UNIQUE};

}

}
