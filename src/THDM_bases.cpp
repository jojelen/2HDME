// THDM_bases.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Miscellaneous functions related to THDM
 * @author: Joel Oredsson
 *============================================================================*/
#include "THDM_bases.h"
#include "HelpFunctions.h"
#include "ParameterScan.h"

#include <Eigen/Dense>

using std::complex;
using std::cout;
using std::string;
using std::vector;

namespace THDME
{

// "<<" operator overloading

std::ostream &operator<<(std::ostream &oS, const Base_generic &gen)
{
  oS << "Generic basis:\n"
     << "beta = " << gen.beta << ", tanb = " << tan(gen.beta) << std::endl
     << "M112 = " << gen.M112 << std::endl
     << "M222 = " << gen.M222 << std::endl
     << "M12 = " << gen.M12 << std::endl
     << "Lambda1 = " << gen.Lambda1 << std::endl
     << "Lambda2 = " << gen.Lambda2 << std::endl
     << "Lambda3 = " << gen.Lambda3 << std::endl
     << "Lambda4 = " << gen.Lambda4 << std::endl
     << "Lambda5 = " << gen.Lambda5 << std::endl
     << "Lambda6 = " << gen.Lambda6 << std::endl
     << "Lambda7 = " << gen.Lambda7 << std::endl;
  return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_compact &comp)
{
  oS << "Compact basis:\n";
  std::cout << "tanb = " << tan(comp.beta) << std::endl;
  std::cout << "Y_11 = " << comp.Y[0][0] << std::endl;
  std::cout << "Y_12 = " << comp.Y[0][1] << std::endl;
  std::cout << "Y_21 = " << comp.Y[1][0] << std::endl;
  std::cout << "Y_22 = " << comp.Y[1][1] << std::endl;
  std::cout << "\nZ_1111 = " << comp.Z[0][0][0][0] << std::endl;
  std::cout << "Z_2222 = " << comp.Z[1][1][1][1] << std::endl;
  std::cout << "Z_1122 = Z_2211 = " << comp.Z[0][0][1][1] << std::endl;
  std::cout << "Z_1221 = Z_2112 = " << comp.Z[0][1][1][0] << std::endl;
  std::cout << "Z_1212 = " << comp.Z[0][1][0][1] << std::endl;
  std::cout << "Z_2121 = " << comp.Z[1][0][1][0] << std::endl;
  std::cout << "Z_1112 = Z_1211 = " << comp.Z[0][0][0][1] << std::endl;
  std::cout << "Z_1121 = Z_2111 = " << comp.Z[0][0][1][0] << std::endl;
  std::cout << "Z_2212 = Z_1222 = " << comp.Z[0][1][1][1] << std::endl;
  std::cout << "Z_2221 = Z_2122 = " << comp.Z[1][1][1][0] << std::endl;
  return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_higgs &higgs)
{
  oS << "Higgs basis:\n"
     << "Y1 = " << higgs.Y1 << std::endl
     << "Y2 = " << higgs.Y2 << std::endl
     << "Y3 = " << higgs.Y3 << std::endl
     << "Z1 = " << higgs.Z1 << std::endl
     << "Z2 = " << higgs.Z2 << std::endl
     << "Z3 = " << higgs.Z3 << std::endl
     << "Z4 = " << higgs.Z4 << std::endl
     << "Z5 = " << higgs.Z5 << std::endl
     << "Z6 = " << higgs.Z6 << std::endl
     << "Z7 = " << higgs.Z7 << std::endl;
  return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_invariant &inv)
{
  oS << "Invariant basis (theta23 = " << inv.theta23 << " ):\n"
     << "mh[0] = " << inv.mh[0] << std::endl
     << "mh[1] = " << inv.mh[1] << std::endl
     << "mh[2] = " << inv.mh[2] << std::endl
     << "mHc = " << inv.mHc << std::endl
     << "Z2 = " << inv.Z2 << std::endl
     << "Z3 = " << inv.Z3 << std::endl
     << "Z7inv =" << inv.Z7inv << std::endl
     << "s12 = " << inv.s12 << std::endl
     << "c13 = " << inv.c13 << std::endl
     << "tanb = " << tan(inv.beta) << std::endl;
  return oS;
}

std::ofstream &operator<<(std::ofstream &of, const Base_invariant &inv)
{
  of << "Invariant basis (theta23 = " << inv.theta23 << " ):\n"
     << "mh[0] = " << inv.mh[0] << std::endl
     << "mh[1] = " << inv.mh[1] << std::endl
     << "mh[2] = " << inv.mh[2] << std::endl
     << "mHc = " << inv.mHc << std::endl
     << "Z2 = " << inv.Z2 << std::endl
     << "Z3 = " << inv.Z3 << std::endl
     << "Z7inv =" << inv.Z7inv << std::endl
     << "s12 = " << inv.s12 << std::endl
     << "c13 = " << inv.c13 << std::endl
     << "tanb = " << tan(inv.beta) << std::endl;
  return of;
}

std::ostream &operator<<(std::ostream &oS, const Base_hybrid &hyb)
{
  oS << "Hybrid basis:\n"
     << "mh = " << hyb.mh << std::endl
     << "mH = " << hyb.mH << std::endl
     << "Z4 = " << hyb.Z4 << std::endl
     << "Z5 = " << hyb.Z5 << std::endl
     << "Z7 =" << hyb.Z7 << std::endl
     << "cba = " << hyb.cba << std::endl
     << "(sba = " << std::sqrt(std::abs(1. - hyb.cba * hyb.cba)) << ")\n"
     << "tanb = " << hyb.tanb << std::endl;
  return oS;
}

//--Base_generic----------------------------------------------------------------

void Base_generic::generate_random_softCpConserved(const gsl_rng *rng)
{
  static const double_range betaRange = {atan(1.1), atan(50.)};
  static const double_range MijRange = {5000., 1.e5};
  static const double_range lambdaRange = {-M_PI, M_PI};

  beta = betaRange.min + (betaRange.max - betaRange.min) * gsl_rng_uniform(rng);
  M12 = complex<double>(
      MijRange.min + (MijRange.max - MijRange.min) * gsl_rng_uniform(rng), 0.);
  Lambda1 = lambdaRange.min +
            (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
  Lambda2 = lambdaRange.min +
            (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
  Lambda3 = lambdaRange.min +
            (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
  Lambda4 = lambdaRange.min +
            (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
  Lambda5 =
      complex<double>(lambdaRange.min + (lambdaRange.max - lambdaRange.min) *
                                            gsl_rng_uniform(rng),
                      0.);
  Lambda6 = complex<double>(0., 0.);
  Lambda7 = complex<double>(0., 0.);
}

Base_compact Base_generic::convert_to_compact() const
{
  Base_compact compact;
  compact.beta = beta;
  compact.xi = xi;

  compact.Y[0][0] = complex<double>(M112, 0.);
  compact.Y[0][1] = -M12;
  compact.Y[1][0] = -std::conj(M12);
  compact.Y[1][1] = complex<double>(M222, 0.);

  compact.Z[0][0][0][0] = complex<double>(Lambda1, 0.);
  compact.Z[0][0][0][1] = Lambda6;
  compact.Z[0][0][1][0] = std::conj(Lambda6);
  compact.Z[0][1][0][0] = Lambda6;
  compact.Z[1][0][0][0] = std::conj(Lambda6);
  compact.Z[1][0][0][1] = complex<double>(Lambda4, 0.);
  compact.Z[1][0][1][0] = std::conj(Lambda5);
  compact.Z[1][1][0][0] = complex<double>(Lambda3, 0.);
  compact.Z[0][1][1][0] = complex<double>(Lambda4, 0.);
  compact.Z[0][0][1][1] = complex<double>(Lambda3, 0.);
  compact.Z[0][1][0][1] = Lambda5;
  compact.Z[0][1][1][1] = Lambda7;
  compact.Z[1][0][1][1] = std::conj(Lambda7);
  compact.Z[1][1][0][1] = Lambda7;
  compact.Z[1][1][1][0] = std::conj(Lambda7);
  compact.Z[1][1][1][1] = complex<double>(Lambda2, 0.);

  return compact;
}

Base_higgs Base_generic::convert_to_higgs() const
{
  Base_higgs higgs;
  higgs.beta = beta;
  higgs.xi = xi;

  // Translation from Higgs basis to generic basis. Eqs(A13-A23) in
  // PhysRevD.72.035004.
  double Lambda345 =
      Lambda3 + Lambda4 + real(Lambda5 * std::polar(1., 2. * (xi)));
  complex<double> I = complex<double>(0., 1.);
  complex<double> expIxi = std::polar(1., xi);
  double cb = cos(beta);
  double sb = std::sqrt(std::abs(1. - cb * cb));

  higgs.Y1 =
      M112 * cb * cb + M222 * sb * sb - real(M12 * expIxi) * sin(2. * (beta));

  higgs.Y2 =
      M112 * sb * sb + M222 * cb * cb + real(M12 * expIxi) * sin(2. * (beta));

  higgs.Y3 = -std::polar(1., -xi) *
             (0.5 * (M112 - M222) * sin(2. * (beta)) +
              real(M12 * expIxi) * cos(2. * (beta)) + I * imag(M12 * expIxi));

  higgs.Z1 =
      Lambda1 * pow(cb, 4.) + Lambda2 * pow(sb, 4.) +
      0.5 * Lambda345 * pow(sin(2. * (beta)), 2.) +
      2. * sin(2. * (beta)) *
          (cb * cb * real(Lambda6 * expIxi) + sb * sb * real(Lambda7 * expIxi));

  higgs.Z2 =
      Lambda1 * pow(sb, 4.) + Lambda2 * pow(cb, 4.) +
      0.5 * Lambda345 * pow(sin(2. * (beta)), 2.) -
      2. * sin(2. * (beta)) *
          (sb * sb * real(Lambda6 * expIxi) + cb * cb * real(Lambda7 * expIxi));

  higgs.Z3 =
      0.25 * pow(sin(2. * (beta)), 2.) * (Lambda1 + Lambda2 - 2. * Lambda345) +
      Lambda3 -
      sin(2. * (beta)) * cos(2. * (beta)) * real((Lambda6 - Lambda7) * expIxi);

  higgs.Z4 =
      0.25 * pow(sin(2. * (beta)), 2.) * (Lambda1 + Lambda2 - 2. * Lambda345) +
      Lambda4 -
      sin(2. * (beta)) * cos(2. * (beta)) * real((Lambda6 - Lambda7) * expIxi);

  higgs.Z5 =
      std::polar(1., -2. * xi) *
      (0.25 * pow(sin(2. * (beta)), 2.) * (Lambda1 + Lambda2 - 2. * Lambda345) +
       real(Lambda5 * std::polar(1., 2. * xi)) +
       I * cos(2. * (beta)) * imag(Lambda5 * std::polar(1., 2. * (xi))) -
       sin(2. * (beta)) * cos(2. * (beta)) *
           real((Lambda6 - Lambda7) * expIxi) -
       I * sin(2. * (beta)) * imag((Lambda6 - Lambda7) * expIxi));

  higgs.Z6 =
      std::polar(1., -xi) * (-0.5 * sin(2. * (beta)) *
                                 (Lambda1 * cb * cb - Lambda2 * sb * sb -
                                  Lambda345 * cos(2. * (beta)) -
                                  I * imag(Lambda5 * std::polar(1., 2. * xi))) +
                             cb * cos(3. * (beta)) * real(Lambda6 * expIxi) +
                             sb * sin(3. * (beta)) * real(Lambda7 * expIxi) +
                             I * cb * cb * imag(Lambda6 * expIxi) +
                             I * sb * sb * imag(Lambda7 * expIxi));

  higgs.Z7 =
      std::polar(1., -xi) * (-0.5 * sin(2. * (beta)) *
                                 (Lambda1 * sb * sb - Lambda2 * cb * cb +
                                  Lambda345 * cos(2. * (beta)) +
                                  I * imag(Lambda5 * std::polar(1., 2. * xi))) +
                             sb * sin(3. * (beta)) * real(Lambda6 * expIxi) +
                             cb * cos(3. * (beta)) * real(Lambda7 * expIxi) +
                             I * sb * sb * imag(Lambda6 * expIxi) +
                             I * cb * cb * imag(Lambda7 * expIxi));

  return higgs;
}

Base_invariant Base_generic::convert_to_invariant(const double &v2) const
{
  Base_invariant inv = convert_to_higgs().convert_to_invariant(v2);
  return inv;
}

vector<double> Base_generic::convert_to_vector() const
{
  return vector<double>{xi,
                        beta,
                        tan(beta),
                        M112,
                        M222,
                        real(M12),
                        imag(M12),
                        Lambda1,
                        Lambda2,
                        Lambda3,
                        Lambda4,
                        real(Lambda5),
                        imag(Lambda5),
                        real(Lambda6),
                        imag(Lambda6),
                        real(Lambda7),
                        imag(Lambda7)};
}
//--Base_higgs------------------------------------------------------------------

Base_generic Base_higgs::convert_to_generic() const
{
  Base_generic gen;
  gen.xi = xi;
  gen.beta = beta;

  // Translation from Higgs basis to generic basis. Eqs(A13-A23) in
  //  PhysRevD.72.035004.
  double Z345 = Z3 + Z4 + real(Z5 * std::polar(1., 2. * (xi)));
  complex<double> I = complex<double>(0., 1.);

  gen.M112 = Y1 * cos(-beta) * cos(-beta) + Y2 * sin(-beta) * sin(-beta) +
             real(Y3 * std::polar(1., xi)) * sin(2. * (-beta));

  gen.M222 = Y1 * sin(-beta) * sin(-beta) + Y2 * cos(-beta) * cos(-beta) -
             real(Y3 * std::polar(1., xi)) * sin(2. * (-beta));

  gen.M12 =
      std::polar(1., -xi) * (0.5 * (Y1 - Y2) * sin(2. * (-beta)) -
                             real(Y3 * std::polar(1., xi)) * cos(2. * (-beta)) -
                             I * imag(Y3 * std::polar(1., xi)));

  gen.Lambda1 = Z1 * pow(cos(-beta), 4.) + Z2 * pow(sin(-beta), 4.) +
                0.5 * Z345 * pow(sin(2. * (-beta)), 2.) +
                2. * sin(2. * (-beta)) *
                    (pow(cos(-beta), 2.) * real(Z6 * std::polar(1., xi)) +
                     pow(sin(-beta), 2.) * real(Z7 * std::polar(1., xi)));

  gen.Lambda2 = Z1 * pow(sin(-beta), 4.) + Z2 * pow(cos(-beta), 4.) +
                0.5 * Z345 * pow(sin(2. * (-beta)), 2.) -
                2. * sin(2. * (-beta)) *
                    (pow(sin(-beta), 2.) * real(Z6 * std::polar(1., xi)) +
                     pow(cos(-beta), 2.) * real(Z7 * std::polar(1., xi)));

  gen.Lambda3 = 0.25 * pow(sin(2. * (-beta)), 2.) * (Z1 + Z2 - 2. * Z345) + Z3 -
                sin(2. * (-beta)) * cos(2. * (-beta)) *
                    real((Z6 - Z7) * std::polar(1., xi));

  gen.Lambda4 = 0.25 * pow(sin(2. * (-beta)), 2.) * (Z1 + Z2 - 2. * Z345) + Z4 -
                sin(2. * (-beta)) * cos(2. * (-beta)) *
                    real((Z6 - Z7) * std::polar(1., xi));

  gen.Lambda5 = std::polar(1., -2. * xi) *
                (0.25 * pow(sin(2. * (-beta)), 2.) * (Z1 + Z2 - 2. * Z345) +
                 real(Z5 * std::polar(1., 2. * xi)) +
                 I * cos(2. * (-beta)) * imag(Z5 * std::polar(1., 2. * (xi))) -
                 sin(2. * (-beta)) * cos(2. * (-beta)) *
                     real((Z6 - Z7) * std::polar(1., xi)) -
                 I * sin(2. * (-beta)) * imag((Z6 - Z7) * std::polar(1., xi)));

  gen.Lambda6 =
      std::polar(1., -xi) *
      (-0.5 * sin(2. * (-beta)) *
           (Z1 * pow(cos(-beta), 2.) - Z2 * pow(sin(-beta), 2.) -
            Z345 * cos(2. * (-beta)) - I * imag(Z5 * std::polar(1., 2. * xi))) +
       cos(-beta) * cos(3. * (-beta)) * real(Z6 * std::polar(1., xi)) +
       sin(-beta) * sin(3. * (-beta)) * real(Z7 * std::polar(1., xi)) +
       I * pow(cos(-beta), 2.) * imag(Z6 * std::polar(1., xi)) +
       I * pow(sin(-beta), 2.) * imag(Z7 * std::polar(1., xi)));

  gen.Lambda7 =
      std::polar(1., -xi) *
      (-0.5 * sin(2. * (-beta)) *
           (Z1 * pow(sin(-beta), 2.) - Z2 * pow(cos(-beta), 2.) +
            Z345 * cos(2. * (-beta)) + I * imag(Z5 * std::polar(1., 2. * xi))) +
       sin(-beta) * sin(3. * (-beta)) * real(Z6 * std::polar(1., xi)) +
       cos(-beta) * cos(3. * (-beta)) * real(Z7 * std::polar(1., xi)) +
       I * pow(sin(-beta), 2.) * imag(Z6 * std::polar(1., xi)) +
       I * pow(cos(-beta), 2.) * imag(Z7 * std::polar(1., xi)));
  return gen;
}

Base_invariant Base_higgs::convert_to_invariant(const double &v2) const
{
  Base_invariant inv;
  /* Algorithm:
      1.) Calculate ordered eigenvalues of neutral Higgs mass matrix.
      2.) Fix U(2) invariant angles that rotates to a diagonal mass matrix.
    */

  // The general case where all neutral Higgs fields mix
  // Mass matrix
  Eigen::Matrix3d mMatrix(3, 3);
  mMatrix.setZero();
  mMatrix << v2 * Z1, v2 * real(Z6), -v2 * imag(Z6), v2 * real(Z6),
      v2 * 0.5 * (Z3 + Z4 + real(Z5)) + Y2, -v2 * 0.5 * imag(Z5),
      -v2 * imag(Z6), -v2 * 0.5 * imag(Z5),
      v2 * 0.5 * (Z3 + Z4 - real(Z5)) + Y2;

  // Since the matrix is symmetric it gives real ordered eigenvalues.
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> M(mMatrix);

  for (int i = 0; i < 3; i++)
  {
    if (M.eigenvalues()[i] < 0)
    {
      cout << "[WARNING]: negative eigenvalues in mass matrix, (mh[" << i
           << "]^2 = " << M.eigenvalues()[i] << " GeV^2). \n";
    }
    else
      inv.mh[i] = sqrt(M.eigenvalues()[i]);
  }

  //--------- Calculating mixing angles --------------------------------------

  double s132 =
      ((Z1 * v2 - inv.mh[0] * inv.mh[0]) * (Z1 * v2 - inv.mh[1] * inv.mh[1]) +
       std::abs(Z6) * std::abs(Z6) * v2 * v2) /
      ((inv.mh[2] * inv.mh[2] - inv.mh[0] * inv.mh[0]) *
       (inv.mh[2] * inv.mh[2] - inv.mh[1] * inv.mh[1]));
  double s13 = 0.;

  /* s13 needs to be sizeable to not run into numerical problems.
      Therefore we check if it is, before calculating inv.c13 and inv.cPhi.
      In case it is not, we set them manually to 1. */
  if (std::abs(s132) > 1.E-10)
  {
    s13 = -sqrt(s132); // s13 is negative for ordered masses.
    inv.c13 = sqrt(1. - s13 * s13);

    /* The angle Phi in [0,pi] is calculated by first calculating
      tan2Phi and sinPhi with eq. C22 & C23 in Phys.Rev.D 74, 015018.
      With those, one can then determine the sign in the end. */
    double tan2Phi =
        imag(conj(Z5) * Z6 * Z6) /
        (real(conj(Z5) * Z6 * Z6) +
         pow(std::abs(Z6), 4) * v2 / (inv.mh[2] * inv.mh[2] - Z1 * v2));
    double sinPhi = (Z1 * v2 - inv.mh[2] * inv.mh[2]) *
                    (s13 / sqrt(1. - s13 * s13)) / (std::abs(Z6) * v2);

    if (sinPhi > 0.7071067811865475 /* =sin(pi/4) */)
    {
      if (sinPhi > 0.99999)
        inv.cPhi = 0.;
      else if (tan2Phi < 0.)
        inv.cPhi = sqrt(1. - sinPhi * sinPhi);
      else
        inv.cPhi = -sqrt(1. - sinPhi * sinPhi);
    }
    else if (std::abs(sinPhi) < 1.E-10)
      inv.cPhi = 1.;
    else
    {
      if (sinPhi < -0.99999)
        inv.cPhi = 0.;
      else if (tan2Phi < 0.)
        inv.cPhi = -sqrt(1. - sinPhi * sinPhi);
      else
        inv.cPhi = sqrt(1. - sinPhi * sinPhi);
    }
  }
  else
  {
    inv.c13 = 1.;
    inv.cPhi = 1.;
  }

  // The angle s12 is determined from eq.C25 in Phys.Rev.D 74, 015018.
  // Here, we also make sure that the angle is sizeable for numerical reasons.
  double s122 =
      ((Z1 * v2 - inv.mh[0] * inv.mh[0]) * (inv.mh[2] * inv.mh[2] - Z1 * v2) -
       std::abs(Z6) * std::abs(Z6) * v2 * v2) /
      (inv.c13 * inv.c13 * (inv.mh[1] * inv.mh[1] - inv.mh[0] * inv.mh[0]) *
       (inv.mh[2] * inv.mh[2] - inv.mh[1] * inv.mh[1]));
  if (s122 > 0.)
    inv.s12 = sign(inv.cPhi) *
              sign(inv.mh[1] * inv.mh[1] - inv.mh[0] * inv.mh[0]) * sqrt(s122);
  else if (std::abs(s122) < 1.E-10)
    inv.s12 = 0.;
  else
    cout << "[WARNING]: large negative s12^2, when calculating "
            "masses and mixings.\n";

  // Charged Higgs:
  if (Y2 + 0.5 * Z3 * v2 < 0)
  {
    cout << "[WARNING]: Encountered negative mass for charged Higgs!\n";
  }
  else
    inv.mHc = sqrt(Y2 + 0.5 * Z3 * v2);

  inv.Z2 = Z2;
  inv.Z3 = Z3;
  inv.beta = beta;
  inv.xi = xi;

  // calculating angle to get Z7inv
  double theta23 = arg(Z6) - acos(inv.cPhi);
  complex<double> detUInv = std::polar(1., -theta23);

  inv.theta23 = theta23;
  inv.Z7inv = Z7 * detUInv;

  return inv;
}

Base_compact Base_higgs::convert_to_compact() const
{
  Base_compact compact = convert_to_generic().convert_to_compact();
  return compact;
}

vector<double> Base_higgs::convert_to_vector() const
{
  return vector<double>{
      xi, beta, tan(beta), Y1, Y2, real(Y3), imag(Y3), Z1, Z2,
      Z3, Z4, real(Z5), imag(Z5), real(Z6), imag(Z6), real(Z7), imag(Z7)};
}

//--Base_invariant--------------------------------------------------------------

Base_generic Base_invariant::convert_to_generic(const double &v2) const
{
  Base_generic gen = convert_to_higgs(v2).convert_to_generic();
  return gen;
}

Base_compact Base_invariant::convert_to_compact(const double &v2) const
{
  Base_compact compact = convert_to_generic(v2).convert_to_compact();
  return compact;
}

Base_higgs Base_invariant::convert_to_higgs(const double &v2) const
{
  Base_higgs higgs;
  higgs.xi = xi;
  higgs.beta = beta;

  // The square of the sin of angles. (sign is to be determined below)
  double s122 = s12 * s12;
  double s132 = 1. - c13 * c13;

  double c12 = sqrt(1. - s122);

  higgs.Z1 = (mh[0] * mh[0] * c12 * c12 * c13 * c13 +
              mh[1] * mh[1] * s122 * c13 * c13 + mh[2] * mh[2] * s132) /
             v2;

  // Fixing the signs
  double s13 = sign(higgs.Z1 * v2 - mh[2] * mh[2]) * sqrt(s132);

  /* Setting up diagonal mass matrix and rotation matrix, eq.44, in
    Phys.Rev.D 74, 015018. */
  Eigen::Matrix3d rot, mass, massDiag;
  rot.setZero();
  massDiag.setZero();

  rot(0, 0) = c12 * c13;
  rot(0, 1) = -s12;
  rot(0, 2) = -c12 * s13;
  rot(1, 0) = c13 * s12;
  rot(1, 1) = c12;
  rot(1, 2) = -s12 * s13;
  rot(2, 0) = s13;
  rot(2, 1) = 0.;
  rot(2, 2) = c13;

  for (int i = 0; i < 3; ++i)
    massDiag(i, i) = mh[i] * mh[i];

  mass = rot.transpose() * massDiag * rot;

  // Extracting Higgs parameters
  higgs.Z2 = Z2;
  higgs.Z3 = Z3;
  higgs.Z7 = Z7inv;
  double A2 = mass(2, 2);
  higgs.Y2 = mHc * mHc - 0.5 * Z3 * v2;
  higgs.Z5 = complex<double>((mass(1, 1) - A2) / v2, -2. * mass(1, 2) / v2);
  higgs.Z4 = real(higgs.Z5) + 2. * (A2 - mHc * mHc) / v2;
  higgs.Z6 = complex<double>(mass(0, 1) / v2, -mass(0, 2) / v2);

  // theta23 is arbitrary, but specifies the phase of the Higgs basis.
  complex<double> detU = std::polar(1., theta23);
  higgs.Z5 *= detU * detU;
  higgs.Z6 *= detU;
  higgs.Z7 *= detU;

  // Tadpole equations sets
  higgs.Y1 = -0.5 * higgs.Z1 * v2;
  higgs.Y3 = -0.5 * higgs.Z6 * v2;

  return higgs;
}

vector<double> Base_invariant::convert_to_vector() const
{
  return vector<double>{xi, beta, tan(beta), cPhi, mHc,
                        mh[0], mh[1], mh[2], s12, c13,
                        Z2, Z3, theta23, real(Z7inv), imag(Z7inv)};
}

//--Base_hybrid-----------------------------------------------------------------

void Base_hybrid::generate_random_softCpConserved(const gsl_rng *rng)
{
  // Ranges for the parameters:
  static const double_range betaRange = {atan(1.1), atan(50.)};
  static Range_hybrid hybRange;
  hybRange.name = "standard_range";

  hybRange.mh = {90., 130.};
  hybRange.mH = {150., 1000.};
  hybRange.cba = {-0.5, 0.5};
  hybRange.tanb = {1.1, 50.};
  hybRange.Z4 = {-M_PI, M_PI};
  hybRange.Z5 = {-M_PI, M_PI};
  hybRange.Z7 = {-M_PI, M_PI};
  // Random free parameters
  mh = hybRange.mh.min +
       (hybRange.mh.max - hybRange.mh.min) * gsl_rng_uniform(rng);
  mH = hybRange.mH.min +
       (hybRange.mH.max - hybRange.mH.min) * gsl_rng_uniform(rng);
  cba = hybRange.cba.min +
        (hybRange.cba.max - hybRange.cba.min) * gsl_rng_uniform(rng);
  double beta =
      betaRange.min + (betaRange.max - betaRange.min) * gsl_rng_uniform(rng);
  tanb = tan(beta);
  Z4 = hybRange.Z4.min +
       (hybRange.Z4.max - hybRange.Z4.min) * gsl_rng_uniform(rng);
  Z5 = hybRange.Z5.min +
       (hybRange.Z5.max - hybRange.Z5.min) * gsl_rng_uniform(rng);
  Z7 = hybRange.Z7.min +
       (hybRange.Z7.max - hybRange.Z7.min) * gsl_rng_uniform(rng);
}

Base_generic Base_hybrid::convert_to_generic(const double &v2) const
{
  Base_generic gen;

  if (v2 < 0.1)
  {
    cout << "[ERROR]: Invalid v2 given to convert_to_generic().\n";
    return gen;
  }

  gen = (convert_to_higgs(v2)).convert_to_generic();

  gen.M12 = complex<double>(real(gen.M12), 0.);
  gen.Lambda5 = complex<double>(real(gen.Lambda5), 0.);
  gen.Lambda6 = complex<double>(0., 0.);
  gen.Lambda7 = complex<double>(0., 0.);

  return gen;
}

Base_higgs Base_hybrid::convert_to_higgs(const double &v2) const
{
  Base_higgs higgs;

  // Beware that some parts of 2HDMC uses different convention where sba is in
  // range -1 to 1. Here, sba is in 0 to 1 and cba is -1 to 1.
  double sba = sqrt(1. - cba * cba);

  static const double VSMALL = 1E-10;

  if ((mh < 0) || (std::abs(cba) > 1) || (std::abs(mh - mH) < VSMALL) ||
      (std::abs(tanb - 1.) < VSMALL))
  {
    cout << "[ERROR]: Can't convert hybrid basis to Higgs basis\n";
    return higgs;
  }

  if (v2 < 0.1)
  {
    cout << "[ERROR]: Invalid v2 given to convert_to_higgs().\n";
    return higgs;
  }
  higgs.beta = atan(tanb);
  double tan2b = tan(2. * higgs.beta);
  double cot2b = 1. / tan2b;

  higgs.Z4 = Z4;
  higgs.Z5 = complex<double>(Z5, 0.);
  higgs.Z7 = complex<double>(Z7, 0.);

  // Calculating Higgs basis
  higgs.Z1 = (sba * sba * mh * mh + cba * cba * mH * mH) / v2;
  higgs.Z2 =
      higgs.Z1 + 2. * cot2b * ((mh * mh - mH * mH) * sba * cba / v2 + Z7);
  higgs.Z3 = higgs.Z1 +
             (2. * cot2b - tan2b) * ((mh * mh - mH * mH) * sba * cba / v2) +
             Z7 * tan2b - Z4 - Z5;
  higgs.Z6 = complex<double>((mh * mh - mH * mH) * cba * sba / v2, 0.);

  higgs.Y2 = mH * mH * sba * sba + mh * mh * cba * cba +
             0.5 * tan2b * (real(higgs.Z6) - Z7) * v2 - 0.5 * higgs.Z1 * v2 -
             real(higgs.Z6) * v2 * cot2b;

  // Using tree-lvl tadpole equations to set Y1 and Y3
  higgs.Y1 = -0.5 * higgs.Z1 * v2;
  higgs.Y3 = -0.5 * higgs.Z6 * v2;

  return higgs;
}

Base_invariant Base_hybrid::convert_to_invariant(const double &v2) const
{
  return convert_to_higgs(v2).convert_to_invariant(v2);
}

std::vector<double> Base_hybrid::convert_to_vector() const
{
  return std::vector<double> {xi, beta, tanb, mh, mH, cba, Z4, Z5, Z7};
}

} // namespace THDME