// Copyright (c) 2018 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#include "ovk/core/Request.hpp"

#include "ovk/core/Array.hpp"
#include "ovk/core/ArrayView.hpp"
#include "ovk/core/Global.hpp"

#include <mpi.h>

#include <memory>
#include <utility>

namespace ovk {

void WaitAll(array_view<request> Requests) {

  int NumRequests = Requests.Count();

  array<request *> RequestPtrs({NumRequests});

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    RequestPtrs[iRequest] = Requests.Data(iRequest);
  }

  request::core_WaitAll(RequestPtrs);

}

void WaitAny(array_view<request> Requests, int &Index) {

  int NumRequests = Requests.Count();

  array<request *> RequestPtrs({NumRequests});

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    RequestPtrs[iRequest] = Requests.Data(iRequest);
  }

  request::core_WaitAny(RequestPtrs, Index);

}

void request::core_WaitAll(array_view<request *> Requests) {

  int NumRequests = Requests.Count();

  array<int> NumRemainingMPIRequests({NumRequests});
  int TotalMPIRequests = 0;

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    if (Requests(iRequest)) {
      request &Request = *Requests(iRequest);
      if (Request) {
        int NumMPIRequests = Request.MPIRequests().Count();
        NumRemainingMPIRequests(iRequest) = NumMPIRequests;
        TotalMPIRequests += NumMPIRequests;
      }
    }
  }

  array<MPI_Request> AllMPIRequests;
  array<int> MPIRequestToRequest;

  AllMPIRequests.Reserve(TotalMPIRequests);
  MPIRequestToRequest.Reserve(TotalMPIRequests);

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    if (Requests(iRequest)) {
      request &Request = *Requests(iRequest);
      if (Request) {
        array_view<MPI_Request> MPIRequests = Request.MPIRequests();
        for (int iMPIRequest = 0; iMPIRequest < MPIRequests.Count(); ++iMPIRequest) {
          AllMPIRequests.Append(MPIRequests(iMPIRequest));
          MPIRequestToRequest.Append(iRequest);
        }
      }
    }
  }

  while (true) {
    int iMPIRequest;
    MPI_Waitany(TotalMPIRequests, AllMPIRequests.Data(), &iMPIRequest, MPI_STATUSES_IGNORE);
    if (iMPIRequest == MPI_UNDEFINED) {
      break;
    }
    int iRequest = MPIRequestToRequest(iMPIRequest);
    --NumRemainingMPIRequests(iRequest);
    if (NumRemainingMPIRequests(iRequest) == 0) {
      request &Request = *Requests(iRequest);
      Request.MPIRequests().Fill(MPI_REQUEST_NULL);
      Request.Wait();
      Request = request();
    }
  }

}

void request::core_WaitAny(array_view<request *> Requests, int &Index) {

  int NumRequests = Requests.Count();

  array<int> NumRemainingMPIRequests({NumRequests});
  int TotalMPIRequests = 0;

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    if (Requests(iRequest)) {
      request &Request = *Requests(iRequest);
      if (Request) {
        int NumMPIRequests = Request.MPIRequests().Count();
        NumRemainingMPIRequests(iRequest) = NumMPIRequests;
        TotalMPIRequests += NumMPIRequests;
      }
    }
  }

  array<MPI_Request> AllMPIRequests;
  array<int> MPIRequestToRequest;

  AllMPIRequests.Reserve(TotalMPIRequests);
  MPIRequestToRequest.Reserve(TotalMPIRequests);

  for (int iRequest = 0; iRequest < NumRequests; ++iRequest) {
    if (Requests(iRequest)) {
      request &Request = *Requests(iRequest);
      if (Request) {
        array_view<MPI_Request> MPIRequests = Request.MPIRequests();
        for (int iMPIRequest = 0; iMPIRequest < MPIRequests.Count(); ++iMPIRequest) {
          AllMPIRequests.Append(MPIRequests(iMPIRequest));
          MPIRequestToRequest.Append(iRequest);
        }
      }
    }
  }

  while (true) {
    int iMPIRequest;
    MPI_Waitany(TotalMPIRequests, AllMPIRequests.Data(), &iMPIRequest, MPI_STATUSES_IGNORE);
    if (iMPIRequest == MPI_UNDEFINED) {
      Index = -1;
      break;
    }
    int iRequest = MPIRequestToRequest(iMPIRequest);
    --NumRemainingMPIRequests(iRequest);
    if (NumRemainingMPIRequests(iRequest) == 0) {
      request &Request = *Requests(iRequest);
      Request.MPIRequests().Fill(MPI_REQUEST_NULL);
      Request.Wait();
      Request = request();
      Index = iRequest;
      break;
    }
  }

}

}
