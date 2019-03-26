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
  // --- These masses are for compatibility test with HDECAY ---
  // Scale for HD quark masses (HD)
  constexpr static double Q_HD = 5.0;
  // switch for HD quark masses (HD)
  constexpr static bool b_HD = false;
  // Scale for strange quark mass (GeV)
  constexpr static double Q_ms = 2.0;
};

/**
* @brief: Returns the fermion MS-bar/pole mass
*
* The pole masses are simply the ones in the Global struct, taken from the PDG.
* The MS-bar masses are determined from these pole masses at the respective
* pole mass scale.
*/
double get_qmass_pole(int flav);
double get_qmass_MSbar(int flav);
double get_dmass_pole(int d);
double get_dmass_MSbar(int d);
double get_umass_pole(int u);
double get_umass_MSbar(int u);
double get_lmass_pole(int l);

/**
 * @brief: Returns the QCD beta function coefficients
 * 
 * @params:
 *   c: Loop order
 *   nf: Number of fermion flavors.
 */
double QCD_beta(int c, int nf);

/**
  * @brief: Evaluates the running strong coupling
  *
  * This method evaluates the running strong coupling alpha_s in the
  * MSbar scheme at the scale specified by Q.
  *
  * @params:
  *   Q          Scale at which to evaluate the running coupling
  *   mtop       MSbar top mass (for thresholds)
  *   mbot       MSbar bottom mass (for thresholds)
  *
  * @returns The strong coupling alpha_s at the scale Q
  */
double run_alphas_MSbar(double Q, double mtop, double mbot);

/**
  * @brief: Evaluates running quark masses
  *
  * This method evaluates the running quark_mass quark_mass, initially specified
  * at the input scale Qinit, at the new scale Qfin. The calculation is
 * performed
  * in the MSbar scheme with variable number of active
  * flavours and threshold matching.
  *
  * @params:
  *   quark_mass The input value for the running quark mass
  *   Qinit      Starting scale
  *   Qfin       Final scale at which the mass is to be evaluated
  *   mtop       MSbar top mass (for thresholds)
  *   mbot       MSbar bottom mass (for thresholds)
  *
  * @returns The running mass at the scale Qfin
  */
double run_qmass_MSbar(double quark_mass, double Qinit, double Qfin,
                       double mtop, double mbot);

double get_gamma_tWd(int d);

// Returns the CKM matrix from the Particle Data Group.
Eigen::Matrix3cd get_pdg_ckm_matrix();

/**
* @brief Number of active flavours
*
* This method returns the number of active quark flavours to be used at
* a certain mass scale when decoupling the heavier quarks. The thresholds
* are determined from the quark MS-bar masses.
*
* @param M The scale for which the number of flavours is desired
*
* @returns The number of quark flavours active at this scale
*/
int get_Nactivef(double M);
} // namespace THDME