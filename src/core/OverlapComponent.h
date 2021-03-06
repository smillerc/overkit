// Copyright (c) 2020 Matthew J. Smith and Overkit contributors
// License: MIT (http://opensource.org/licenses/MIT)

#ifndef OVK_CORE_OVERLAP_COMPONENT_H_INCLUDED
#define OVK_CORE_OVERLAP_COMPONENT_H_INCLUDED

#include <ovk/core/Global.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  OVK_OVERLAP_EVENT_FLAGS_NONE = 0,
  OVK_OVERLAP_EVENT_FLAGS_CREATE = 1 << 0,
  OVK_OVERLAP_EVENT_FLAGS_DESTROY = 1 << 1,
  OVK_OVERLAP_EVENT_FLAGS_RESIZE_M = 1 << 2,
  OVK_OVERLAP_EVENT_FLAGS_EDIT_M_CELLS = 1 << 3,
  OVK_OVERLAP_EVENT_FLAGS_EDIT_M_COORDS = 1 << 4,
  OVK_OVERLAP_EVENT_FLAGS_EDIT_M_DESTINATIONS = 1 << 5,
  OVK_OVERLAP_EVENT_FLAGS_RESIZE_N = 1 << 6,
  OVK_OVERLAP_EVENT_FLAGS_EDIT_N_POINTS = 1 << 7,
  OVK_OVERLAP_EVENT_FLAGS_EDIT_N_SOURCES = 1 << 8,
  OVK_OVERLAP_EVENT_FLAGS_ALL_EDITS =
    OVK_OVERLAP_EVENT_FLAGS_RESIZE_M |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_CELLS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_COORDS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_DESTINATIONS |
    OVK_OVERLAP_EVENT_FLAGS_RESIZE_N |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_N_POINTS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_N_SOURCES,
  OVK_OVERLAP_EVENT_FLAGS_ALL =
    OVK_OVERLAP_EVENT_FLAGS_CREATE |
    OVK_OVERLAP_EVENT_FLAGS_DESTROY |
    OVK_OVERLAP_EVENT_FLAGS_RESIZE_M |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_CELLS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_COORDS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_M_DESTINATIONS |
    OVK_OVERLAP_EVENT_FLAGS_RESIZE_N |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_N_POINTS |
    OVK_OVERLAP_EVENT_FLAGS_EDIT_N_SOURCES
} ovk_overlap_event_flags;

static inline bool ovkValidOverlapEventFlags(ovk_overlap_event_flags EventFlags) {

  return EventFlags >= OVK_OVERLAP_EVENT_FLAGS_NONE && EventFlags <= OVK_OVERLAP_EVENT_FLAGS_ALL;

}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
constexpr inline ovk_overlap_event_flags operator|(ovk_overlap_event_flags Left,
  ovk_overlap_event_flags Right) {
  return ovk_overlap_event_flags(int(Left) | int(Right));
}
constexpr inline ovk_overlap_event_flags operator&(ovk_overlap_event_flags Left,
  ovk_overlap_event_flags Right) {
  return ovk_overlap_event_flags(int(Left) & int(Right));
}
constexpr inline ovk_overlap_event_flags operator^(ovk_overlap_event_flags Left,
  ovk_overlap_event_flags Right) {
  return ovk_overlap_event_flags(int(Left) ^ int(Right));
}
constexpr inline ovk_overlap_event_flags operator~(ovk_overlap_event_flags EventFlags) {
  return ovk_overlap_event_flags(~int(EventFlags));
}
inline ovk_overlap_event_flags operator|=(ovk_overlap_event_flags &Left, ovk_overlap_event_flags
  Right) {
  return Left = Left | Right;
}
inline ovk_overlap_event_flags operator&=(ovk_overlap_event_flags &Left, ovk_overlap_event_flags
  Right) {
  return Left = Left & Right;
}
inline ovk_overlap_event_flags operator^=(ovk_overlap_event_flags &Left, ovk_overlap_event_flags
  Right) {
  return Left = Left ^ Right;
}
#endif

#endif
