// THDM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: General (complex) 2HDM class
 * @author: Joel Oredsson
 *============================================================================*/
#include "THDM.h"
#include "HelpFunctions.h"
#include "RGE.h"

#ifdef SPHENO
#include "SPheno.h"
#endif

#ifdef GNUPLOT
#include "GnuPlotSystem.h"
#endif

#include <Eigen/Dense>
#include <algorithm>
#include <complex>
#include <cstdlib>
#include <gsl/gsl_odeiv2.h>
#include <iostream>
#include <string>

#include <cmath>
#include <cstdio>
#include <unistd.h>

using namespace std;

namespace THDME
{

/*----------------------------------------------------------------------------*/

THDM::THDM() : _z2_symmetry(NO_SYMMETRY) { init(); }

THDM::THDM(const SM &sm) : _z2_symmetry(NO_SYMMETRY)
{
  init();
  set_sm(sm);
}

// Copy-constructor
THDM::THDM(const THDM &thdm) { set_from_thdm(thdm); }

void THDM::set_from_thdm(const THDM &thdm)
{
  set_logLevel(thdm.get_logLevel());
  init();

  _renormScale = thdm.get_renormalization_scale();
#ifdef SPHENO
  _spheno.set(thdm.get_spheno_output());
#endif
  set_rgeConfig(thdm.get_rgeConfig());

  double y[_numParams];
  thdm.set_y(y);
  set_model_from_y(y);

  calc_kF_rF_ckm_from_etaF();
  _z2_symmetry = thdm.get_yukawa_type();

  calc_treeLvl_masses_and_mixings();
}

THDM::~THDM()
{
  // std::cout << "Destroying thdm!\n"; // Debugging
}

bool THDM::set_param_gen(const Base_generic &in, const bool enforceTadpole)
{
  _base_generic = in;

  if (enforceTadpole)
    if (!fix_treeLvl_tadpole_eqs())
      return false;

  return calc_treeLvl_masses_and_mixings();
}

Base_generic THDM::get_param_gen() const { return _base_generic; }

bool THDM::set_param_higgs(const Base_higgs &in, const bool enforceTadpole)
{
  return set_param_gen(in.convert_to_generic(),enforceTadpole);
}

Base_higgs THDM::get_param_higgs() const
{
  return _base_generic.convert_to_higgs();
}

// CP invariant and Z2 symmetric basis (all parameters are real)
bool THDM::set_param_hybrid(const Base_hybrid &hyb, const bool enforceTadpole)
{
  return set_param_gen(hyb.convert_to_generic(_v2),enforceTadpole);
}

bool THDM::set_param_invariant(const Base_invariant &inv, const bool enforceTadpole)
{

  return set_param_gen(inv.convert_to_generic(_v2),enforceTadpole);
}

Base_invariant THDM::get_param_invariant() const
{
  return _base_generic.convert_to_invariant(_v2);
}

Base_hybrid THDM::get_param_hybrid() const
{
  // This function requires that the THDM is CP conserving and Z2 symmetric.
  if (!is_cp_conserved())
    _console.error
        << "[ERROR]: Can't retrieve hybrid basis from CP violating THDM.\n";

  Base_higgs higgs = _base_generic.convert_to_higgs();

  double mA = std::sqrt(_mHc * _mHc + 0.5 * (higgs.Z4 - real(higgs.Z5)) * _v2);

  double mh, mH, cba;
  if (std::abs(higgs.Z6) > 1e-10)
  {
    mh = std::sqrt(
        0.5 * (mA * mA + (higgs.Z1 + real(higgs.Z5)) * _v2 -
               std::sqrt((mA * mA + (real(higgs.Z5) - higgs.Z1) * _v2) *
                             (mA * mA + (real(higgs.Z5) - higgs.Z1) * _v2) +
                         4. * real(higgs.Z6) * real(higgs.Z6) * _v2 * _v2)));

    mH = std::sqrt(
        0.5 * (mA * mA + (higgs.Z1 + real(higgs.Z5)) * _v2 +
               std::sqrt((mA * mA + (real(higgs.Z5) - higgs.Z1) * _v2) *
                             (mA * mA + (real(higgs.Z5) - higgs.Z1) * _v2) +
                         4. * real(higgs.Z6) * real(higgs.Z6) * _v2 * _v2)));

    cba = -real(higgs.Z6) * _v2 /
          std::sqrt((mH * mH - mh * mh) * (mH * mH - higgs.Z1 * _v2));
  }
  else
  {
    double m1 = std::sqrt(higgs.Z1 * _v2);
    double m2 = std::sqrt(mA * mA + real(higgs.Z5) * _v2);

    if (m1 < m2)
    {
      mh = m1;
      mH = m2;
      cba = 0.;
    }
    else
    {
      mh = m2;
      mH = m1;
      cba = 1.;
    }
  }

  Base_hybrid hyb;
  hyb.mh = mh;
  hyb.mH = mH;
  hyb.cba = cba;
  hyb.tanb = tan(higgs.beta);
  hyb.Z4 = higgs.Z4;
  hyb.Z5 = real(higgs.Z5);
  hyb.Z7 = real(higgs.Z7);

  return hyb;
}

bool THDM::fix_treeLvl_tadpole_eqs()
{
  double sinb = sin(_base_generic.beta);
  double cosb = sqrt(1.0 - sinb * sinb);
  double tanb = tan(_base_generic.beta);
  double cotb = 1. / tanb;

  // Tree-lvl minimum from eq.(A4-A5) and eq.(A7) in PhysRevD.72.035004
  if (abs(_base_generic.beta) > _MACHINE_PRECISION)
  {
    if (tanb < 1.E-3)
      _console.warning
          << "Very low tanb!"
          << "This results in very large masses from tadpole requirements.\n";

    // We solve eq.(A7) in PhysRevD.72.035004 numerically
    // M12 must be non-zero, otherwise xi is set to zero
    if (std::abs(_base_generic.M12) > 1e-6)
    {
      // Using Newton's method to find xi that solves f = 0
      // OBS: This can in principal fail.
      auto f = [&, this](double x) {
        return std::imag(_base_generic.M12 * std::polar(1., x)) -
               0.5 * _v2 *
                   (std::imag(_base_generic.Lambda5 * std::polar(1., 2 * x)) *
                        sinb * cosb +
                    std::imag(_base_generic.Lambda6 * std::polar(1., x)) *
                        cosb * cosb +
                    std::imag(_base_generic.Lambda7 * std::polar(1., x)) *
                        sinb * sinb);
      };
      std::complex<double> I(0., 1.);

      auto dfdx = [&, this](double x) {
        return std::imag(I * _base_generic.M12 * std::polar(1., x)) -
               0.5 * _v2 *
                   (std::imag(2. * I * _base_generic.Lambda5 *
                              std::polar(1., 2 * x)) *
                        sinb * cosb +
                    std::imag(I * _base_generic.Lambda6 * std::polar(1., x)) *
                        cosb * cosb +
                    std::imag(I * _base_generic.Lambda7 * std::polar(1., x)) *
                        sinb * sinb);
      };

      static double XI_PRECISION = 1e-10;
      int nrTries = 0;
      int firstGuesses = 1;
      static int maxGuesses = (int)(20. * M_PI);
      double xi = 0; // Initial guess, this is incremented if it fails
      while (std::abs(f(xi)) > XI_PRECISION)
      {
        if (++nrTries > 20 || std::abs(dfdx(xi)) < 1e-16)
        {
          // Failed to find xi such that f=0.
          // Now tries new initial guess of xi.
          xi = 0.1 * firstGuesses;
          firstGuesses++;
          nrTries = 0; // Reseting the nr of iterations
        }

        xi = xi - f(xi) / dfdx(xi); // Newton's method
        xi = fmod(xi, 2. * M_PI);

        if (firstGuesses > maxGuesses)
        {
          _console.error << "[ERROR]: Couldn't find good xi value when setting "
                            "tadpole eqs.\n";
          return false;
        }
      }

      _base_generic.xi = xi;
    }
    else
      _base_generic.xi = 0.;

    _base_generic.M112 = real(
        _base_generic.M12 * std::polar(1., _base_generic.xi) * tanb -
        0.5 * _v2 *
            (_base_generic.Lambda1 * cosb * cosb +
             (_base_generic.Lambda3 + _base_generic.Lambda4 +
              _base_generic.Lambda5 * std::polar(1., 2. * _base_generic.xi)) *
                 sinb * sinb +
             (2. * _base_generic.Lambda6 * std::polar(1., _base_generic.xi) +
              conj(_base_generic.Lambda6) * std::polar(1., -_base_generic.xi)) *
                 sinb * cosb +
             _base_generic.Lambda7 * sinb * sinb * tanb *
                 std::polar(1., _base_generic.xi)));

    _base_generic.M222 =
        real(conj(_base_generic.M12 * std::polar(1., _base_generic.xi)) * cotb -
             0.5 * _v2 *
                 (_base_generic.Lambda2 * sinb * sinb +
                  (_base_generic.Lambda3 + _base_generic.Lambda4 +
                   conj(_base_generic.Lambda5) *
                       std::polar(1., -2. * _base_generic.xi)) *
                      cosb * cosb +
                  (_base_generic.Lambda7 * std::polar(1., _base_generic.xi) +
                   2. * conj(_base_generic.Lambda7) *
                       std::polar(1., -_base_generic.xi)) *
                      sinb * cosb +
                  conj(_base_generic.Lambda6) * cosb * cosb *
                      std::polar(1., -_base_generic.xi) * cotb));
  }
  else
  {
    _console.warning
        << "[WARNING]: using Higgs basis tadpole equation because beta=0!\n";

    _base_generic.M112 = -0.5 * _base_generic.Lambda1 * _v2;
    _base_generic.M12 = -0.5 * _base_generic.Lambda6 * _v2;
  }

  return true;
}

void THDM::set_tanb(const double &tanb) { _base_generic.beta = atan(tanb); }

void THDM::chi_phase_shift(const double &chi)
{
  // chi is the phase of the determinant of a U(2) Higgs flavor transformation
  // in the generic basis: det(U) = exp( i chi)
  complex<double> detU = std::polar(1., chi);
  complex<double> detUInv = std::polar(1., -chi);

  Base_higgs higgs = _base_generic.convert_to_higgs();

  higgs.Y3 *= detUInv;
  higgs.Z6 *= detUInv;
  higgs.Z7 *= detUInv;
  higgs.Z5 *= detUInv * detUInv;

  set_param_higgs(higgs);

  _rU *= detU;
  _rD *= detU;
  _rL *= detU;

  calc_eta_from_kF_rF_ckm();
}

bool THDM::rge_update(const double y[], const double ti)
{
  // Creates data files with the parameters as function of energy.
  if (!_rgeResults.evolved)
  {
    if (_rgeConfig.dataOutput)
    {
      create_data_files();
    }
    _rgeResults.evolved = true;
  }

  // Updates the class member parameters
  _rgeResults.ef = exp(ti);
  _renormScale = _rgeResults.ef;
  set_model_from_y(y);

  calc_treeLvl_masses_and_mixings();
  calc_kF_rF_ckm_from_etaF();

  if (_rgeConfig.dataOutput)
    write_to_data_files(ti);

  // Check the Higgs masses for NaN values as well.
  for (int i = 0; i < 3; i++)
  {
    if (std::isnan(_mh[i]))
    {
      _console.warning << "[WARNING]: NaN value detected in mh[" << i
                       << "] = " << _mh[i] << " at " << exp(ti) << " GeV !!!\n";
      return false;
    }
  }

  return true;
}

void THDM::rge_finished()
{

  if (_rgeConfig.dataOutput)
  {
    _files.close_streams();

#ifdef GNUPLOT
    _console.info << "Running GNUPLOT!\n";
    GnuPlotSystem gnuSys(_files.get_directory());
    gnuSys.set_z2_symmetry(_z2_symmetry);
    gnuSys.create_pdf_plots(_rgeConfig, _rgeResults);
#endif
  }
}

void THDM::set_y(double y[]) const
{
  y[0] = _g1;
  y[1] = _g2;
  y[2] = _g3;
  y[3] = cos(_base_generic.beta) * sqrt(_v2);
  y[4] = 0.;
  y[5] = real(std::polar(1., _base_generic.xi) * sin(_base_generic.beta) *
              sqrt(_v2));
  y[6] = imag(std::polar(1., _base_generic.xi) * sin(_base_generic.beta) *
              sqrt(_v2));
  y[7] = tan(_base_generic.beta);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      y[8 + i * 3 + j] = real(_eta1U(i, j));
      y[17 + i * 3 + j] = imag(_eta1U(i, j));
      y[26 + i * 3 + j] = real(_eta1D(i, j));
      y[35 + i * 3 + j] = imag(_eta1D(i, j));
      y[44 + i * 3 + j] = real(_eta1L(i, j));
      y[53 + i * 3 + j] = imag(_eta1L(i, j));
      y[62 + i * 3 + j] = real(_eta2U(i, j));
      y[71 + i * 3 + j] = imag(_eta2U(i, j));
      y[80 + i * 3 + j] = real(_eta2D(i, j));
      y[89 + i * 3 + j] = imag(_eta2D(i, j));
      y[98 + i * 3 + j] = real(_eta2L(i, j));
      y[107 + i * 3 + j] = imag(_eta2L(i, j));
    }

  y[116] = _base_generic.M112;
  y[117] = _base_generic.M222;
  y[118] = real(_base_generic.M12);
  y[119] = imag(_base_generic.M12);
  y[120] = _base_generic.Lambda1;
  y[121] = _base_generic.Lambda2;
  y[122] = _base_generic.Lambda3;
  y[123] = _base_generic.Lambda4;
  y[124] = real(_base_generic.Lambda5);
  y[125] = imag(_base_generic.Lambda5);
  y[126] = real(_base_generic.Lambda6);
  y[127] = imag(_base_generic.Lambda6);
  y[128] = real(_base_generic.Lambda7);
  y[129] = imag(_base_generic.Lambda7);
}

void THDM::set_model_from_y(const double y[])
{
  _g1 = y[0];
  _g2 = y[1];
  _g3 = y[2];

  complex<double> v1C = complex<double>(y[3], y[4]);
  complex<double> v2C = complex<double>(y[5], y[6]);
  _v2 = norm(v1C) + norm(v2C);
  _base_generic.beta = atan(std::abs(v2C) / std::abs(v1C));
  _base_generic.xi = arg(v2C);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
    {
      _eta1U(i, j) = complex<double>(y[8 + i * 3 + j], y[17 + i * 3 + j]);
      _eta1D(i, j) = complex<double>(y[26 + i * 3 + j], y[35 + i * 3 + j]);
      _eta1L(i, j) = complex<double>(y[44 + i * 3 + j], y[53 + i * 3 + j]);
      _eta2U(i, j) = complex<double>(y[62 + i * 3 + j], y[71 + i * 3 + j]);
      _eta2D(i, j) = complex<double>(y[80 + i * 3 + j], y[89 + i * 3 + j]);
      _eta2L(i, j) = complex<double>(y[98 + i * 3 + j], y[107 + i * 3 + j]);
    };

  _base_generic.M112 = y[116];
  _base_generic.M222 = y[117];
  _base_generic.M12 = complex<double>(y[118], y[119]);
  _base_generic.Lambda1 = y[120];
  _base_generic.Lambda2 = y[121];
  _base_generic.Lambda3 = y[122];
  _base_generic.Lambda4 = y[123];
  _base_generic.Lambda5 = complex<double>(y[124], y[125]);
  _base_generic.Lambda6 = complex<double>(y[126], y[127]);
  _base_generic.Lambda7 = complex<double>(y[128], y[129]);
}

void THDM::set_yukawa_type(const Z2symmetry &z2)
{
  _z2_symmetry = z2;

  // Sets the kappa matrices to be diagonal proportional to the fermion masses.
  set_kF_from_fermion_masses();

  // Can't set Z2 symmetry if beta = 0.
  if (z2 != NO_SYMMETRY)
  {
    if (_base_generic.beta < _MACHINE_PRECISION)
    {
      _console.error << "[ERROR]: Can't set Yukawa symmetry because tanb is "
                        "too small; beta = "
                     << _base_generic.beta << std::endl;
      _z2_symmetry = NO_SYMMETRY;
    }
  }

  switch (_z2_symmetry)
  {
  case NO_SYMMETRY:
    _console.debug
        << "[OBS]: No Z_2 symmetry chosen, all rho matrices are zero!\n";
    break;
  case TYPE_I:
    set_yukawa_rho(_kU / tan(_base_generic.beta), _kD / tan(_base_generic.beta),
                   _kL / tan(_base_generic.beta));
    calc_eta_from_kF_rF_ckm();
    _eta1U.setZero();
    _eta1D.setZero();
    _eta1L.setZero();
    break;
  case TYPE_II:
    set_yukawa_rho(_kU / tan(_base_generic.beta),
                   -_kD * tan(_base_generic.beta),
                   -_kL * tan(_base_generic.beta));
    calc_eta_from_kF_rF_ckm();
    _eta1U.setZero();
    _eta2D.setZero();
    _eta2L.setZero();
    break;
  case TYPE_III:
    set_yukawa_rho(_kU / tan(_base_generic.beta),
                   -_kD * tan(_base_generic.beta),
                   _kL / tan(_base_generic.beta));
    calc_eta_from_kF_rF_ckm();
    _eta1U.setZero();
    _eta2D.setZero();
    _eta1L.setZero();
    break;
  case TYPE_IV:
    set_yukawa_rho(_kU / tan(_base_generic.beta), _kD / tan(_base_generic.beta),
                   -_kL * tan(_base_generic.beta));
    calc_eta_from_kF_rF_ckm();
    _eta1U.setZero();
    _eta1D.setZero();
    _eta2L.setZero();
    break;
  }
}

Z2symmetry THDM::get_yukawa_type() const { return _z2_symmetry; }

void THDM::set_yukawa_aligned(const std::complex<double> &aU,
                              const std::complex<double> &aD,
                              const std::complex<double> &aL)
{
  _z2_symmetry = NO_SYMMETRY;

  // Sets the kappa matrices to be diagonal proportional to the fermion masses.
  set_kF_from_fermion_masses();

  set_yukawa_rho(aU * _kU, aD * _kD, aL * _kL);

  calc_eta_from_kF_rF_ckm();
}

vector<complex<double>> THDM::get_aF() const
{
  // aFVec = {aU, aD, aL}
  vector<complex<double>> aFVec = {_rU(2, 2) / _kU(2, 2), _rD(2, 2) / _kD(2, 2),
                                   _rL(2, 2) / _kL(2, 2)};

  return aFVec;
}

void THDM::set_yukawa_rho(const Eigen::Matrix3cd &rU,
                          const Eigen::Matrix3cd &rD,
                          const Eigen::Matrix3cd &rL)
{
  _rU = rU;
  _rD = rD;
  _rL = rL;
}

void THDM::set_yukawa_manual(const Eigen::Matrix3cd &rU,
                             const Eigen::Matrix3cd &rD,
                             const Eigen::Matrix3cd &rL)
{
  if (_z2_symmetry != NO_SYMMETRY)
  {
    _console.warning << "[WARNING]: Overwriting Z_2 symmetry.\n";
    _z2_symmetry = NO_SYMMETRY;
  }

  set_kF_from_fermion_masses();

  _rU = rU;
  _rD = rD;
  _rL = rL;

  calc_eta_from_kF_rF_ckm();
}

void THDM::set_yukawa_eta(const Eigen::Matrix3cd &eta1U,
                          const Eigen::Matrix3cd &eta1D,
                          const Eigen::Matrix3cd &eta1L,
                          const Eigen::Matrix3cd &eta2U,
                          const Eigen::Matrix3cd &eta2D,
                          const Eigen::Matrix3cd &eta2L)
{
  if (_z2_symmetry != NO_SYMMETRY)
  {
    _console.warning << "[WARNING]: Overwriting Z_2 symmetry.\n";
    set_yukawa_type(NO_SYMMETRY);
  }

  _eta1U = eta1U;
  _eta1D = eta1D;
  _eta1L = eta1L;
  _eta2U = eta2U;
  _eta2D = eta2D;
  _eta2L = eta2L;

  calc_kF_rF_ckm_from_etaF();
}

bool THDM::is_perturbative() const
{
  double maxPert = 4. * M_PI;
  if (_base_generic.Lambda1 > maxPert || _base_generic.Lambda2 > maxPert ||
      _base_generic.Lambda3 > maxPert || _base_generic.Lambda4 > maxPert ||
      std::abs(_base_generic.Lambda5) > maxPert ||
      std::abs(_base_generic.Lambda6) > maxPert ||
      std::abs(_base_generic.Lambda7) > maxPert)
  {
    return false;
  }
  return true;
}

bool THDM::is_unitary() const
{
  double unitarityBound = (8. * M_PI);
  bool unitary = true;

  Eigen::Matrix3cd S21(3, 3);
  double S20;
  Eigen::Matrix4cd S01(4, 4);
  Eigen::Matrix4cd S00(4, 4);
  S21.setZero();
  S01.setZero();
  S00.setZero();
  double s2 = sqrt(2.);

  // These are eqs.(7a-d) in hep-ph/0508020v1
  S21(0, 0) = complex<double>(_base_generic.Lambda1, 0.);
  S21(0, 1) = _base_generic.Lambda5;
  S21(0, 2) = s2 * _base_generic.Lambda6;
  S21(1, 0) = conj(_base_generic.Lambda5);
  S21(1, 1) = complex<double>(_base_generic.Lambda2, 0.);
  S21(1, 2) = s2 * conj(_base_generic.Lambda7);
  S21(2, 0) = s2 * conj(_base_generic.Lambda6);
  S21(2, 1) = s2 * _base_generic.Lambda7;
  S21(2, 2) =
      complex<double>(_base_generic.Lambda3 + _base_generic.Lambda4, 0.);

  S20 = _base_generic.Lambda3 - _base_generic.Lambda4;

  S01(0, 0) = complex<double>(_base_generic.Lambda1, 0.);
  S01(0, 1) = complex<double>(_base_generic.Lambda4, 0.);
  S01(0, 2) = _base_generic.Lambda6;
  S01(0, 3) = conj(_base_generic.Lambda6);
  S01(1, 0) = complex<double>(_base_generic.Lambda4, 0.);
  S01(1, 1) = complex<double>(_base_generic.Lambda2, 0.);
  S01(1, 2) = _base_generic.Lambda7;
  S01(1, 3) = conj(_base_generic.Lambda7);
  S01(2, 0) = conj(_base_generic.Lambda6);
  S01(2, 1) = conj(_base_generic.Lambda7);
  S01(2, 2) = complex<double>(_base_generic.Lambda3, 0.);
  S01(2, 3) = conj(_base_generic.Lambda5);
  S01(3, 0) = _base_generic.Lambda6;
  S01(3, 1) = _base_generic.Lambda7;
  S01(3, 2) = _base_generic.Lambda5;
  S01(3, 3) = complex<double>(_base_generic.Lambda3, 0.);

  S00(0, 0) = complex<double>(3. * _base_generic.Lambda1, 0.);
  S00(0, 1) =
      complex<double>(2. * _base_generic.Lambda3 + _base_generic.Lambda4, 0.);
  S00(0, 2) = 3. * _base_generic.Lambda6;
  S00(0, 3) = 3. * conj(_base_generic.Lambda6);
  S00(1, 0) =
      complex<double>(2. * _base_generic.Lambda3 + _base_generic.Lambda4, 0.);
  S00(1, 1) = complex<double>(3. * _base_generic.Lambda2, 0.);
  S00(1, 2) = 3. * _base_generic.Lambda7;
  S00(1, 3) = 3. * conj(_base_generic.Lambda7);
  S00(2, 0) = 3. * conj(_base_generic.Lambda6);
  S00(2, 1) = 3. * conj(_base_generic.Lambda7);
  S00(2, 2) =
      complex<double>(_base_generic.Lambda3 + 2. * _base_generic.Lambda4, 0.);
  S00(2, 3) = 3. * conj(_base_generic.Lambda5);
  S00(3, 0) = 3. * _base_generic.Lambda6;
  S00(3, 1) = 3. * _base_generic.Lambda7;
  S00(3, 2) = 3. * _base_generic.Lambda5;
  S00(3, 3) =
      complex<double>(_base_generic.Lambda3 + 2. * _base_generic.Lambda4, 0.);

  Eigen::ComplexEigenSolver<Eigen::Matrix3cd> eigenS21;
  eigenS21.compute(S21);
  Eigen::ComplexEigenSolver<Eigen::Matrix4cd> eigenS01;
  eigenS01.compute(S01);
  Eigen::ComplexEigenSolver<Eigen::Matrix4cd> eigenS00;
  eigenS00.compute(S00);

  if (std::abs(S20) > unitarityBound)
    unitary = false;
  for (int i = 0; i < 3; i++)
  {
    if (std::abs(eigenS21.eigenvalues()[i]) > unitarityBound)
      unitary = false;
  }
  for (int i = 0; i < 4; i++)
  {
    if (std::abs(eigenS01.eigenvalues()[i]) > unitarityBound)
      unitary = false;
    if (std::abs(eigenS00.eigenvalues()[i]) > unitarityBound)
      unitary = false;
  }
  return unitary;
}

bool THDM::is_stable_z2sym() const
{
  if (_base_generic.Lambda1 < 0. || _base_generic.Lambda2 < 0. ||
      _base_generic.Lambda3 <
          -sqrt(std::abs(_base_generic.Lambda1 * _base_generic.Lambda2)) ||
      _base_generic.Lambda3 + _base_generic.Lambda4 -
              std::abs(_base_generic.Lambda5) <
          -sqrt(std::abs(_base_generic.Lambda1 * _base_generic.Lambda2)))
  {
    _console.warning
        << "[WARNING]: Z2 symmetric stability conditions violated!\n";
    return false;
  }
  return true;
}
bool THDM::is_stable() const
{
  // return is_stable_z2sym(); // DEBUG!

  Eigen::Matrix4cd LE(4, 4);
  Eigen::Matrix4cd mink(4, 4);
  LE.setZero();
  mink.setZero();

  for (int i = 0; i < 4; i++)
  {
    mink(i, i) = complex<double>(-1., 0.);
  }
  mink(0, 0) += complex<double>(2., 0.);

  LE(0, 0) = complex<double>(
      0.5 * ((_base_generic.Lambda1 + _base_generic.Lambda2) * 0.5 +
             _base_generic.Lambda3),
      0.);
  LE(0, 1) = complex<double>(0.5 * real(_base_generic.Lambda6) +
                                 0.5 * real(_base_generic.Lambda7),
                             0.);
  LE(0, 2) = complex<double>(-0.5 * imag(_base_generic.Lambda6) -
                                 0.5 * imag(_base_generic.Lambda7),
                             0.);
  LE(0, 3) = complex<double>(
      0.25 * (_base_generic.Lambda1 - _base_generic.Lambda2), 0.);
  LE(1, 0) = complex<double>(-0.5 * real(_base_generic.Lambda6) -
                                 0.5 * real(_base_generic.Lambda7),
                             0.);
  LE(1, 1) = complex<double>(
      -0.5 * _base_generic.Lambda4 - 0.5 * real(_base_generic.Lambda5), 0.);
  LE(1, 2) = complex<double>(0.5 * imag(_base_generic.Lambda5), 0.);
  LE(1, 3) = complex<double>(-0.5 * real(_base_generic.Lambda6) +
                                 0.5 * real(_base_generic.Lambda7),
                             0.);
  LE(2, 0) = complex<double>(0.5 * imag(_base_generic.Lambda6) +
                                 0.5 * imag(_base_generic.Lambda7),
                             0.);
  LE(2, 1) = complex<double>(0.5 * imag(_base_generic.Lambda5), 0.);
  LE(2, 2) = complex<double>(
      -0.5 * _base_generic.Lambda4 + 0.5 * real(_base_generic.Lambda5), 0.);
  LE(2, 3) = complex<double>(0.5 * imag(_base_generic.Lambda6) -
                                 0.5 * imag(_base_generic.Lambda7),
                             0.);
  LE(3, 0) = complex<double>(
      -0.25 * (_base_generic.Lambda1 - _base_generic.Lambda2), 0.);
  LE(3, 1) = complex<double>(-0.5 * real(_base_generic.Lambda6) +
                                 0.5 * real(_base_generic.Lambda7),
                             0.);
  LE(3, 2) = complex<double>(0.5 * imag(_base_generic.Lambda6) -
                                 0.5 * imag(_base_generic.Lambda7),
                             0.);
  LE(3, 3) = complex<double>(
      0.5 * ((-_base_generic.Lambda1 - _base_generic.Lambda2) * 0.5 +
             _base_generic.Lambda3),
      0.);

  Eigen::ComplexEigenSolver<Eigen::Matrix4cd> eigenLE;
  eigenLE.compute(LE);
  complex<double> eigenValues[4];
  eigenValues[0] = eigenLE.eigenvalues()[0];
  eigenValues[1] = eigenLE.eigenvalues()[1];
  eigenValues[2] = eigenLE.eigenvalues()[2];
  eigenValues[3] = eigenLE.eigenvalues()[3];
  Eigen::Matrix4cd eigenVectors = eigenLE.eigenvectors().adjoint();

  // 1) check if eigenvalues are real
  for (int i = 0; i < 4; i++)
  {
    if (abs(imag(eigenValues[i])) > 1e-10)
    {
      // std::cout << "Imaginary eigenvalues!\n";
      return false;
    }
  }
  // Calculates the maximum eigenvalue
  double maxEigen = real(eigenValues[0]);
  int maxID = 0;
  for (int i = 1; i < 4; i++)
  {
    if (real(eigenValues[i]) > maxEigen)
    {
      maxEigen = real(eigenValues[i]);
      maxID = i;
    }
  }

  // 2) Check that the largest one is positive
  if (maxEigen < 0)
  {
    // std::cout << "Largest negative, eigenvalues!\n";
    return false;
  }

  // 3) Check linear independence
  if (std::abs(eigenVectors.determinant()) < 0.001)
  {
    // std::cout << "Not linear indep!\n";
    return false;
  }
  // 4) Check that the largest eigenvector is timelike and the others spacelike
  Eigen::Matrix4cd VgVdagger = eigenVectors * mink * eigenVectors.adjoint();
  for (int i = 0; i < 4; i++)
  {
    if (i != maxID)
    {
      if (real(VgVdagger(i, i)) > 0)
        return false;
    }
  }
  if (real(VgVdagger(maxID, maxID)) < 0)
    return false;
  return true;
}

bool THDM::is_cp_conserved() const
{
  /* The Higgs scalar potential and vacuum is CP conserving iff eq.3.1 in
    arXiv:1011.6188v2 is satisfied. We calculate these three imaginary parts of
    combinations of quartic couplings in the Higgs basis here. */

  Base_higgs higgs = _base_generic.convert_to_higgs();
  double imag56 = abs(imag(conj(higgs.Z5) * higgs.Z6 * higgs.Z6));
  double imag57 = abs(imag(conj(higgs.Z5) * higgs.Z7 * higgs.Z7));
  double imag67 = abs(imag(conj(higgs.Z6) * higgs.Z7));

  static const double ANGLE_PRECISION = 1.E-6;

  if (imag56 > ANGLE_PRECISION || imag57 > ANGLE_PRECISION ||
      imag67 > ANGLE_PRECISION)
  {
    _console.debug << "The model is CP violating:\n";
    _console.debug << "imag(Z5* Z6^2) = " << imag56 << std::endl;
    _console.debug << "imag(Z5* Z7^2) = " << imag57 << std::endl;
    _console.debug << "imag(Z6* Z7) = " << imag67 << std::endl;
    return false;
  }

  return true;
}

bool THDM::enforce_z2Symmetric_potential(const bool soft)
{

  if (!soft)
    _base_generic.M12 = complex<double>(0., 0.);

  _base_generic.Lambda6 = complex<double>(0., 0.);
  _base_generic.Lambda7 = complex<double>(0., 0.);

  fix_treeLvl_tadpole_eqs();

  // calc_compact_from_generic();
  // return calc_higgs_from_compact();
  return true;
}

bool THDM::delete_imag_parts()
{
  _base_generic.M12 = complex<double>(real(_base_generic.M12), 0.);
  _base_generic.Lambda5 = complex<double>(real(_base_generic.Lambda5), 0.);
  _base_generic.Lambda6 = complex<double>(real(_base_generic.Lambda6), 0.);
  _base_generic.Lambda7 = complex<double>(real(_base_generic.Lambda7), 0.);

  fix_treeLvl_tadpole_eqs();

  return true;
}

bool THDM::is_pert_unit_stab() const
{
  if (!is_perturbative())
  {
    _console.warning << "[WARNING]: Perturbativity is violated!!\n";
    return false;
  }
  if (!is_unitary())
  {
    _console.warning << "[WARNING]: Unitarity is violated!\n";
    return false;
  }
  if (!is_stable())
  {
    _console.warning << "[WARNING]: Stability is violated!\n";
    return false;
  }

  /* Checks for imaginary masses. */
  for (int i = 0; i < 3; i++)
  {
    if (std::isnan(_mh[i]))
      return false;
  }
  if (std::isnan(_mHc))
    return false;

  _console.info << "Model is okay!\n";
  return true;
}

void THDM::calc_vev_tensors()
{
  _vhat[0] = complex<double>(cos(_base_generic.beta), 0.);
  _vhat[1] = sin(_base_generic.beta) * std::polar(1., _base_generic.xi);
  _what[0] = -sin(_base_generic.beta) * std::polar(1., -_base_generic.xi);
  _what[1] = complex<double>(cos(_base_generic.beta), 0.);
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      _V[i][j] = _vhat[i] * conj(_vhat[j]);
      _W[i][j] = _what[i] * conj(_what[j]);
    }
  }
}

void THDM::calc_eta_from_kF_rF_ckm()
{
  calc_vev_tensors();

  // Rotate with CKM to go to flavor basis.
  Eigen::Matrix3cd kD = _VCKM * _kD;
  Eigen::Matrix3cd rD = _VCKM * _rD;

  _eta1U = _kU * _vhat[0] + _rU * _what[0];
  _eta2U = _kU * _vhat[1] + _rU * _what[1];
  _eta1D = kD * conj(_vhat[0]) + rD * conj(_what[0]);
  _eta2D = kD * conj(_vhat[1]) + rD * conj(_what[1]);
  _eta1L = _kL * conj(_vhat[0]) + _rL * conj(_what[0]);
  _eta2L = _kL * conj(_vhat[1]) + _rL * conj(_what[1]);
}

void THDM::calc_kF_rF_ckm_from_etaF()
{
  calc_vev_tensors();

  _kU = conj(_vhat[0]) * _eta1U + conj(_vhat[1]) * _eta2U;
  _kD = _vhat[0] * _eta1D + _vhat[1] * _eta2D;
  _kL = _vhat[0] * _eta1L + _vhat[1] * _eta2L;
  _rU = conj(_what[0]) * _eta1U + conj(_what[1]) * _eta2U;
  _rD = _what[0] * _eta1D + _what[1] * _eta2D;
  _rL = _what[0] * _eta1L + _what[1] * _eta2L;

  BiUnitary(_kU, _kD, _kL, _rU, _rD, _rL, _VCKM, _mup, _mdn, _ml, _v2);
}

void THDM::print_features() const
{
  auto check = [](const string &message, const bool boolean) {
    std::cout << message;
    if (boolean)
      std::cout << "\x1B[0;32mtrue\x1B[0m\n";
    else
      std::cout << "\x1B[1;31mfalse\x1B[0m\n";
  };

  check("CP conserving: ", is_cp_conserved());
  check("Perturbative: ", is_perturbative());
  check("Unitary (tree-lvl): ", is_unitary());
  check("Stable (tree-lvl): ", is_stable());
  check("Stable (tree-lvl, z2sym): ", is_stable_z2sym());

  print_rgeResults();
}

void THDM::calc_rgeResults()
{
  THDM thdm(*this);
  //  thdm.print_all();
  thdm.set_logLevel(LOG_ERRORS);
  thdm.evolve();
  _rgeResults = thdm.get_rgeResults();
}

void THDM::print_all() const
{
  std::cout << "\n"
               "------------------------------------------------------------\n";
  std::cout << "THDM\n";
  std::cout << "Renormalization scale: " << _renormScale << " GeV\n";
  print_features();
  std::cout << "g1 = " << _g1 << std::endl;
  std::cout << "g2 = " << _g2 << std::endl;
  std::cout << "g3 = " << _g3 << std::endl;
  std::cout << "v = " << sqrt(_v2) << std::endl;
  print_z2Symmetry();
  std::cout << "tanb = " << tan(_base_generic.beta) << std::endl;
  std::cout << "xi = " << _base_generic.xi << std::endl;
  print_potential();
  print_higgs_masses();
  print_neutral_higgs_eigenVectors();
#ifdef SPHENO
  _spheno.print();
#endif
  print_CKM();
  print_yukawa();

  std::cout << "\n"
               "------------------------------------------------------------\n";
}

void THDM::print_potential() const
{

  // Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Base_higgs higgs = _base_generic.convert_to_higgs();

  Table tab(4);
  tab.set_title("Potential, Q = " + stringAuto(get_renormalization_scale()) +
                " GeV");
  tab.add_row(
      vector<string>{"Generic basis",
                     "(tanb=" + stringAuto(tan(_base_generic.beta)) + ")",
                     "Higgs basis", ""},
      true);
  tab.add_row(vector<string>{"M_11^2 =", to_string(_base_generic.M112),
                             "Y1 =", to_string(higgs.Y1)});
  tab.add_row(vector<string>{"M_22^2 =", to_string(_base_generic.M222),
                             "Y2 =", to_string(higgs.Y2)});
  tab.add_row(vector<string>{"M_12^2 =", stringAuto(_base_generic.M12),
                             "Y3 =", stringAuto(higgs.Y3)});
  tab.add_row(vector<string>{"Lambda_1 =", to_string(_base_generic.Lambda1),
                             "Z1 =", to_string(higgs.Z1)});
  tab.add_row(vector<string>{"Lambda_2 =", to_string(_base_generic.Lambda2),
                             "Z2 =", to_string(higgs.Z2)});
  tab.add_row(vector<string>{"Lambda_3 =", to_string(_base_generic.Lambda3),
                             "Z3 =", to_string(higgs.Z3)});
  tab.add_row(vector<string>{"Lambda_4 =", to_string(_base_generic.Lambda4),
                             "Z4 =", to_string(higgs.Z4)});
  tab.add_row(vector<string>{"Lambda_5 =", stringAuto(_base_generic.Lambda5),
                             "Z5 =", stringAuto(higgs.Z5)});
  tab.add_row(vector<string>{"Lambda_6 =", stringAuto(_base_generic.Lambda6),
                             "Z6 =", stringAuto(higgs.Z6)});
  tab.add_row(vector<string>{"Lambda_7 =", stringAuto(_base_generic.Lambda7),
                             "Z7 =", stringAuto(higgs.Z7)});
  tab.print();

  // _console.info << inv << std::endl;
}

void THDM::print_z2Symmetry() const
{
  std::cout << "Yukawa Z_2 symmetry: ";
  switch (_z2_symmetry)
  {
  case NO_SYMMETRY:
    std::cout << "NONE\n";
    break;
  case TYPE_I:
    std::cout << "TYPE_I\n";
    break;
  case TYPE_II:
    std::cout << "TYPE_II\n";
    break;
  case TYPE_III:
    std::cout << "TYPE_III\n";
    break;
  case TYPE_IV:
    std::cout << "TYPE_IV\n";
    break;
  }
}

void THDM::print_higgs_masses() const
{
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Table tab(4);
  tab.set_frame_style("-", "|");
  tab.add_row(vector<string>{"Tree-lvl masses", "[GeV]", "Mixing angles", ""},
              true);
  tab.add_row(
      vector<string>{"m_h1:", stringAuto(_mh[0]), "s12:", stringAuto(inv.s12)});
  tab.add_row(
      vector<string>{"m_h2:", stringAuto(_mh[1]), "c13:", stringAuto(inv.c13)});
  tab.add_row(vector<string>{"m_h3:", stringAuto(_mh[2]),
                             "cPhi:", stringAuto(inv.cPhi)});
  tab.add_row(vector<string>{"m_Hc:", stringAuto(_mHc), "", ""});
  tab.print();
  cout << "\n";
}

void THDM::print_fermion_masses() const
{
  Table tab(2);
  tab.set_frame_style("-", "|");
  tab.set_title("Fermions");
  tab.add_row(vector<string>{"Masses", "[GeV]"}, true);
  tab.add_row(vector<string>{"mT:", stringAuto(_mup[2])});
  tab.add_row(vector<string>{"mC:", stringAuto(_mup[1])});
  tab.add_row(vector<string>{"mU:", stringAuto(_mup[0])});
  tab.add_row(vector<string>{"mB:", stringAuto(_mdn[2])});
  tab.add_row(vector<string>{"mS:", stringAuto(_mdn[1])});
  tab.add_row(vector<string>{"mD:", stringAuto(_mdn[0])});
  tab.add_row(vector<string>{"mTau:", stringAuto(_ml[2])});
  tab.add_row(vector<string>{"mMu:", stringAuto(_ml[1])});
  tab.add_row(vector<string>{"mE:", stringAuto(_ml[0])});
  tab.print();
  cout << "\n";
}

void THDM::print_neutral_higgs_eigenVectors() const
{

  Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Base_higgs higgs = _base_generic.convert_to_higgs();

  Eigen::Matrix3d rotMatrix;
  double s13 = -sqrt(abs(1. - inv.c13 * inv.c13));
  double c12 = sqrt(abs(1. - inv.s12 * inv.s12));
  double c23 = cos(arg(higgs.Z6) - acos(inv.cPhi));
  double s23 = sqrt(abs(1. - c23 * c23));
  rotMatrix << inv.c13 * c12, -c23 * inv.s12 - c12 * s13 * s23,
      -c12 * c23 * s13 + inv.s12 * s23, inv.c13 * inv.s12,
      c12 * c23 - inv.s12 * s13 * s23, -c23 * inv.s12 * s13 - c12 * s23, s13,
      inv.c13 * s23, inv.c13 * c23;

  Table tabEigenstate(4);
  tabEigenstate.set_frame_style("-", "|");
  tabEigenstate.add_row(
      vector<string>{"Eigenvector", "phi_1^0", "phi_2^0", "a^0"}, true);
  tabEigenstate.add_row(vector<string>{"h1:", stringAuto(rotMatrix(0, 0)),
                                       stringAuto(rotMatrix(0, 1)),
                                       stringAuto(rotMatrix(0, 2))});
  tabEigenstate.add_row(vector<string>{"h2:", stringAuto(rotMatrix(1, 0)),
                                       stringAuto(rotMatrix(1, 1)),
                                       stringAuto(rotMatrix(1, 2))});
  tabEigenstate.add_row(vector<string>{"h3:", stringAuto(rotMatrix(2, 0)),
                                       stringAuto(rotMatrix(2, 1)),
                                       stringAuto(rotMatrix(2, 2))});
  tabEigenstate.print();
  cout << "\n";
}

bool THDM::calc_treeLvl_masses_and_mixings()
{
  // Algorithm:
  //  1.) Calculate ordered eigenvalues of neutral Higgs mass matrix.
  //  2.) Fix U(2) invariant angles that rotates to a diagonal mass matrix.
  Base_higgs higgs = _base_generic.convert_to_higgs();

  // // The general case where all neutral Higgs fields mix
  // if (std::abs(higgs.Z6) > 1.E-10)
  // {
  // Mass matrix for neutral Higgs bosons in Higgs basis
  Eigen::Matrix3d mMatrix(3, 3);
  mMatrix.setZero();
  mMatrix << _v2 * higgs.Z1, _v2 * real(higgs.Z6), -_v2 * imag(higgs.Z6),
      _v2 * real(higgs.Z6),
      _v2 * 0.5 * (higgs.Z3 + higgs.Z4 + real(higgs.Z5)) + higgs.Y2,
      -_v2 * 0.5 * imag(higgs.Z5), -_v2 * imag(higgs.Z6),
      -_v2 * 0.5 * imag(higgs.Z5),
      _v2 * 0.5 * (higgs.Z3 + higgs.Z4 - real(higgs.Z5)) + higgs.Y2;

  // std::cout << "\n mMatrix:\n" << mMatrix << std::endl;// DEBUG

  // Set up an EigenSolver for the mass matrix. Since the matrix is symmetric,
  // it gives real eigenvalues
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> M(mMatrix);

  // M gives ordered eigenvalues by increasing size.
  for (int i = 0; i < 3; i++)
  {
    if (M.eigenvalues()[i] < 0)
    {
      _console.debug << "[DEBUG]: negative eigenvalues in mass matrix, (mh["
                     << i << "]^2 = " << M.eigenvalues()[i] << " GeV^2). \n";
      _mh[i] = 0.;
      return false;
    }
    else
      _mh[i] = sqrt(M.eigenvalues()[i]);
  }
  // }
  // // Case of Z6 = 0
  // else
  // {
  //   _console.warning << "[ERROR]: The case of Z6 = 0 has not been implemented
  //   "
  //                       "yet :( \n"; // TODO
  // }

  // Charged Higgs:
  if (higgs.Y2 + 0.5 * higgs.Z3 * _v2 < 0)
  {
    _console.debug << "[DEBUG]: Encountered negative mass for charged Higgs!\n";
    _mHc = 0.;
    return false;
  }
  else
    _mHc = sqrt(higgs.Y2 + 0.5 * higgs.Z3 * _v2);

  return true;
}

vector<double> THDM::get_higgs_treeLvl_masses() const
{
  return vector<double>{_mh[0], _mh[1], _mh[2], _mHc};
}

complex<double> THDM::get_z2_breaking_quantity() const
{
  Base_higgs higgs = _base_generic.convert_to_higgs();

  complex<double> z2Quantity =
      (higgs.Z1 - higgs.Z2) *
          (higgs.Z1 * higgs.Z7 + higgs.Z2 * higgs.Z6 -
           (higgs.Z3 + higgs.Z4 + higgs.Z5) * (higgs.Z6 + higgs.Z7)) +
      2. * (higgs.Z6 + higgs.Z7) * (higgs.Z6 + higgs.Z7) *
          (higgs.Z6 - higgs.Z7);
  (higgs.Z6 - higgs.Z7);

  return z2Quantity;
}

std::tuple<string, int, complex<double>> THDM::get_largest_diagonal_rF() const
{
  complex<double> maxYuk(0., 0.);
  int maxID[2] = {0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i)
    {
      if (abs((*rF[k])(i, i)) > abs(maxYuk))
      {
        maxYuk = (*rF[k])(i, i);
        maxID[0] = k;
        maxID[1] = i;
      }
    }

  vector<string> rFStrings = {"rU", "rD", "rL"};

  _console.info << "Largest diagonal yukawa element is " << rFStrings[maxID[0]]
                << "(" << maxID[1] + 1 << "," << maxID[1] + 1
                << ") = " << maxYuk << std::endl;

  return std::make_tuple(rFStrings[maxID[0]] + "(" +
                             std::to_string(maxID[1] + 1) + "," +
                             std::to_string(maxID[1] + 1) + ")",
                         maxID[1], maxYuk);
}

std::tuple<string, int, int, complex<double>>
THDM::get_largest_nonDiagonal_rF() const
{
  complex<double> maxYuk(0., 0.);
  int maxID[3] = {0, 0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
      {
        if (i != j && abs((*rF[k])(i, j)) > abs(maxYuk))
        {
          maxYuk = (*rF[k])(i, j);
          maxID[0] = k;
          maxID[1] = i;
          maxID[2] = j;
        }
      }

  vector<string> rFStrings = {"rU", "rD", "rL"};
  if (maxID[0] == 0 && maxID[1] == 0 && maxID[2] == 0)
  {
    _console.info << "All non-diagonal rF elements are zero.\n";
    return std::make_tuple(rFStrings[maxID[0]], maxID[1], maxID[2], maxYuk);
  }

  _console.info << "Largest non-diagonal yukawa element is "
                << rFStrings[maxID[0]] << "(" << maxID[1] + 1 << ","
                << maxID[2] + 1 << ") = " << maxYuk << std::endl;

  return std::make_tuple(rFStrings[maxID[0]] + "(" +
                             std::to_string(maxID[1] + 1) + "," +
                             std::to_string(maxID[2] + 1) + ")",
                         maxID[1], maxID[2], maxYuk);
}

std::tuple<string, int, int, complex<double>>
THDM::get_largest_nonDiagonal_lamF() const
{
  complex<double> maxYuk(0., 0.);
  int maxID[3] = {0, 0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};
  vector<const double *> mF = {_mup, _mdn, _ml};
  double v = sqrt(_v2);

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
      {
        double lambdaFij =
            v * abs((*rF[k])(i, j)) / sqrt(2. * mF[k][i] * mF[k][j]);

        if (i != j && abs((*rF[k])(i, j)) > abs(maxYuk))
        {
          maxYuk = lambdaFij;
          maxID[0] = k;
          maxID[1] = i;
          maxID[2] = j;
        }
      }

  vector<string> rFStrings = {"lamU", "lamD", "lamL"};
  if (maxID[0] == 0 && maxID[1] == 0 && maxID[2] == 0)
  {
    _console.info << "All non-diagonal rF elements are zero.\n";
    return std::make_tuple(rFStrings[maxID[0]], maxID[1], maxID[2], maxYuk);
  }

  _console.info << "Largest non-diagonal Cheng-Sher parameter is "
                << rFStrings[maxID[0]] << "(" << maxID[1] + 1 << ","
                << maxID[2] + 1 << ") = " << maxYuk << std::endl;

  return std::make_tuple(rFStrings[maxID[0]] + "(" +
                             std::to_string(maxID[1] + 1) + "," +
                             std::to_string(maxID[2] + 1) + ")",
                         maxID[1], maxID[2], maxYuk);
}

double THDM::get_lamF_element(const FermionSector flavor, const int i,
                              const int j) const
{
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};
  vector<const double *> mF = {_mup, _mdn, _ml};
  double v = sqrt(_v2);

  return v * abs((*rF[(int)flavor])(i, j)) /
         sqrt(2. * mF[(int)flavor][i] * mF[(int)flavor][j]);
}

Eigen::Matrix3cd THDM::get_lamF(const FermionSector flavor) const
{
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};
  vector<const double *> mF = {_mup, _mdn, _ml};
  double v = sqrt(_v2);

  Eigen::Matrix3cd lamF;
  lamF.setZero();
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      lamF(i, j) = v * abs((*rF[(int)flavor])(i, j)) /
                   sqrt(2. * mF[(int)flavor][i] * mF[(int)flavor][j]);

  return lamF;
}

std::tuple<string, int, double> THDM::get_largest_lambda() const
{
  double maxLambda = 0.;
  int maxID = 0;

  vector<double> absLambda = {
      abs(_base_generic.Lambda1), abs(_base_generic.Lambda2),
      abs(_base_generic.Lambda3), abs(_base_generic.Lambda4),
      abs(_base_generic.Lambda5), abs(_base_generic.Lambda6),
      abs(_base_generic.Lambda7)};

  for (unsigned int i = 0; i < absLambda.size(); ++i)
  {
    if (absLambda[i] > maxLambda)
    {
      maxLambda = absLambda[i];
      maxID = i;
    }
  }

  vector<string> lamStrings = {"Lambda1", "Lambda2", "Lambda3", "Lambda4",
                               "Lambda5", "Lambda6", "Lambda7"};

  _console.info << "Largest quartic coupling: |" << lamStrings[maxID]
                << "| = " << maxLambda << std::endl;

  return std::make_tuple(lamStrings[maxID], maxID + 1, absLambda[maxID]);
}

double THDM::get_v2() const { return _v2; }

vector<double> THDM::get_gauge_couplings() const
{
  return vector<double>{_g1, _g2, _g3};
}

vector<Eigen::Matrix3cd> THDM::get_yukawa_eta() const
{
  return vector<Eigen::Matrix3cd>{_eta1U, _eta1D, _eta1L,
                                  _eta2U, _eta2D, _eta2L};
}

vector<std::complex<double>> THDM::get_vevs() const
{
  double v = sqrt(_v2);
  return vector<std::complex<double>>{_vhat[0] * v, _vhat[1] * v};
}

vector<double> THDM::get_mup() const
{
  return vector<double>{_mup[0], _mup[1], _mup[2]};
}

vector<double> THDM::get_mdn() const
{
  return vector<double>{_mdn[0], _mdn[1], _mdn[2]};
}

vector<double> THDM::get_ml() const
{
  return vector<double>{_ml[0], _ml[1], _ml[2]};
}

Eigen::Matrix3cd THDM::get_vCkm() const { return _VCKM; }

void THDM::print_CKM() const { cout << " V_CKM:\n"
                                    << _VCKM << endl; }

void THDM::print_param_gen() const
{
  cout << "\nPotential parameters:\n";
  print_z2Symmetry();
  cout << "v = " << sqrt(_v2) << std::endl;
  cout << "tanb = " << tan(_base_generic.beta) << std::endl;
  cout << "xi = " << _base_generic.xi << std::endl;
  cout << "M_11^2 =" << _base_generic.M112 << std::endl;
  cout << "M_22^2 =" << _base_generic.M222 << std::endl;
  cout << "M_12^2 = " << _base_generic.M12 << std::endl;
  cout << "Lambda_1 = " << _base_generic.Lambda1 << std::endl;
  cout << "Lambda_2 = " << _base_generic.Lambda2 << std::endl;
  cout << "Lambda_3 = " << _base_generic.Lambda3 << std::endl;
  cout << "Lambda_4 = " << _base_generic.Lambda4 << std::endl;
  cout << "Lambda_5 = " << _base_generic.Lambda5 << std::endl;
  cout << "Lambda_6 = " << _base_generic.Lambda6 << std::endl;
  cout << "Lambda_7 = " << _base_generic.Lambda7 << std::endl;
  cout << "\nYukawa matrices::\n";
  cout << " eta1U\n"
       << _eta1U << endl;
  cout << " eta1D\n"
       << _eta1D << endl;
  cout << " eta1L\n"
       << _eta1L << endl;
  cout << " eta2U\n"
       << _eta2U << endl;
  cout << " eta2D\n"
       << _eta2D << endl;
  cout << " eta2L\n"
       << _eta2L << endl;
  cout << "\n";
}

void THDM::print_param_compact() const
{
  cout << "\nPotential parameters in compact basis:\n";
  print_z2Symmetry();
  cout << "v = " << sqrt(_v2) << std::endl;
  cout << _base_generic.convert_to_compact();
  cout << "\n";
}

void THDM::print_param_higgs() const
{
  cout << "\nPotential parameters:\n";
  print_z2Symmetry();
  cout << "v = " << sqrt(_v2) << std::endl;
  cout << "tanb = " << tan(_base_generic.beta) << std::endl;
  cout << "xi = " << _base_generic.xi << std::endl;
  cout << _base_generic.convert_to_higgs();
  cout << "\nkappa and rho Yukawa matrices:\n";
  cout << " kU\n"
       << _kU << endl;
  cout << " kD\n"
       << _kD << endl;
  cout << " kL\n"
       << _kL << endl;
  cout << " rU\n"
       << _rU << endl;
  cout << " rD\n"
       << _rD << endl;
  cout << " rL\n"
       << _rL << endl;
  cout << "\n";
}

void THDM::print_yukawa() const
{
  cout << "\neta Yukawa matrices::\n";
  cout << " eta1U\n"
       << _eta1U << endl;
  cout << " eta1D\n"
       << _eta1D << endl;
  cout << " eta1L\n"
       << _eta1L << endl;
  cout << " eta2U\n"
       << _eta2U << endl;
  cout << " eta2D\n"
       << _eta2D << endl;
  cout << " eta2L\n"
       << _eta2L << endl;

  cout << "\nkappa and rho Yukawa matrices:\n";
  cout << " kU\n"
       << _kU << endl;
  cout << " kD\n"
       << _kD << endl;
  cout << " kL\n"
       << _kL << endl;
  cout << " rU\n"
       << _rU << endl;
  cout << " rD\n"
       << _rD << endl;
  cout << " rL\n"
       << _rL << endl;
  cout << "\n";
}

void THDM::init()
{
  _modelName = "THDM";

  // Points to RGE functions. 130 is the total number of parameters in RGE
  // system.
  init_rge_systems(rgeFuncThdm_1loop, rgeFuncThdm_2loop, 130);

  _renormScale = 0.;
  _base_generic.beta = 0.;
  _base_generic.xi = 0.;

  _eta1U.setZero();
  _eta2U.setZero();
  _eta1D.setZero();
  _eta2D.setZero();
  _eta1L.setZero();
  _eta2L.setZero();

  _kU.setZero();
  _kD.setZero();
  _kL.setZero();
  _rU.setZero();
  _rD.setZero();
  _rL.setZero();

  _VCKM.setZero();
}

bool THDM::reset_to_saved_state()
{
  _rgeResults.reset();

  if (_ySaved != nullptr)
  {
    set_model_from_y(_ySaved); // Set parameters in generic basis
    _renormScale = _savedRenormScale;

    calc_kF_rF_ckm_from_etaF();
  }
  else
  {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
  return true;
}

bool THDM::reset(const double y[], const double &renormScale)
{
  _rgeResults.reset();
  _renormScale = renormScale;

  if (y != nullptr)
  {
    set_model_from_y(y); // Set parameters in generic basis

    calc_kF_rF_ckm_from_etaF();
  }
  else
  {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
  return true;
}

void THDM::set_v2(const double &v2) { _v2 = v2; }

void THDM::set_gauge_couplings(const vector<double> &couplings)
{
  if (couplings.size() != 3)
  {
    _console.error << "[ERROR]: Couldn't set gauge couplings.\n";
    return;
  }

  _g1 = couplings[0];
  _g2 = couplings[1];
  _g3 = couplings[2];
}

void THDM::set_fermion_masses(const vector<double> &mup,
                              const vector<double> &mdn,
                              const vector<double> &ml)

{
  if (mup.size() != 3 || mdn.size() != 3 || ml.size() != 3)
  {
    _console.error << "[ERROR]: Couldn't set fermion masses.\n";
    return;
  }

  _mup[0] = mup[0];
  _mup[1] = mup[1];
  _mup[2] = mup[2];
  _mdn[0] = mdn[0];
  _mdn[1] = mdn[1];
  _mdn[2] = mdn[2];
  _ml[0] = ml[0];
  _ml[1] = ml[1];
  _ml[2] = ml[2];
}

void THDM::set_vCkm(const Eigen::Matrix3cd &VCKM) { _VCKM = VCKM; }

void THDM::set_sm(const SM &sm)
{
  _renormScale = sm.get_renormalization_scale();

  // Setting SM ms-bar parameters
  _v2 = sm.get_v2();
  set_gauge_couplings(sm.get_gauge_couplings());
  set_fermion_masses(sm.get_mup(), sm.get_mdn(), sm.get_ml());
  set_vCkm(sm.get_vCkm());

  // Translating to 2HDM.
  _base_generic.beta = 0.;
  _base_generic.Lambda1 = 2. * sm.get_lambda();

  set_kF_from_fermion_masses();
  // calc_treeLvl_masses_and_mixings();

  calc_eta_from_kF_rF_ckm();
}

void THDM::set_kF_from_fermion_masses()
{
  _kU.setZero();
  _kD.setZero();
  _kL.setZero();

  for (int i = 0; i < 3; i++)
  {
    _kU(i, i) = complex<double>(_mup[i] * sqrt(2. / _v2), 0.);
    _kD(i, i) = complex<double>(_mdn[i] * sqrt(2. / _v2), 0.);
    _kL(i, i) = complex<double>(_ml[i] * sqrt(2. / _v2), 0.);
  }
}

void THDM::create_data_files()
{
  // Decides the filename type
  string looporder = _rgeConfig.twoloop ? "twoloop" : "oneloop";

  _files.set_directory("output/" + _rgeConfig.evolutionName);

  // Data files path
  _lambdaFile = "data/lambda_" + looporder + "_generic.dat";
  _ZFile = "data/Z_" + looporder + "_higgs.dat";
  _gaugeFile = "data/gauge_" + looporder + ".dat";
  _MijFile = "data/Mij_" + looporder + "_generic.dat";
  _VEVFile = "data/VevAndMixing_" + looporder + ".dat";
  _eta1UFile = "data/eta1U_" + looporder + ".dat";
  _eta2UFile = "data/eta2U_" + looporder + ".dat";
  _eta1DFile = "data/eta1D_" + looporder + ".dat";
  _eta2DFile = "data/eta2D_" + looporder + ".dat";
  _eta1LFile = "data/eta1L_" + looporder + ".dat";
  _eta2LFile = "data/eta2L_" + looporder + ".dat";
  _kUFile = "data/kU_" + looporder + ".dat";
  _rUFile = "data/rU_" + looporder + ".dat";
  _kDFile = "data/kD_" + looporder + ".dat";
  _rDFile = "data/rD_" + looporder + ".dat";
  _kLFile = "data/kL_" + looporder + ".dat";
  _rLFile = "data/rL_" + looporder + ".dat";
  _z2Breaking = "data/z2Breaking_" + looporder + ".dat";
  _massesFile = "data/HiggsMasses_" + looporder + ".dat";

  // Creates the files
  _files.create_multiple_files(vector<string>{
      _lambdaFile, _MijFile, _VEVFile, _gaugeFile, _eta1UFile, _eta2UFile,
      _eta1DFile, _eta2DFile, _eta1LFile, _eta2LFile, _ZFile, _kUFile, _kDFile,
      _kLFile, _rUFile, _rDFile, _rLFile, _z2Breaking, _massesFile});

  // Adds a commented first line
  _files.add_line(_lambdaFile,
                  vector<string>{"#energy[GeV]", "Lambda1", "Lambda2",
                                 "Lambda3", "Lambda4", "re(Lambda5)",
                                 "im(Lambda5)", "re(Lambda6)", "im(Lambda6)",
                                 "re(Lambda7)", "im(Lambda7)"});
  _files.add_line(_MijFile, vector<string>{"#energy[GeV]", "M112", "M222",
                                           "re(M12)", "im(M12"});
  _files.add_line(_gaugeFile, vector<string>{"#energy[GeV]", "g1", "g2", "g3"});
  _files.add_line(_VEVFile, vector<string>{"#energy[GeV]", "sqrt(abs(v2))",
                                           "beta", "tan(beta)", "xi"});
  _files.add_line(_eta1UFile, vector<string>{"#energy[GeV]",
                                             "abs(eta1U_ij)(3 x 3 coloumns)"});
  _files.add_line(_eta2UFile, vector<string>{"#energy[GeV]",
                                             "abs(eta2U_ij)(3 x 3 coloumns)"});
  _files.add_line(_eta1DFile, vector<string>{"#energy[GeV]",
                                             "abs(eta1D_ij)(3 x 3 coloumns)"});
  _files.add_line(_eta2DFile, vector<string>{"#energy[GeV]",
                                             "abs(eta2D_ij)(3 x 3 coloumns)"});
  _files.add_line(_eta1LFile, vector<string>{"#energy[GeV]",
                                             "abs(eta1L_ij)(3 x 3 coloumns)"});
  _files.add_line(_eta2LFile, vector<string>{"#energy[GeV]",
                                             "abs(eta2L_ij)(3 x 3 coloumns)"});
  _files.add_line(_kUFile,
                  vector<string>{"#energy[GeV]", "abs(kU_ij)(3 x 3 coloumns)"});
  _files.add_line(_rUFile,
                  vector<string>{"#energy[GeV]", "abs(rU_ij)(3 x 3 coloumns)"});
  _files.add_line(_kDFile,
                  vector<string>{"#energy[GeV]", "abs(kD_ij)(3 x 3 coloumns)"});
  _files.add_line(_rDFile,
                  vector<string>{"#energy[GeV]", "abs(rD_ij)(3 x 3 coloumns)"});
  _files.add_line(_kLFile,
                  vector<string>{"#energy[GeV]", "abs(kL_ij)(3 x 3 coloumns)"});
  _files.add_line(_rLFile,
                  vector<string>{"#energy[GeV]", "abs(rL_ij)(3 x 3 coloumns)"});
  _files.add_line(_z2Breaking, vector<string>{"#energy[GeV]", "rU/kU", "1/tb"});

  _files.add_line(_massesFile, vector<string>{"#energy[GeV]", "m_h1", "m_h2",
                                              "m_h3", "m_h4"});
}

void THDM::write_to_data_files(const double &t)
{
  // Writes the member variables to files created in create_data_files()

  Base_higgs higgs = _base_generic.convert_to_higgs();
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);

  _files.add_line(
      _lambdaFile,
      vector<double>{exp(t), _base_generic.Lambda1, _base_generic.Lambda2,
                     _base_generic.Lambda3, _base_generic.Lambda4,
                     real(_base_generic.Lambda5), imag(_base_generic.Lambda5),
                     real(_base_generic.Lambda6), imag(_base_generic.Lambda6),
                     real(_base_generic.Lambda7), imag(_base_generic.Lambda7)});
  _files.add_line(
      _ZFile, vector<double>{exp(t), higgs.Z1, higgs.Z2, higgs.Z3, higgs.Z4,
                             real(higgs.Z5), imag(higgs.Z5), real(higgs.Z6),
                             imag(higgs.Z6), real(higgs.Z7), imag(higgs.Z7)});
  _files.add_line(_MijFile,
                  vector<double>{exp(t), _base_generic.M112, _base_generic.M222,
                                 real(_base_generic.M12),
                                 imag(_base_generic.M12)});
  _files.add_line(_VEVFile,
                  vector<double>{exp(t), sqrt(abs(_v2)), _base_generic.beta,
                                 tan(_base_generic.beta), _base_generic.xi,
                                 inv.s12, inv.c13, inv.cPhi});
  _files.add_line(_gaugeFile, vector<double>{exp(t), _g1, _g2, _g3});
  _files.add_line(_eta1UFile, t, _eta1U);
  _files.add_line(_eta2UFile, t, _eta2U);
  _files.add_line(_eta1DFile, t, _eta1D);
  _files.add_line(_eta2DFile, t, _eta2D);
  _files.add_line(_eta1LFile, t, _eta1L);
  _files.add_line(_eta2LFile, t, _eta2L);
  _files.add_line(_kUFile, t, _kU);
  _files.add_line(_rUFile, t, _rU);
  _files.add_line(_kDFile, t, _kD);
  _files.add_line(_rDFile, t, _rD);
  _files.add_line(_kLFile, t, _kL);
  _files.add_line(_rLFile, t, _rL);

  double aU = abs(_rU(2, 2) / _kU(2, 2));
  _files.add_line(_z2Breaking,
                  vector<double>{exp(t), aU, 1. / tan(_base_generic.beta)});

  vector<double> masses = get_higgs_treeLvl_masses();
  // vector<double> poleMasses = _spheno.get_output();
  masses.insert(masses.begin(), exp(t));
  // masses.insert(masses.end(), poleMasses.begin(), poleMasses.end());
  _files.add_line(_massesFile, masses);
}

#ifdef SPHENO
bool THDM::run_spheno(const int massLoopLvl)
{
  _console.info << "Running SPheno...\n";
  return _spheno.run(*this, massLoopLvl);
}

bool THDM::is_within_spheno_limits() const { return _spheno.is_allowed(); }

vector<double> THDM::get_spheno_output() const { return _spheno.get_output(); }

void THDM::print_spheno_results() const { _spheno.print(); }

#endif


void THDM::write_slha_file(const int sphenoLoopLvl, const string &file) const
{
  FILE *output;
  output = fopen(file.c_str(), "w");

  if (output == NULL)
  {
    printf("Cannot open file \"%s\" for writing\n", file.c_str());
    return;
  }
  string info = "#  Created: " + date() + "\n";
  fprintf(
      output,
      "##################################################################\n");
  fprintf(output, "#\n");
  fprintf(output, "#  2HDM SLHA file                                 \n");
  fprintf(output, "%s", info.c_str());
  fprintf(output, "#\n");
  fprintf(
      output,
      "##################################################################\n");
  fprintf(output, "Block THDME   # Features at current scale\n");
  fprintf(output, " 0   % 16.8e   # Renormalization scale\n",
          get_renormalization_scale());
  fprintf(output, " 1   %i   # Yukawa symmetry (0=none or Type 1-4)\n",
          _z2_symmetry);
  fprintf(output, " 2   %i   # CP conserving (0=false, 1=true)\n",
          is_cp_conserved());
  fprintf(output, " 3   %i   # Perturbative (0=false, 1=true)\n",
          is_perturbative() ? 1 : 0);
  fprintf(output, " 4   %i   # Unitary, tree-lvl (0=false, 1=true)\n",
          is_unitary() ? 1 : 0);
  fprintf(output, " 5   %i   # Stable, tree-lvl  (0=false, 1=true)\n",
          is_stable() ? 1 : 0);
  fprintf(output, " 6   %i   # Stable, tree-lvl and z2sym  (0=false, 1=true)\n",
          is_stable_z2sym() ? 1 : 0);
  // If the model has undergone RG evolution
  if (_rgeResults.evolved)
  {
    fprintf(output, "Block RGE   # Results of RGE evolution\n");
    fprintf(output, " 0   % 16.8e   # Start scale\n", _rgeResults.e0);
    fprintf(output,
            " 1   % 16.8e   # Finish scale (same as current renorm scale)\n",
            _rgeResults.ef);
    fprintf(
        output,
        " 2   % 16.8e   # Perturbativity breakdown scale (-1 = no violation)\n",
        _rgeResults.ef_pert);
    fprintf(output,
            " 3   % 16.8e   # Unitarity breakdown scale (-1 = no violation)\n",
            _rgeResults.ef_unit);
    fprintf(output,
            " 4   % 16.8e   # Stability breakdown scale (-1 = no violation)\n",
            _rgeResults.ef_stab);
  }

  // Scalar potential
  fprintf(output, "Block MIJ2      # Mij^2\n");
  fprintf(output, " 1   % 16.8e   # M112\n", _base_generic.M112);
  fprintf(output, " 2   % 16.8e   # M222\n", _base_generic.M222);
  fprintf(output, "Block MINPAR      # Input parameters (real part)\n");
  fprintf(output, " 1   % 16.8e   # Lambda1Input\n", _base_generic.Lambda1);
  fprintf(output, " 2   % 16.8e   # Lambda2Input\n", _base_generic.Lambda2);
  fprintf(output, " 3   % 16.8e   # Lambda3Input\n", _base_generic.Lambda3);
  fprintf(output, " 4   % 16.8e   # Lambda4Input\n", _base_generic.Lambda4);
  fprintf(output, " 5   % 16.8e   # re(Lambda5Input)\n",
          real(_base_generic.Lambda5));
  fprintf(output, " 6   % 16.8e   # re(Lambda6Input)\n",
          real(_base_generic.Lambda6));
  fprintf(output, " 7   % 16.8e   # re(Lambda7Input)\n",
          real(_base_generic.Lambda7));
  fprintf(output, " 9   % 16.8e   # re(M12input)\n", real(_base_generic.M12));
  fprintf(output, " 10  % 16.8e   # TanBeta\n", tan(_base_generic.beta));
  fprintf(output, "Block IMMINPAR      # Input parameters (imaginary part)\n");
  fprintf(output, " 5   % 16.8e   # im(Lambda5Input)\n",
          imag(_base_generic.Lambda5));
  fprintf(output, " 6   % 16.8e   # im(Lambda6Input)\n",
          imag(_base_generic.Lambda6));
  fprintf(output, " 7   % 16.8e   # im(Lambda7Input)\n",
          imag(_base_generic.Lambda7));
  fprintf(output, " 9   % 16.8e   # im(M12input)\n", imag(_base_generic.M12));
  // Yukawa sector
  fprintf(output, "Block ETA1UIN    # eta1U Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1U(%i,%i)\n", i + 1, j + 1,
              real(_eta1U(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block ETA1DIN    # eta1D Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1D(%i,%i)\n", i + 1, j + 1,
              real(_eta1D(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block ETA1LIN    # eta1L Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1L(%i,%i)\n", i + 1, j + 1,
              real(_eta1L(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block ETA2UIN    # eta2U Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2U(%i,%i)\n", i + 1, j + 1,
              real(_eta2U(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block ETA2DIN    # eta2D Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2D(%i,%i)\n", i + 1, j + 1,
              real(_eta2D(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block ETA2LIN    # eta2L Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2L(%i,%i)\n", i + 1, j + 1,
              real(_eta2L(i, j)), i + 1, j + 1);
    }

  fprintf(output, "Block IMETA1UIN    # eta1U Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1U(%i,%i)\n", i + 1, j + 1,
              imag(_eta1U(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMETA1DIN    # eta1D Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1D(%i,%i)\n", i + 1, j + 1,
              imag(_eta1D(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMETA1LIN    # eta1L Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta1L(%i,%i)\n", i + 1, j + 1,
              imag(_eta1L(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMETA2UIN    # eta2U Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2U(%i,%i)\n", i + 1, j + 1,
              imag(_eta2U(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMETA2DIN    # eta2D Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2D(%i,%i)\n", i + 1, j + 1,
              imag(_eta2D(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMETA2LIN    # eta2L Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # eta2L(%i,%i)\n", i + 1, j + 1,
              imag(_eta2L(i, j)), i + 1, j + 1);
    }

  double v = sqrt(_v2);
  fprintf(output, "Block HMIXIN       #  \n");
  fprintf(output, " 102   % 16.8e   # re(v1)\n", cos(_base_generic.beta) * v);
  fprintf(output, " 103   % 16.8e   # re(v2)\n",
          real(std::polar(1., _base_generic.xi) * sin(_base_generic.beta) * v));
  fprintf(output, "Block IMHMIXIN       #  \n");
  fprintf(output, " 102   % 16.8e   # im(v1)\n", 0.);
  fprintf(output, " 103   % 16.8e   # im(v2)\n",
          imag(std::polar(1., _base_generic.xi) * sin(_base_generic.beta) * v));
  fprintf(output, "Block GAUGEIN       #  \n");
  fprintf(output, " 1   % 16.8e   # g1\n", _g1);
  fprintf(output, " 2   % 16.8e   # g2\n", _g2);
  fprintf(output, " 3   % 16.8e   # g3\n", _g3);

  // SPheno input
  if (sphenoLoopLvl != -1)
  {
    int loopCorrectedMasses = sphenoLoopLvl > 0 ? 1 : 0;
    int TwoLoopHiggsMassCorr = sphenoLoopLvl == 2 ? 1 : 0;
    fprintf(output, "Block MODSEL      #\n");
    fprintf(output, " 1 1               #  1/0: High/low scale input\n");
    fprintf(output, " 2 1              # Boundary Condition\n");
    fprintf(output, " 6 1               # Generation Mixing\n");
    fprintf(output, "Block SMINPUTS    # Standard Model inputs\n");
    fprintf(output, " 2 1.166370E-05    # G_F,Fermi constant\n");
    fprintf(output, " 3 1.187000E-01    # alpha_s(MZ) SM MSbar\n");
    fprintf(output, " 4 9.118870E+01    # Z-boson pole mass\n");
    fprintf(output, " 5 4.180000E+00    # m_b(mb) SM MSbar\n");
    fprintf(output, " 6 1.735000E+02    # m_top(pole)\n");
    fprintf(output, " 7 1.776690E+00    # m_tau(pole)\n");
    fprintf(output, "Block SPhenoInput   # SPheno specific input \n");
    fprintf(output, "  1 -1              # error level \n");
    fprintf(output, "  2  0              # SPA conventions \n");
    fprintf(output, "  7  %i              # Skip 2-loop Higgs corrections \n",
            TwoLoopHiggsMassCorr);
    fprintf(output,
            "  8  3              # Method used for two-loop calculation \n");
    fprintf(output,
            "  9  1              # Gaugeless limit used at two-loop \n");
    fprintf(output, " 10  0              # safe-mode used at two-loop \n");
    fprintf(output, " 11 0               # calculate branching ratios \n");
    fprintf(output,
            " 13 0               # 3-Body decays: none (0), fermion (1), "
            "scalar (2), both (3) \n");
    fprintf(
        output,
        " 14 0               # Run couplings to scale of decaying particle \n");
    fprintf(output, " 12 1.000E-04       # write only branching ratios larger "
                    "than this value \n");
    fprintf(output,
            " 15 1.000E-30       # write only decay if width larger than "
            "this value \n");
    fprintf(output, " 16 0              # One-loop decays \n");
    fprintf(output, " 19 -2              # Matching order (-2:automatic, "
                    "-1:pole, 0-2: tree, one- & two-loop) \n");
    fprintf(
        output,
        " 31 -1              # fixed GUT scale (-1: dynamical GUT scale) \n");
    fprintf(output, " 32 0               # Strict unification \n");
    fprintf(output, " 33   % 16.8e   # Renormalization scale, Q\n",
            get_renormalization_scale());
    fprintf(output, " 34 1.000E-04       # Precision of mass calculation \n");
    fprintf(output, " 35 40              # Maximal number of iterations\n");
    fprintf(output, " 36 5               # Minimal number of iterations before "
                    "discarding points\n");
    fprintf(output, " 37 1               # Set Yukawa scheme  \n");
    fprintf(output, " 38 1               # 1- or 2-Loop RGEs \n");
    fprintf(output, " 50 1               # Majorana phases: use only positive "
                    "masses (put 0 to use file with CalcHep/Micromegas!)\n");
    fprintf(output, " 51 0               # Write Output in CKM basis \n");
    fprintf(
        output,
        " 52 0               # Write spectrum in case of tachyonic states \n");
    fprintf(output, " 55 %i               # Calculate loop corrected masses \n",
            loopCorrectedMasses);
    fprintf(output,
            " 57 1               # Calculate low energy constraints \n");
    fprintf(output, " 65 1               # Solution tadpole equation \n");
    fprintf(output, " 66 1               # Two-Scale Matching \n");
    fprintf(output,
            " 67 %i               # effective Higgs mass calculation \n",
            loopCorrectedMasses);
    fprintf(output, " 75 0               # Write WHIZARD files \n");
    fprintf(output, " 76 0               # Write HiggsBounds file   \n");
    fprintf(output,
            " 77 0               # Output for MicrOmegas (running masses "
            "for light quarks; real mixing matrices)\n");
    fprintf(output, " 78 0               # Output for MadGraph (writes also "
                    "vanishing blocks)   \n");
    fprintf(output,
            " 79 0               # Write WCXF files (exchange format for "
            "Wilson coefficients) \n");
    fprintf(output, " 86 0.              # Maximal width to be counted as "
                    "invisible in Higgs decays; -1: only LSP \n");
    fprintf(output, " 440 0               # Tree-level unitarity constraints "
                    "(limit s->infinity) \n");
    fprintf(output,
            " 441 0               # Full tree-level unitarity constraints \n");
    fprintf(output, " 442 1000.           # sqrt(s_min)   \n");
    fprintf(output, " 443 2000.           # sqrt(s_max)   \n");
    fprintf(output, " 444 5               # steps   \n");
    fprintf(output, " 445 0               # running   \n");
    fprintf(output,
            " 510 1.              # Write tree level values for tadpole "
            "solutions \n");
    fprintf(output,
            " 515 0               # Write parameter values at GUT scale \n");
    fprintf(output,
            " 520 0.              # Write effective Higgs couplings "
            "(HiggsBounds blocks): put 0 to use file with MadGraph! \n");
    fprintf(output, " 521 0.              # Diphoton/Digluon widths including "
                    "higher order \n");
    fprintf(output,
            " 525 0.              # Write loop contributions to diphoton "
            "decay of Higgs \n");
    fprintf(output, " 530 0.              # Write Blocks for Vevacious\n");
    fprintf(output,
            "Block DECAYOPTIONS   # Options to turn on/off specific decays \n");
    fprintf(output, " 1    0     # Calc 3-Body decays of Fu \n");
    fprintf(output, " 2    0     # Calc 3-Body decays of Fe \n");
    fprintf(output, " 3    0     # Calc 3-Body decays of Fd \n");
    fprintf(output, " 1001 0     # Loop Decay of Fu \n");
    fprintf(output, " 1002 0     # Loop Decay of Fe \n");
    fprintf(output, " 1003 0     # Loop Decay of Fd \n");
    fprintf(output, " 1004 0     # Loop Decay of hh \n");
    fprintf(output, " 1005 0     # Loop Decay of Ah \n");
    fprintf(output, " 1006 0     # Loop Decay of Hm \n");
    fprintf(output, " 1114 0.     # U-factors (0: off, 1:p2_i=m2_i, 2:p2=0, "
                    "p3:p2_i=m2_1) \n");
    fprintf(output,
            " 1115 %i.     # Use loop-corrected masses for external states \n",
            loopCorrectedMasses);
    fprintf(output, " 1116 0.     # OS values for W,Z and fermions (0: off, "
                    "1:g1,g2,v 2:g1,g2,v,Y_i)\n");
    fprintf(output, " 1117 0.     # Use defined counter-terms\n");
    fprintf(output,
            " 1118 %i.     # Use everywhere loop-corrected masses for "
            "loop-induced decays\n",
            loopCorrectedMasses);
  }
  fclose(output);

  _console.info << "SLHA output file, " << file << ", created!\n";
}

bool THDM::set_from_slha_file(const string &file)
{
  // Loading the output
  std::ifstream modelFileStream(file, std::ios_base::in);

  if (!modelFileStream)
  {
    _console.warning << "[ERROR]: Couldn't open " << file << std::endl;
    return false;
  }

  double y[_numParams];

  string line, firstWord, blockType;
  while (std::getline(modelFileStream, line))
  {
    // Retrieving first word
    std::istringstream iss(line);
    if (!(iss >> firstWord))
    {
      break;
    }

    // If it is the start of a block
    if (firstWord == "Block")
    {
      iss >> blockType;
      // Fills the y array with info from that specific block.
      fill_y(y, blockType, modelFileStream);
    }
  }

  set_model_from_y(y);
  calc_kF_rF_ckm_from_etaF();

  return calc_treeLvl_masses_and_mixings();
}

void THDM::fill_y(double y[], const string &blockType,
                  std::ifstream &ifStream)
{
  if (blockType == "THDME")
  {
    _renormScale = readLine(ifStream, 2);
    _z2_symmetry = (Z2symmetry)readLine(ifStream, 2);
  }
  if (blockType == "MINPAR")
  {
    y[120] = readLine(ifStream, 2); // Lambda1
    y[121] = readLine(ifStream, 2); // Lambda2
    y[122] = readLine(ifStream, 2); // Lambda3
    y[123] = readLine(ifStream, 2); // Lambda4
    y[124] = readLine(ifStream, 2); // real(Lambda5)
    y[126] = readLine(ifStream, 2); // real(Lambda6)
    y[128] = readLine(ifStream, 2); // real(Lambda7)
    y[118] = readLine(ifStream, 2); // real(M12)
    y[7] = readLine(ifStream, 2);   // tanb
  }
  else if (blockType == "IMMINPAR")
  {
    y[125] = readLine(ifStream, 2); // im(Lambda5Input)
    y[127] = readLine(ifStream, 2); // im(Lambda6Input)
    y[129] = readLine(ifStream, 2); // im(Lambda7Input)
    y[119] = readLine(ifStream, 2); // im(M12input)
  }
  else if (blockType == "MIJ2")
  {
    y[116] = readLine(ifStream, 2); // M112
    y[117] = readLine(ifStream, 2); // M222
  }
  // Real parts of Yukawa matrices
  else if (blockType == "ETA1UIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[8 + i * 3 + j] = readLine(ifStream, 3); // real(eta1U(i,j))
      }
  }
  else if (blockType == "ETA1DIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[26 + i * 3 + j] = readLine(ifStream, 3); // real(eta1D(i,j))
      }
  }
  else if (blockType == "ETA1LIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[44 + i * 3 + j] = readLine(ifStream, 3); // real(eta1L(i,j))
      }
  }
  else if (blockType == "ETA2UIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[62 + i * 3 + j] = readLine(ifStream, 3); // real(eta2U(i,j))
      }
  }
  else if (blockType == "ETA2DIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[80 + i * 3 + j] = readLine(ifStream, 3); // real(eta2D(i,j))
      }
  }
  else if (blockType == "ETA2LIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[98 + i * 3 + j] = readLine(ifStream, 3); // real(eta2L(i,j))
      }
  }
  // Imag parts of Yukawa matrices
  else if (blockType == "IMETA1UIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[17 + i * 3 + j] = readLine(ifStream, 3); // imag(eta1U(i,j))
      }
  }
  else if (blockType == "IMETA1DIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[35 + i * 3 + j] = readLine(ifStream, 3); // imag(eta1D(i,j))
      }
  }
  else if (blockType == "IMETA1LIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[53 + i * 3 + j] = readLine(ifStream, 3); // imag(eta1L(i,j))
      }
  }
  else if (blockType == "IMETA2UIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[71 + i * 3 + j] = readLine(ifStream, 3); // imag(eta2U(i,j))
      }
  }
  else if (blockType == "IMETA2DIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[89 + i * 3 + j] = readLine(ifStream, 3); // imag(eta2D(i,j))
      }
  }
  else if (blockType == "IMETA2LIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[107 + i * 3 + j] = readLine(ifStream, 3); // imag(eta2L(i,j))
      }
  }
  else if (blockType == "HMIXIN")
  {
    y[3] = readLine(ifStream, 2); // real(v1)
    y[5] = readLine(ifStream, 2); // real(v2)
  }
  else if (blockType == "IMHMIXIN")
  {
    y[4] = readLine(ifStream, 2); // imag(v1)
    y[6] = readLine(ifStream, 2); // imag(v2)
  }
  else if (blockType == "GAUGEIN")
  {
    y[0] = readLine(ifStream, 2); // g1
    y[1] = readLine(ifStream, 2); // g2
    y[2] = readLine(ifStream, 2); // g3
  }
}
} // namespace THDME
