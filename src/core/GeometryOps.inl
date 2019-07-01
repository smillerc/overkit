// Copyright (c) 2019 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

namespace ovk {
namespace core {

template <typename IndexType> IndexType CartesianGridCell1D(double Origin, double CellSize, double
  Coord) {

  return IndexType(std::floor((Coord - Origin)/CellSize));

}

template <typename IndexType> tuple<IndexType> CartesianGridCell2D(const tuple<double> &Origin,
  const tuple<double> &CellSize, const tuple<double> &Coords) {

  return {
    IndexType(std::floor((Coords(0) - Origin(0))/CellSize(0))),
    IndexType(std::floor((Coords(1) - Origin(1))/CellSize(1))),
    IndexType(0)
  };

}

template <typename IndexType> tuple<IndexType> CartesianGridCell3D(const tuple<double> &Origin,
  const tuple<double> &CellSize, const tuple<double> &Coords) {

  return {
    IndexType(std::floor((Coords(0) - Origin(0))/CellSize(0))),
    IndexType(std::floor((Coords(1) - Origin(1))/CellSize(1))),
    IndexType(std::floor((Coords(2) - Origin(2))/CellSize(2)))
  };

}

inline double ColumnDeterminant2D(const tuple<double> &AI, const tuple<double> &AJ) {

  return AI(0)*AJ(1) - AI(1)*AJ(0);

}

inline double ColumnDeterminant3D(const tuple<double> &AI, const tuple<double> &AJ, const
  tuple<double> &AK) {

  return
    AI(0) * (AJ(1)*AK(2) - AJ(2)*AK(1)) +
    AJ(0) * (AK(1)*AI(2) - AK(2)*AI(1)) +
    AK(0) * (AI(1)*AJ(2) - AI(2)*AJ(1));

}

inline tuple<double> ColumnSolve2D(const tuple<double> &AI, const tuple<double> &AJ, const
  tuple<double> &B) {

  tuple<double> X;

  double Det = ColumnDeterminant2D(AI, AJ);

  X(0) = ColumnDeterminant2D(B, AJ)/Det;
  X(1) = ColumnDeterminant2D(AI, B)/Det;
  X(2) = 0.;

  return X;

}

inline tuple<double> ColumnSolve3D(const tuple<double> &AI, const tuple<double> &AJ, const
  tuple<double> &AK, const tuple<double> &B) {

  tuple<double> X;

  double Det = ColumnDeterminant3D(AI, AJ, AK);

  X(0) = ColumnDeterminant3D(B, AJ, AK)/Det;
  X(1) = ColumnDeterminant3D(AI, B, AK)/Det;
  X(2) = ColumnDeterminant3D(AI, AJ, B)/Det;

  return X;

}

inline elem<double,2> LagrangeInterpLinear(double U) {

  return {1. - U, U};

}

inline elem<double,2> LagrangeInterpLinearDeriv(double U) {

  return {-1., 1.};

}

inline elem<double,4> LagrangeInterpCubic(double U) {

  return {
    -(       U * (U - 1.) * (U - 2.)) / 6.,
     ((U + 1.) * (U - 1.) * (U - 2.)) / 2.,
    -((U + 1.) *        U * (U - 2.)) / 2.,
     ((U + 1.) *        U * (U - 1.)) / 6.
  };

}

inline elem<double,4> LagrangeInterpCubicDeriv(double U) {

  constexpr double Sqrt3 = 1.732050807568877293527446341505872366942805253810380628055;
  constexpr double Sqrt7 = 2.645751311064590590501615753639260425710259183082450180368;

  constexpr double Roots[] = {
      1. + 1./Sqrt3,
      1. - 1./Sqrt3,
    (2. + Sqrt7)/3.,
    (2. - Sqrt7)/3.,
    (1. + Sqrt7)/3.,
    (1. - Sqrt7)/3.,
           1./Sqrt3,
          -1./Sqrt3
  };

  return {
         -((U - Roots[0]) * (U - Roots[1]))/2.,
     (3. * (U - Roots[2]) * (U - Roots[3]))/2.,
    -(3. * (U - Roots[4]) * (U - Roots[5]))/2.,
          ((U - Roots[6]) * (U - Roots[7]))/2.
  };

}

}}
