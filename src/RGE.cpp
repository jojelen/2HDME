// RGE.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: RGEs for models that derive RGE functionality from RgeModel
 * @author: Joel Oredsson
 *============================================================================*/
#include "RGE.h"
#include "HelpFunctions.h"

#include <complex>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

namespace THDME {

/* rgeFuncThdm_1loop calculates the general complex 2HDM RGEs to 1-loop in the 
  generic basis. */
int rgeFuncThdm_1loop(double t, const double y[], double dydt[], void *params)
{
  // Initialization of Yukawa matrices
  Matrix3cd eta1U(3, 3), eta2U(3, 3), eta1D(3, 3), eta2D(3, 3),
            eta1L(3, 3), eta2L(3, 3);
  Matrix3cd Deta1U_1(3, 3), Deta2U_1(3, 3), Deta1D_1(3, 3), Deta2D_1(3, 3),
            Deta1L_1(3, 3), Deta2L_1(3, 3);

  Matrix3cd N1(3, 3), N2(3, 3);

  double Dg1_1, Dg2_1, Dg3_1;

  // Potential
  double Dtanb;
  complex<double> v1, v2, Dv1_1, Dv2_1;

  double M112, M222;
  complex<double> M122;
  double DM112_1, DM222_1;
  complex<double> DM122_1;

  double Lambda1, Lambda2, Lambda3, Lambda4;
  complex<double> Lambda5, Lambda6, Lambda7;
  double DLambda1_1, DLambda2_1, DLambda3_1, DLambda4_1;
  complex<double> DLambda5_1, DLambda6_1, DLambda7_1;

  // Initial conditions
  double g1 = y[0];
  double g2 = y[1];
  double g3 = y[2];
  v1 = complex<double>(y[3], y[4]);
  v2 = complex<double>(y[5], y[6]);
  // double tanb = y[7]; // Not used anymore

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      eta1U(i, j) = complex<double>(y[8 + i * 3 + j], y[17 + i * 3 + j]);
      eta1D(i, j) = complex<double>(y[26 + i * 3 + j], y[35 + i * 3 + j]);
      eta1L(i, j) = complex<double>(y[44 + i * 3 + j], y[53 + i * 3 + j]);
      eta2U(i, j) = complex<double>(y[62 + i * 3 + j], y[71 + i * 3 + j]);
      eta2D(i, j) = complex<double>(y[80 + i * 3 + j], y[89 + i * 3 + j]);
      eta2L(i, j) = complex<double>(y[98 + i * 3 + j], y[107 + i * 3 + j]);
    };

  M112 = y[116];
  M222 = y[117];
  M122 = complex<double>(y[118], y[119]);
  Lambda1 = y[120];
  Lambda2 = y[121];
  Lambda3 = y[122];
  Lambda4 = y[123];
  Lambda5 = complex<double>(y[124], y[125]);
  Lambda6 = complex<double>(y[126], y[127]);
  Lambda7 = complex<double>(y[128], y[129]);

  static double pi = M_PI;

  /*---------Calculating-RGEs-------------------------------------------------*/

  // tanb does not run. Instead calculate tanb from the running VEVs
  Dtanb = 0.;
  // The general RGEs are loaded from the files in "RGEs".
  Dg1_1 = real(
#include "RGEs/g1_1loop_cForm.h"
  );
  Dg2_1 = real(
#include "RGEs/g2_1loop_cForm.h"
  );
  Dg3_1 = real(
#include "RGEs/g3_1loop_cForm.h"
  );
  Dv1_1 = (
#include "RGEs/g11_1loop_cForm.h"
              ) *
              v1 +
          (
#include "RGEs/g12_1loop_cForm.h"
              ) *
              v2;
  Dv2_1 = (
#include "RGEs/g22_1loop_cForm.h"
              ) *
              v2 +
          (
#include "RGEs/g12_1loop_cForm.h"
              ) *
              v1;
  Deta1U_1 =
#include "RGEs/eta1U_1loop_cForm.h"
      ;
  Deta2U_1 =
#include "RGEs/eta2U_1loop_cForm.h"
      ;
  Deta1D_1 =
#include "RGEs/eta1D_1loop_cForm.h"
      ;
  Deta2D_1 =
#include "RGEs/eta2D_1loop_cForm.h"
      ;
  Deta1L_1 =
#include "RGEs/eta1L_1loop_cForm.h"
      ;
  Deta2L_1 =
#include "RGEs/eta2L_1loop_cForm.h"
      ;
  DM112_1 = real(
#include "RGEs/m112_1loop_cForm.h"
  );
  DM222_1 = real(
#include "RGEs/m222_1loop_cForm.h"
  );
  DM122_1 =
#include "RGEs/m122_1loop_cForm.h"
      ;
  DLambda1_1 = real(
#include "RGEs/lambda1_1loop_cForm.h"
  );
  DLambda2_1 = real(
#include "RGEs/lambda2_1loop_cForm.h"
  );
  DLambda3_1 = real(
#include "RGEs/lambda3_1loop_cForm.h"
  );
  DLambda4_1 = real(
#include "RGEs/lambda4_1loop_cForm.h"
  );
  DLambda5_1 =
#include "RGEs/lambda5_1loop_cForm.h"
      ;
  DLambda6_1 =
#include "RGEs/lambda6_1loop_cForm.h"
      ;
  DLambda7_1 =
#include "RGEs/lambda7_1loop_cForm.h"
      ;

  // fill derivatives in dydt

  static double pif = 1. / (16. * pi * pi);

  dydt[0] = pif * Dg1_1;
  dydt[1] = pif * Dg2_1;
  dydt[2] = pif * Dg3_1;
  dydt[3] = pif * real(Dv1_1);
  dydt[4] = pif * imag(Dv1_1);
  dydt[5] = pif * real(Dv2_1);
  dydt[6] = pif * imag(Dv2_1);
  dydt[7] = pif * Dtanb;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      dydt[8 + i * 3 + j] = pif * real(Deta1U_1(i, j));
      dydt[17 + i * 3 + j] = pif * imag(Deta1U_1(i, j));
      dydt[26 + i * 3 + j] = pif * real(Deta1D_1(i, j));
      dydt[35 + i * 3 + j] = pif * imag(Deta1D_1(i, j));
      dydt[44 + i * 3 + j] = pif * real(Deta1L_1(i, j));
      dydt[53 + i * 3 + j] = pif * imag(Deta1L_1(i, j));
      dydt[62 + i * 3 + j] = pif * real(Deta2U_1(i, j));
      dydt[71 + i * 3 + j] = pif * imag(Deta2U_1(i, j));
      dydt[80 + i * 3 + j] = pif * real(Deta2D_1(i, j));
      dydt[89 + i * 3 + j] = pif * imag(Deta2D_1(i, j));
      dydt[98 + i * 3 + j] = pif * real(Deta2L_1(i, j));
      dydt[107 + i * 3 + j] = pif * imag(Deta2L_1(i, j));
    }

  dydt[116] = pif * DM112_1;
  dydt[117] = pif * DM222_1;
  dydt[118] = pif * real(DM122_1);
  dydt[119] = pif * imag(DM122_1);
  dydt[120] = pif * DLambda1_1;
  dydt[121] = pif * DLambda2_1;
  dydt[122] = pif * DLambda3_1;
  dydt[123] = pif * DLambda4_1;
  dydt[124] = pif * real(DLambda5_1);
  dydt[125] = pif * imag(DLambda5_1);
  dydt[126] = pif * real(DLambda6_1);
  dydt[127] = pif * imag(DLambda6_1);
  dydt[128] = pif * real(DLambda7_1);
  dydt[129] = pif * imag(DLambda7_1);

  return GSL_SUCCESS;
}

/* rgeFuncThdm_2loop calculates the general complex 2HDM RGEs to 1-loop in the 
  generic basis. */
int rgeFuncThdm_2loop(double t, const double y[], double dydt[], void *params)
{

  // Initialization
  // Yukawa matrices
  Matrix3cd eta1U(3, 3), eta2U(3, 3), eta1D(3, 3), eta2D(3, 3),
      eta1L(3, 3), eta2L(3, 3);
  Matrix3cd Deta1U_1(3, 3), Deta2U_1(3, 3), Deta1D_1(3, 3), Deta2D_1(3, 3),
      Deta1L_1(3, 3), Deta2L_1(3, 3);
  Matrix3cd Deta1U_2(3, 3), Deta2U_2(3, 3), Deta1D_2(3, 3), Deta2D_2(3, 3),
      Deta1L_2(3, 3), Deta2L_2(3, 3);

  Matrix3cd N1(3, 3), N2(3, 3);

  double Dg1_1, Dg2_1, Dg3_1, Dg1_2, Dg2_2, Dg3_2;

  // Potential
  double Dtanb;
  complex<double> v1, v2, Dv1_1, Dv2_1, Dv1_2, Dv2_2;

  double M112, M222;
  complex<double> M122;
  double DM112_1, DM222_1, DM112_2, DM222_2;
  complex<double> DM122_1, DM122_2;

  double Lambda1, Lambda2, Lambda3, Lambda4;
  complex<double> Lambda5, Lambda6, Lambda7;
  double DLambda1_1, DLambda2_1, DLambda3_1, DLambda4_1;
  complex<double> DLambda5_1, DLambda6_1, DLambda7_1;
  double DLambda1_2, DLambda2_2, DLambda3_2, DLambda4_2;
  complex<double> DLambda5_2, DLambda6_2, DLambda7_2;

  //Initial conditions
  double g1 = y[0];
  double g2 = y[1];
  double g3 = y[2];
  v1 = complex<double>(y[3], y[4]);
  v2 = complex<double>(y[5], y[6]);
  // double tanb = y[7]; // Not used anymore

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      eta1U(i, j) = complex<double>(y[8 + i * 3 + j], y[17 + i * 3 + j]);
      eta1D(i, j) = complex<double>(y[26 + i * 3 + j], y[35 + i * 3 + j]);
      eta1L(i, j) = complex<double>(y[44 + i * 3 + j], y[53 + i * 3 + j]);
      eta2U(i, j) = complex<double>(y[62 + i * 3 + j], y[71 + i * 3 + j]);
      eta2D(i, j) = complex<double>(y[80 + i * 3 + j], y[89 + i * 3 + j]);
      eta2L(i, j) = complex<double>(y[98 + i * 3 + j], y[107 + i * 3 + j]);
    };

  M112 = y[116];
  M222 = y[117];
  M122 = complex<double>(y[118], y[119]);
  Lambda1 = y[120];
  Lambda2 = y[121];
  Lambda3 = y[122];
  Lambda4 = y[123];
  Lambda5 = complex<double>(y[124], y[125]);
  Lambda6 = complex<double>(y[126], y[127]);
  Lambda7 = complex<double>(y[128], y[129]);

  static double pi = M_PI;

  /*---------Calculating-RGEs-------------------------------------------------*/

  // tanb does not run. Instead calculate tanb from the running VEVs
  Dtanb = 0.;
  // The general RGEs are loaded from the files in "RGEs".
  Dg1_1 = real(
#include "RGEs/g1_1loop_cForm.h"
  );
  Dg2_1 = real(
#include "RGEs/g2_1loop_cForm.h"
  );
  Dg3_1 = real(
#include "RGEs/g3_1loop_cForm.h"
  );
  Dv1_1 = (
#include "RGEs/g11_1loop_cForm.h"
              ) *
              v1 +
          (
#include "RGEs/g12_1loop_cForm.h"
              ) *
              v2;
  Dv2_1 = (
#include "RGEs/g22_1loop_cForm.h"
              ) *
              v2 +
          (
#include "RGEs/g12_1loop_cForm.h"
              ) *
              v1;
  Deta1U_1 =
#include "RGEs/eta1U_1loop_cForm.h"
      ;
  Deta2U_1 =
#include "RGEs/eta2U_1loop_cForm.h"
      ;
  Deta1D_1 =
#include "RGEs/eta1D_1loop_cForm.h"
      ;
  Deta2D_1 =
#include "RGEs/eta2D_1loop_cForm.h"
      ;
  Deta1L_1 =
#include "RGEs/eta1L_1loop_cForm.h"
      ;
  Deta2L_1 =
#include "RGEs/eta2L_1loop_cForm.h"
      ;
  DM112_1 = real(
#include "RGEs/m112_1loop_cForm.h"
  );
  DM222_1 = real(
#include "RGEs/m222_1loop_cForm.h"
  );
  DM122_1 =
#include "RGEs/m122_1loop_cForm.h"
      ;
  DLambda1_1 = real(
#include "RGEs/lambda1_1loop_cForm.h"
  );
  DLambda2_1 = real(
#include "RGEs/lambda2_1loop_cForm.h"
  );
  DLambda3_1 = real(
#include "RGEs/lambda3_1loop_cForm.h"
  );
  DLambda4_1 = real(
#include "RGEs/lambda4_1loop_cForm.h"
  );
  DLambda5_1 =
#include "RGEs/lambda5_1loop_cForm.h"
      ;
  DLambda6_1 =
#include "RGEs/lambda6_1loop_cForm.h"
      ;
  DLambda7_1 =
#include "RGEs/lambda7_1loop_cForm.h"
      ;

  // Twoloop terms
  Dg1_2 = real(
#include "RGEs/g1_2loop_cForm.h"
  );
  Dg2_2 = real(
#include "RGEs/g2_2loop_cForm.h"
  );
  Dg3_2 = real(
#include "RGEs/g3_2loop_cForm.h"
  );
  Dv1_2 = (
#include "RGEs/g11_2loopB_cForm.h"
              +
#include "RGEs/g11_2loopF_cForm.h"
              ) *
              v1 +
          (
#include "RGEs/g12_2loopB_cForm.h"
              +
#include "RGEs/g12_2loopF_cForm.h"
              ) *
              v2;
  Dv2_2 = (
#include "RGEs/g22_2loopB_cForm.h"
              +
#include "RGEs/g22_2loopF_cForm.h"
              ) *
              v2 +
          (
#include "RGEs/g12_2loopB_cForm.h"
              +
#include "RGEs/g12_2loopF_cForm.h"
              ) *
              v1;
  Deta1U_2 =
#include "RGEs/eta1U_2loop_cForm.h"
      ;
  Deta2U_2 =
#include "RGEs/eta2U_2loop_cForm.h"
      ;
  Deta1D_2 =
#include "RGEs/eta1D_2loop_cForm.h"
      ;
  Deta2D_2 =
#include "RGEs/eta2D_2loop_cForm.h"
      ;
  Deta1L_2 =
#include "RGEs/eta1L_2loop_cForm.h"
      ;
  Deta2L_2 =
#include "RGEs/eta2L_2loop_cForm.h"
      ;
  DM112_2 = real(
#include "RGEs/m112_2loop_cForm.h"
  );
  DM222_2 = real(
#include "RGEs/m222_2loop_cForm.h"
  );
  DM122_2 =
#include "RGEs/m122_2loop_cForm.h"
      ;
  DLambda1_2 = real(
#include "RGEs/lambda1_2loop_cForm.h"
  );
  DLambda2_2 = real(
#include "RGEs/lambda2_2loop_cForm.h"
  );
  DLambda3_2 = real(
#include "RGEs/lambda3_2loop_cForm.h"
  );
  DLambda4_2 = real(
#include "RGEs/lambda4_2loop_cForm.h"
  );
  DLambda5_2 =
#include "RGEs/lambda5_2loop_cForm.h"
      ;
  DLambda6_2 =
#include "RGEs/lambda6_2loop_cForm.h"
      ;
  DLambda7_2 =
#include "RGEs/lambda7_2loop_cForm.h"
      ;

  // fill derivatives in dydt

  static double pif = 1. / (16. * pi * pi);

  dydt[0] = pif * Dg1_1 + pif * pif * Dg1_2;
  dydt[1] = pif * Dg2_1 + pif * pif * Dg2_2;
  dydt[2] = pif * Dg3_1 + pif * pif * Dg3_2;
  dydt[3] = pif * real(Dv1_1) + pif * pif * real(Dv1_2);
  dydt[4] = pif * imag(Dv1_1) + pif * pif * imag(Dv1_2);
  dydt[5] = pif * real(Dv2_1) + pif * pif * real(Dv2_2);
  dydt[6] = pif * imag(Dv2_1) + pif * pif * imag(Dv2_2);
  dydt[7] = pif * Dtanb;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      dydt[8 + i * 3 + j] = pif * real(Deta1U_1(i, j)) + pif * pif * 
      real(Deta1U_2(i, j));
      dydt[17 + i * 3 + j] = pif * imag(Deta1U_1(i, j)) + pif * pif * 
      imag(Deta1U_2(i, j));
      dydt[26 + i * 3 + j] = pif * real(Deta1D_1(i, j)) + pif * pif * 
      real(Deta1D_2(i, j));
      dydt[35 + i * 3 + j] = pif * imag(Deta1D_1(i, j)) + pif * pif * 
      imag(Deta1D_2(i, j));
      dydt[44 + i * 3 + j] = pif * real(Deta1L_1(i, j)) + pif * pif * 
      real(Deta1L_2(i, j));
      dydt[53 + i * 3 + j] = pif * imag(Deta1L_1(i, j)) + pif * pif * 
      imag(Deta1L_2(i, j));
      dydt[62 + i * 3 + j] = pif * real(Deta2U_1(i, j)) + pif * pif * 
      real(Deta2U_2(i, j));
      dydt[71 + i * 3 + j] = pif * imag(Deta2U_1(i, j)) + pif * pif * 
      imag(Deta2U_2(i, j));
      dydt[80 + i * 3 + j] = pif * real(Deta2D_1(i, j)) + pif * pif * 
      real(Deta2D_2(i, j));
      dydt[89 + i * 3 + j] = pif * imag(Deta2D_1(i, j)) + pif * pif * 
      imag(Deta2D_2(i, j));
      dydt[98 + i * 3 + j] = pif * real(Deta2L_1(i, j)) + pif * pif * 
      real(Deta2L_2(i, j));
      dydt[107 + i * 3 + j] = pif * imag(Deta2L_1(i, j)) + pif * pif * 
      imag(Deta2L_2(i, j));
    }

  dydt[116] = pif * DM112_1 + pif * pif * DM112_2;
  dydt[117] = pif * DM222_1 + pif * pif * DM222_2;
  dydt[118] = pif * real(DM122_1) + pif * pif * real(DM122_2);
  dydt[119] = pif * imag(DM122_1) + pif * pif * imag(DM122_2);
  dydt[120] = pif * DLambda1_1 + pif * pif * DLambda1_2;
  dydt[121] = pif * DLambda2_1 + pif * pif * DLambda2_2;
  dydt[122] = pif * DLambda3_1 + pif * pif * DLambda3_2;
  dydt[123] = pif * DLambda4_1 + pif * pif * DLambda4_2;
  dydt[124] = pif * real(DLambda5_1) + pif * pif * real(DLambda5_2);
  dydt[125] = pif * imag(DLambda5_1) + pif * pif * imag(DLambda5_2);
  dydt[126] = pif * real(DLambda6_1) + pif * pif * real(DLambda6_2);
  dydt[127] = pif * imag(DLambda6_1) + pif * pif * imag(DLambda6_2);
  dydt[128] = pif * real(DLambda7_1) + pif * pif * real(DLambda7_2);
  dydt[129] = pif * imag(DLambda7_1) + pif * pif * imag(DLambda7_2);

  return GSL_SUCCESS;
}

//------------------------------------------------------------------------------

int rgeFuncSm_1loop(double t, const double y[], double dydt[], void *params) {
  using namespace std;
  using namespace Eigen;

  // Initializing matrices
  Eigen::Matrix3cd yU, yD, yL;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      yU(i, j) = std::complex<double>(y[5 + i * 3 + j], y[14 + i * 3 + j]);
      yD(i, j) = std::complex<double>(y[23 + i * 3 + j], y[32 + i * 3 + j]);
      yL(i, j) = std::complex<double>(y[41 + i * 3 + j], y[50 + i * 3 + j]);
    }

  // Initial conditions
  double g1 = y[0];
  double g2 = y[1];
  double g3 = y[2];
  double v = y[3];
  double lambda = y[4];

  constexpr static double pi = M_PI;

  //---------Calculating-RGEs---------------------------------------------------

  double Dg1_1, Dg2_1, Dg3_1, Dv_1, Dlambda_1;
  Eigen::Matrix3cd DyU_1, DyD_1, DyL_1;

  // The general RGEs are loaded from the files in "RGEs".
  Dg1_1 =
#include "RGEs/SM/g1_1loop_cForm.h"
      ;
  Dg2_1 =
#include "RGEs/SM/g2_1loop_cForm.h"
      ;
  Dg3_1 =
#include "RGEs/SM/g3_1loop_cForm.h"
      ;
  Dv_1 = real(
#include "RGEs/SM/g_1loop_cForm.h"
             ) *
         v;
  Dlambda_1 = real(
#include "RGEs/SM/lambda_1loop_cForm.h"
  );
  DyU_1 =
#include "RGEs/SM/yU_1loop_cForm.h"
      ;
  DyD_1 =
#include "RGEs/SM/yD_1loop_cForm.h"
      ;
  DyL_1 =
#include "RGEs/SM/yL_1loop_cForm.h"
      ;

  // fill derivatives in dydt

  constexpr static double pif = 1. / (16. * pi * pi);

  dydt[0] = pif * Dg1_1;
  dydt[1] = pif * Dg2_1;
  dydt[2] = pif * Dg3_1;
  dydt[3] = pif * Dv_1;
  dydt[4] = pif * Dlambda_1;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      dydt[5 + i * 3 + j] = pif * real(DyU_1(i, j));
      dydt[14 + i * 3 + j] = pif * imag(DyU_1(i, j));
      dydt[23 + i * 3 + j] = pif * real(DyD_1(i, j));
      dydt[32 + i * 3 + j] = pif * imag(DyD_1(i, j));
      dydt[41 + i * 3 + j] = pif * real(DyL_1(i, j));
      dydt[50 + i * 3 + j] = pif * imag(DyL_1(i, j));
    }

  return GSL_SUCCESS;
}

int rgeFuncSm_2loop(double t, const double y[], double dydt[], void *params) {
  using namespace std;
  using namespace Eigen;

  // Initializing matrices
  Eigen::Matrix3cd yU, yD, yL;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      yU(i, j) = std::complex<double>(y[5 + i * 3 + j], y[14 + i * 3 + j]);
      yD(i, j) = std::complex<double>(y[23 + i * 3 + j], y[32 + i * 3 + j]);
      yL(i, j) = std::complex<double>(y[41 + i * 3 + j], y[50 + i * 3 + j]);
    }

  // Initial conditions
  double g1 = y[0];
  double g2 = y[1];
  double g3 = y[2];
  double v = y[3];
  double lambda = y[4];

  constexpr static double pi = M_PI;

  //---------Calculating-RGEs---------------------------------------------------

  double Dg1_1, Dg2_1, Dg3_1, Dv_1, Dlambda_1, Dg1_2, Dg2_2, Dg3_2, Dv_2,
      Dlambda_2;
  Eigen::Matrix3cd DyU_1, DyD_1, DyL_1, DyU_2, DyD_2, DyL_2;

  // The general RGEs are loaded from the files in "RGEs".
  Dg1_1 =
#include "RGEs/SM/g1_1loop_cForm.h"
      ;
  Dg2_1 =
#include "RGEs/SM/g2_1loop_cForm.h"
      ;
  Dg3_1 =
#include "RGEs/SM/g3_1loop_cForm.h"
      ;
  Dv_1 = real(
#include "RGEs/SM/g_1loop_cForm.h"
             ) *
         v;
  Dlambda_1 = real(
#include "RGEs/SM/lambda_1loop_cForm.h"
  );
  DyU_1 =
#include "RGEs/SM/yU_1loop_cForm.h"
      ;
  DyD_1 =
#include "RGEs/SM/yD_1loop_cForm.h"
      ;
  DyL_1 =
#include "RGEs/SM/yL_1loop_cForm.h"
      ;

  // The general RGEs are loaded from the files in "RGEs".
  Dg1_2 = real(
#include "RGEs/SM/g1_2loop_cForm.h"
  );
  Dg2_2 = real(
#include "RGEs/SM/g2_2loop_cForm.h"
  );
  Dg3_2 = real(
#include "RGEs/SM/g3_2loop_cForm.h"
  );
  Dv_2 = real(
#include "RGEs/SM/g_2loopB_cForm.h"
             +
#include "RGEs/SM/g_2loopF_cForm.h"
             ) *
         v;
  Dlambda_2 = real(
#include "RGEs/SM/lambda_2loop_cForm.h"
  );
  DyU_2 =
#include "RGEs/SM/yU_2loop_cForm.h"
      ;
  DyD_2 =
#include "RGEs/SM/yD_2loop_cForm.h"
      ;
  DyL_2 =
#include "RGEs/SM/yL_2loop_cForm.h"
      ;

  // fill derivatives in dydt

  constexpr static double pif = 1. / (16. * pi * pi);
  constexpr static double pif2 = pif * pif;

  dydt[0] = pif * Dg1_1 + pif2 * Dg1_2;
  dydt[1] = pif * Dg2_1 + pif2 * Dg2_2;
  dydt[2] = pif * Dg3_1 + pif2 * Dg3_2;
  dydt[3] = pif * Dv_1 + pif2 * Dv_2;
  dydt[4] = pif * Dlambda_1 + pif2 * Dlambda_2;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      dydt[5 + i * 3 + j] = pif * real(DyU_1(i, j)) + pif2 * real(DyU_2(i, j));
      dydt[14 + i * 3 + j] = pif * imag(DyU_1(i, j)) + pif2 * imag(DyU_2(i, j));
      dydt[23 + i * 3 + j] = pif * real(DyD_1(i, j)) + pif2 * real(DyD_2(i, j));
      dydt[32 + i * 3 + j] = pif * imag(DyD_1(i, j)) + pif2 * imag(DyD_2(i, j));
      dydt[41 + i * 3 + j] = pif * real(DyL_1(i, j)) + pif2 * real(DyL_2(i, j));
      dydt[50 + i * 3 + j] = pif * imag(DyL_1(i, j)) + pif2 * imag(DyL_2(i, j));
    }

  return GSL_SUCCESS;
}

}