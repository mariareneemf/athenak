//========================================================================================
// AthenaXXX astrophysical plasma code
// Copyright(C) 2020 James M. Stone <jmstone@ias.edu> and the Athena code team
// Licensed under the 3-clause BSD License (the "LICENSE")
//========================================================================================
//! \file eos.cpp
//! \brief implements constructor and some fns for EquationOfState abstract base class

#include <float.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "athena.hpp"
#include "mesh/mesh.hpp"
#include "parameter_input.hpp"
#include "eos/eos.hpp"

//----------------------------------------------------------------------------------------
// EquationOfState constructor

EquationOfState::EquationOfState(std::string bk, MeshBlockPack* pp, ParameterInput *pin) :
    pmy_pack(pp) {
  eos_data.dfloor = pin->GetOrAddReal(bk,"dfloor",(FLT_MIN));
  eos_data.pfloor = pin->GetOrAddReal(bk,"pfloor",(FLT_MIN));
  eos_data.tfloor = pin->GetOrAddReal(bk,"tfloor",(FLT_MIN));
  // tfloor only counts as set if above its default value
  // eos_data.use_tfloor = (eos_data.tfloor > (FLT_MIN));
  // 10x margin: default read back from a restart file may not equal FLT_MIN exactly
  eos_data.use_tfloor = (eos_data.tfloor > 10.0*(FLT_MIN));
  // optional trigger temperature for the tfloor reset (code units, like tfloor).
  // Default = tfloor, which reproduces tfloor acting as a plain minimum temperature.
  eos_data.tfloor_trigger = pin->GetOrAddReal(bk,"tfloor_trigger",eos_data.tfloor);
  if (eos_data.use_tfloor && eos_data.tfloor_trigger > eos_data.tfloor) {
    // a reset value below the trigger would re-trigger the floor on every call
    std::cout << "### FATAL ERROR in " << __FILE__ << " at line " << __LINE__
              << std::endl << "<" << bk << ">/tfloor_trigger = "
              << eos_data.tfloor_trigger << " must be <= tfloor = "
              << eos_data.tfloor << std::endl;
    std::exit(EXIT_FAILURE);
  }
  eos_data.sfloor = pin->GetOrAddReal(bk,"sfloor",(FLT_MIN));
  eos_data.debug_eos_statistic = pin->GetOrAddInteger(bk,"debug_eos_statistic",0);
  eos_data.debug_fill_zero = pin->GetOrAddInteger(bk,"debug_fill_zero",0);
}

//----------------------------------------------------------------------------------------
//! \fn void ConsToPrim()
//! \brief No-Op versions of hydro and MHD conservative to primitive functions.
//! Required because each derived class overrides only one.

void EquationOfState::ConsToPrim(DvceArray5D<Real> &cons, DvceArray5D<Real> &prim,
                                 const bool only_testfloors,
                                 const int il, const int iu, const int jl, const int ju,
                                 const int kl, const int ku) {
  Kokkos::abort("NoOp hydro ConsToPrim called.\n"
                "  If using MHD, call MHD version instead.\n"
                "  If using DynGRMHD, use the functions exposed in DynGRMHD instead.\n");
}

void EquationOfState::ConsToPrim(DvceArray5D<Real> &cons, const DvceFaceFld4D<Real> &b,
                                 DvceArray5D<Real> &prim, DvceArray5D<Real> &bcc,
                                 const bool only_testfloors,
                                 const int il, const int iu, const int jl, const int ju,
                                 const int kl, const int ku) {
  Kokkos::abort("NoOp MHD ConsToPrim called.\n"
                "  If using hydro, call hydro version instead.\n"
                "  If using DynGRMHD, use the functions exposed in DynGRMHD instead.\n");
}

//----------------------------------------------------------------------------------------
//! \fn void PrimToCon()
//! \brief No-Op versions of hydro and MHD primitive to conservative functions.
//! Required because each derived class overrides only one.

void EquationOfState::PrimToCons(const DvceArray5D<Real> &prim, DvceArray5D<Real> &cons,
                                 const int il, const int iu, const int jl, const int ju,
                                 const int kl, const int ku) {
  Kokkos::abort("NoOp hydro PrimToCons called.\n"
                "  If using MHD, call MHD version instead.\n"
                "  If using DynGRMHD, use the functions exposed in DynGRMHD instead.\n");
}
void EquationOfState::PrimToCons(const DvceArray5D<Real> &prim,
                                 const DvceArray5D<Real> &bcc, DvceArray5D<Real> &cons,
                                 const int il, const int iu, const int jl, const int ju,
                                 const int kl, const int ku) {
  Kokkos::abort("NoOp MHD PrimToCons called.\n"
                "  If using hydro, call hydro version instead.\n"
                "  If using DynGRMHD, use the functions exposed in DynGRMHD instead.\n");
}
