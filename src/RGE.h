// RGE.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: RGEs for models that derive RGE functionality from RgeModel
 * @author: Joel Oredsson
 *
 * The functions
 *   int func(double t, const double y[], double dydt[], void *params)
 * calculates dy/dt that can depend on y and t, i.e. the RGEs for a QftModel
 * at the renormalization scale t = log(mu).
 *
 * Usage: Pass the functions as arguments in the constructor of
 *        gsl_odeiv2_system.
 *
 *============================================================================*/
#pragma once

namespace THDME {

/**
* @brief RGEs for a general THDM and the SM
*
* These functions feed the RGEs collected in the "RGEs" folder to the ODE
* solver.
* The RGEs have been derived in Mathematica using the general RGE formalism
* written down in Phys.Rev. D67 (2003) 065019.
*
* Use one of the functions depending on the loop order wanted, i.e. the 2-loop
* function contains the 1-loop RGEs as well.
*
* There is a total of 130 (59) real parameters in y[] and dydt[] for 2HDM (SM).
*/
int rgeFuncThdm_1loop(double t, const double y[], double dydt[], void *params);

int rgeFuncThdm_2loop(double t, const double y[], double dydt[], void *params);

int rgeFuncSm_1loop(double t, const double y[], double dydt[], void *params);

int rgeFuncSm_2loop(double t, const double y[], double dydt[], void *params);
}
