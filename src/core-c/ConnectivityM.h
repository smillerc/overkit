// Copyright (c) 2020 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#ifndef OVK_CORE_C_CONNECTIVITY_M_H_INCLUDED
#define OVK_CORE_C_CONNECTIVITY_M_H_INCLUDED

#include <ovk/core-c/Context.h>
#include <ovk/core-c/Global.h>
#include <ovk/core-c/Grid.h>

#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ovk_connectivity_m;
typedef struct ovk_connectivity_m ovk_connectivity_m;

void ovkGetConnectivityMContextC(const ovk_connectivity_m *ConnectivityM, const ovk_context
  **Context);
void ovkGetConnectivityMContext(ovk_connectivity_m *ConnectivityM, ovk_context **Context);
void ovkGetConnectivityMSharedContext(ovk_connectivity_m *ConnectivityM, ovk_shared_context
  **Context);

void ovkGetConnectivityMGrid(const ovk_connectivity_m *ConnectivityM, const ovk_grid **Grid);

void ovkGetConnectivityMDestinationGridInfo(const ovk_connectivity_m *ConnectivityM, ovk_grid_info
  **DestinationGridInfo);

void ovkGetConnectivityMDimension(const ovk_connectivity_m *ConnectivityM, int *NumDims);
void ovkGetConnectivityMComm(const ovk_connectivity_m *ConnectivityM, MPI_Comm *Comm);
void ovkGetConnectivityMCommSize(const ovk_connectivity_m *ConnectivityM, int *CommSize);
void ovkGetConnectivityMCommRank(const ovk_connectivity_m *ConnectivityM, int *CommRank);

long long ovkGetConnectivityMSize(const ovk_connectivity_m *ConnectivityM);
int ovkGetConnectivityMMaxStencilSize(const ovk_connectivity_m *ConnectivityM);

void ovkResizeConnectivityM(ovk_connectivity_m *ConnectivityM, long long NumDonors, int
  MaxStencilSize);

void ovkGetConnectivityMExtents(const ovk_connectivity_m *ConnectivityM, int Dimension, const int
  **Begins, const int **Ends);
bool ovkEditingConnectivityMExtents(const ovk_connectivity_m *ConnectivityM);
void ovkEditConnectivityMExtents(ovk_connectivity_m *ConnectivityM, int Dimension, int **Begins,
  int **Ends);
void ovkRestoreConnectivityMExtents(ovk_connectivity_m *ConnectivityM, int Dimension, int **Begins,
  int **Ends);

void ovkGetConnectivityMCoords(const ovk_connectivity_m *ConnectivityM, int Dimension, const
  double **Coords);
bool ovkEditingConnectivityMCoords(const ovk_connectivity_m *ConnectivityM);
void ovkEditConnectivityMCoords(ovk_connectivity_m *ConnectivityM, int Dimension, double **Coords);
void ovkRestoreConnectivityMCoords(ovk_connectivity_m *ConnectivityM, int Dimension, double
  **Coords);

void ovkGetConnectivityMInterpCoefs(const ovk_connectivity_m *ConnectivityM, int Dimension, int
  Point, const double **InterpCoefs);
bool ovkEditingConnectivityMInterpCoefs(const ovk_connectivity_m *ConnectivityM);
void ovkEditConnectivityMInterpCoefs(ovk_connectivity_m *ConnectivityM, int Dimension, int Point,
  double **InterpCoefs);
void ovkRestoreConnectivityMInterpCoefs(ovk_connectivity_m *ConnectivityM, int Dimension, int Point,
  double **InterpCoefs);

void ovkGetConnectivityMDestinations(const ovk_connectivity_m *ConnectivityM, int Dimension, const
  int **Destinations);
bool ovkEditingConnectivityMDestinations(const ovk_connectivity_m *ConnectivityM);
void ovkEditConnectivityMDestinations(ovk_connectivity_m *ConnectivityM, int Dimension, int
  **Destinations);
void ovkRestoreConnectivityMDestinations(ovk_connectivity_m *ConnectivityM, int Dimension, int
  **Destinations);

void ovkGetConnectivityMDestinationRanks(const ovk_connectivity_m *ConnectivityM, const int
  **DestinationRanks);
bool ovkEditingConnectivityMDestinationRanks(const ovk_connectivity_m *ConnectivityM);
void ovkEditConnectivityMDestinationRanks(ovk_connectivity_m *ConnectivityM, int
  **DestinationRanks);
void ovkRestoreConnectivityMDestinationRanks(ovk_connectivity_m *ConnectivityM, int
  **DestinationRanks);

#ifdef __cplusplus
}
#endif

#endif
