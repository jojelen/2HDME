// Oblique.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Functions that calculate oblique parameters for 2HDM.
 * @author: Joel Oredsson
 *  
 * Calculates S,T and U from the Higgs masses, mi, and the |q_{ij}|^2 vectors.
 * All formulas are taken from arXiv:1011.6188v2.
 * 
 * The masses are ordered as mi = {mh1, mh2, mh3, mHc}. 
 * 
 * Note that in some regions where the Higgs masses are too low, the integrals
 * encounter branch cuts and the oblique parameters are useless.
 * If the integrals turn complex, they return zero; thus one has to keep track 
 * of the problematic parameter space manually.
 * 
 * q_ij refers to the vectors in table 1 in arXiv:1011.6188v2.
 * 
 * The calculations depends on mW, mZ and sin(theta_W)^2 that are hard coded to 
 * be the ones in Globals.h.
 * S and T also depend on the measured SM Higgs boson mass.
 *
 *============================================================================*/
#pragma once

#include <vector>

namespace THDME {

// Calculates S, T and U and returns the result.
double calcS(const std::vector<double> &mi, const std::vector<double> &qi12);
double calcT(const std::vector<double> &mi, const std::vector<double> &qi12,
             const std::vector<double> &qi22);
double calcU(const std::vector<double> &mi, const std::vector<double> &qi12,
             const std::vector<double> &qi22);


//-----------------------------------------------------------------------------
// Functions that are needed for calculating S,T,U
//-----------------------------------------------------------------------------

// B_i in eq.5.12-13 in arXiv:1011.6188v2 (i = 0 or 22)
// We omit the Delta terms since they cancel out in physical observables.
double PVB(const int i, const double &q, const double &m1, const double &m2);

double Fm1m2(const double &m1,
             const double &m2); // eq.5.18 in arXiv:1011.6188v2
double GmV(const double &mV, const std::vector<double> &mi,
           const std::vector<double> &qi12); // eq.5.24 in arXiv:1011.6188v2

// Functions needed for the integration in PVB
struct integration_params_pvb {
  double q, m1, m2;
};
double b0_integrand(double x, void *p);
double b22_integrand(double x, void *p);
} // namespace THDME