// Copyright (c) 2019 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#ifndef OVK_CORE_ASSEMBLER_HPP_INCLUDED
#define OVK_CORE_ASSEMBLER_HPP_INCLUDED

#include <ovk/core/Array.hpp>
#include <ovk/core/ArrayView.hpp>
#include <ovk/core/Assembler.h>
#include <ovk/core/Comm.hpp>
#include <ovk/core/ConnectivityComponent.hpp>
#include <ovk/core/Context.hpp>
#include <ovk/core/DistributedField.hpp>
#include <ovk/core/Domain.hpp>
#include <ovk/core/ElemMap.hpp>
#include <ovk/core/ElemSet.hpp>
#include <ovk/core/Event.hpp>
#include <ovk/core/FloatingRef.hpp>
#include <ovk/core/GeometryComponent.hpp>
#include <ovk/core/Global.hpp>
#include <ovk/core/Grid.hpp>
#include <ovk/core/Map.hpp>
#include <ovk/core/Optional.hpp>
#include <ovk/core/OverlapComponent.hpp>
#include <ovk/core/Set.hpp>
#include <ovk/core/StateComponent.hpp>
#include <ovk/core/StringWrapper.hpp>

#include <mpi.h>

#include <memory>
#include <string>

namespace ovk {

enum class occludes {
  NONE = OVK_OCCLUDES_NONE,
  ALL = OVK_OCCLUDES_ALL,
  COARSE = OVK_OCCLUDES_COARSE
};

inline bool ValidOccludes(occludes Occludes) {
  return ovkValidOccludes(ovk_occludes(Occludes));
}

enum class connection_type {
  NONE = OVK_CONNECTION_NONE,
  NEAREST = OVK_CONNECTION_NEAREST,
  LINEAR = OVK_CONNECTION_LINEAR,
  CUBIC = OVK_CONNECTION_CUBIC
};

inline bool ValidConnectionType(connection_type ConnectionType) {
  return ovkValidConnectionType(ovk_connection_type(ConnectionType));
}

class assembler {

public:

  class params {
  public:
    params() = default;
    const std::string &Name() const { return Name_; }
    params &SetName(std::string Name);
  private:
    core::string_wrapper Name_ = "Assembler";
    friend class assembler;
  };

  class bindings {
  public:
    bindings() = default;
    int GeometryComponentID() const { return GeometryComponentID_; }
    bindings &SetGeometryComponentID(int GeometryComponentID);
    int StateComponentID() const { return StateComponentID_; }
    bindings &SetStateComponentID(int StateComponentID);
    int OverlapComponentID() const { return OverlapComponentID_; }
    bindings &SetOverlapComponentID(int OverlapComponentID);
    int ConnectivityComponentID() const { return ConnectivityComponentID_; }
    bindings &SetConnectivityComponentID(int ConnectivityComponentID);
  private:
    int GeometryComponentID_ = -1;
    int StateComponentID_ = -1;
    int OverlapComponentID_ = -1;
    int ConnectivityComponentID_ = -1;
    friend class assembler;
  };

  class options {
  public:
    bool Overlappable(const elem<int,2> &GridIDPair) const;
    options &SetOverlappable(const elem<int,2> &GridIDPair, bool Overlappable);
    options &ResetOverlappable(const elem<int,2> &GridIDPair);
    double OverlapTolerance(const elem<int,2> &GridIDPair) const;
    options &SetOverlapTolerance(const elem<int,2> &GridIDPair, double OverlapTolerance);
    options &ResetOverlapTolerance(const elem<int,2> &GridIDPair);
    double OverlapAccelDepthAdjust(int MGridID) const;
    options &SetOverlapAccelDepthAdjust(int MGridID, double OverlapAccelDepthAdjust);
    options &ResetOverlapAccelDepthAdjust(int MGridID);
    double OverlapAccelResolutionAdjust(int MGridID) const;
    options &SetOverlapAccelResolutionAdjust(int MGridID, double OverlapAccelResolutionAdjust);
    options &ResetOverlapAccelResolutionAdjust(int MGridID);
    bool InferBoundaries(int GridID) const;
    options &SetInferBoundaries(int GridID, bool InferBoundaries);
    options &ResetInferBoundaries(int GridID);
    bool CutBoundaryHoles(const elem<int,2> &GridIDPair) const;
    options &SetCutBoundaryHoles(const elem<int,2> &GridIDPair, bool CutBoundaryHoles);
    options &ResetCutBoundaryHoles(const elem<int,2> &GridIDPair);
    occludes Occludes(const elem<int,2> &GridIDPair) const;
    options &SetOccludes(const elem<int,2> &GridIDPair, occludes Occludes);
    options &ResetOccludes(const elem<int,2> &GridIDPair);
    int EdgePadding(const elem<int,2> &GridIDPair) const;
    options &SetEdgePadding(const elem<int,2> &GridIDPair, int EdgePadding);
    options &ResetEdgePadding(const elem<int,2> &GridIDPair);
    int EdgeSmoothing(int NGridID) const;
    options &SetEdgeSmoothing(int NGridID, int EdgeSmoothing);
    options &ResetEdgeSmoothing(int NGridID);
    connection_type ConnectionType(const elem<int,2> &GridIDPair) const;
    options &SetConnectionType(const elem<int,2> &GridIDPair, connection_type ConnectionType);
    options &ResetConnectionType(const elem<int,2> &GridIDPair);
    int FringeSize(int NGridID) const;
    options &SetFringeSize(int NGridID, int FringeSize);
    options &ResetFringeSize(int NGridID);
    bool MinimizeOverlap(const elem<int,2> &GridIDPair) const;
    options &SetMinimizeOverlap(const elem<int,2> &GridIDPair, bool MinimizeOverlap);
    options &ResetMinimizeOverlap(const elem<int,2> &GridIDPair);
  private:
    set<int> GridIDs_;
    elem_map<int,2,bool> Overlappable_;
    elem_map<int,2,double> OverlapTolerance_;
    map<int,double> OverlapAccelDepthAdjust_;
    map<int,double> OverlapAccelResolutionAdjust_;
    map<int,bool> InferBoundaries_;
    elem_map<int,2,bool> CutBoundaryHoles_;
    elem_map<int,2,occludes> Occludes_;
    elem_map<int,2,int> EdgePadding_;
    map<int,int> EdgeSmoothing_;
    elem_map<int,2,connection_type> ConnectionType_;
    map<int,int> FringeSize_;
    elem_map<int,2,bool> MinimizeOverlap_;
    options() = default;
    void AddGrids(const set<int> &GridIDs);
    void RemoveGrids(const set<int> &GridIDs);
    template <typename T> T GetOption_(const map<int,T> &Option, int GridID, T DefaultValue) const;
    template <typename T> T GetOption_(const elem_map<int,2,T> &Option, const elem<int,2>
      &GridIDPair, T DefaultValue) const;
    template <typename T> void SetOption_(map<int,T> &Option, int GridID, T Value, T DefaultValue);
    template <typename T> void SetOption_(elem_map<int,2,T> &Option, const elem<int,2> &GridIDPair,
      T Value, T DefaulValue);
    void PrintOptions_();
    friend class assembler;
  };

  assembler(const assembler &Other) = delete;
  assembler(assembler &&Other) noexcept = default;

  assembler &operator=(const assembler &Other) = delete;
  assembler &operator=(assembler &&Other) noexcept = default;

  ~assembler() noexcept;

  const context &Context() const;
  context &Context();
  const std::shared_ptr<context> &SharedContext() const;

  const std::string &Name() const { return *Name_; }

  bool Bound() const;
  void Bind(domain &Domain, bindings Bindings);
  void Unbind();

  const domain &Domain() const;
  domain &Domain();

  const options &Options() const { return Options_; }
  bool EditingOptions() const;
  edit_handle<options> EditOptions();
  void RestoreOptions();

  void Assemble();

  static assembler internal_Create(std::shared_ptr<context> &&Context, params &&Params);

private:

  struct update_manifest {
    set<int> AddGridsToOptions;
    set<int> RemoveGridsFromOptions;
    set<int> RemoveAssemblyManifestEntries;
  };

  struct assembly_manifest {
    elem_set<int,2> DetectOverlap;
    set<int> InferBoundaries;
    elem_set<int,2> CutBoundaryHoles;
    elem_set<int,2> ComputeOcclusion;
    elem_set<int,2> ApplyPadding;
    set<int> ApplySmoothing;
    elem_set<int,2> MinimizeOverlap;
    elem_set<int,2> GenerateConnectivity;
  };

  floating_ref_generator FloatingRefGenerator_;

  std::shared_ptr<context> Context_;

  core::string_wrapper Name_;

  floating_ref<domain> Domain_;
  event_listener_handle GridEventListener_;
  event_listener_handle ComponentEventListener_;

  int GeometryComponentID_ = -1;
  event_listener_handle GeometryEventListener_;

  int StateComponentID_ = -1;
  event_listener_handle StateEventListener_;

  int OverlapComponentID_ = -1;
  event_listener_handle OverlapEventListener_;

  int ConnectivityComponentID_ = -1;
  event_listener_handle ConnectivityEventListener_;

  options Options_;
  options CachedOptions_;
  editor OptionsEditor_;

  update_manifest UpdateManifest_;

  assembly_manifest AssemblyManifest_;

  struct local_grid_aux_data {
    distributed_field<bool> ActiveMask;
    distributed_field<bool> CellActiveMask;
    distributed_field<bool> DomainBoundaryMask;
    distributed_field<bool> InternalBoundaryMask;
  };

  struct assembly_data {
    map<int,local_grid_aux_data> LocalGridAuxData;
    assembly_data(int NumDims, comm_view Comm);
  };

  optional<assembly_data> AssemblyData_;

  assembler(std::shared_ptr<context> &&Context, params &&Params);

  void OnGridEvent_(int GridID, grid_event_flags Flags, bool LastInSequence);
  void OnComponentEvent_(int ComponentID, component_event_flags Flags);
  void OnGeometryEvent_(int GridID, geometry_event_flags Flags, bool LastInSequence);
  void OnStateEvent_(int GridID, state_event_flags Flags, bool LastInSequence);
  void OnOverlapEvent_(const elem<int,2> &OverlapID, overlap_event_flags Flags, bool
    LastInSequence);
  void OnConnectivityEvent_(const elem<int,2> &ConnectivityID, connectivity_event_flags Flags, bool
    LastInSequence);

  void OnOptionsStartEdit_();
  void OnOptionsEndEdit_();

  void Update_();

  void AddGridsToOptions_();
  void RemoveGridsFromOptions_();
  void RemoveAssemblyManifestEntries_();

  void InitializeAssembly_();

};

assembler CreateAssembler(std::shared_ptr<context> Context, assembler::params Params={});

}

#endif
