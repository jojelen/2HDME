// Globals.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Globals
 * @author: Joel Oredsson
 *
 * Collection of constants.
 *
 *============================================================================*/
#pragma once

#include <Eigen/Dense>

namespace THDME {

struct Global {
  //---------------------------------------------------------------------------
  // Pole masses (in GeV) from PDG 2018:
  constexpr static double mZ = 91.1876;
  constexpr static double mW = 80.379;
  constexpr static double mh = 125.18;

  // Quarks:
  constexpr static double mu = 2.2e-3;
  constexpr static double md = 4.7e-3;
  constexpr static double ms = 95e-3;
  constexpr static double mc = 1.275;
  constexpr static double mb = 4.18;
  constexpr static double mt = 173.1; // Pole from cross-section measurements

  // Leptons:
  constexpr static double me = 0.5109989461e-3;
  constexpr static double mmuon = 105.6583745e-3;
  constexpr static double mtau = 1.77686;

  //---------------------------------------------------------------------------
  // Misc:
  constexpr static bool CKM_PDG_CONVENTION = true;
  constexpr static double GF = 1.1663787e-5; // Fermi's constant
  constexpr static double v = 246.21971;     // VEV
  constexpr static double alpha_e_me = 1. / 137.0359997;
  constexpr static double alpha_s_mZ = 0.1181; // World average from PDG 2018
  constexpr static double sW2 = 0.22343; // sin(theta_W)^2 (on-shell scheme)
  // Total widths from PDG 2018 in GeV:
  constexpr static double W_width = 2.085;
  constexpr static double Z_width = 2.4952;
  constexpr static double top_width = 1.41;

  //---------------------------------------------------------------------------
  // Quantum numbers:
  constexpr static int nC = 3;                              // Number of colors
  constexpr static double Qf[3] = {2. / 3., -1. / 3., -1.}; // Charge of u,d,e

  //---------------------------------------------------------------------------
  // Used by 2HDMC functions
  // Scale for strange quark mass (GeV)
  constexpr static double Q_ms = 2.0;
};

// Returns the CKM matrix from the Particle Data Group.
Eigen::Matrix3cd get_pdg_ckm_matrix();

} // namespace THDME