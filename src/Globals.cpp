// Globals.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Globals
 * @author: Joel Oredsson
 *============================================================================*/
#include "Globals.h"
#include "HelpFunctions.h"

#include <cmath>
#include <vector>

namespace THDME {
constexpr double Global::Qf[3];

Eigen::Matrix3cd get_pdg_ckm_matrix() {
  // Parameterisation of CKM matrix from PDG 2018.
  constexpr static double lambda = 0.22453;
  constexpr static double A = 0.836;
  constexpr static double rhobar = 0.122;
  constexpr static double etabar = 0.355;

  Eigen::Matrix3cd CKM_input = createCkmMatrix(lambda, A, rhobar, etabar);

  return CKM_input;
}
} // namespace THDME