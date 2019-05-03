// THDM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: General (complex) 2HDM class
 * @author: Joel Oredsson
 *============================================================================*/
#include "THDM.h"
#include "Globals.h"
#include "HelpFunctions.h"
#include "Oblique.h"
#include "RGE.h"
#include "SLHA.h"

#ifdef GNUPLOT
#include "GnuPlotSystem.h"
#endif

#include <Eigen/Dense>
#include <algorithm>
#include <complex>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h> // MINIMIZER
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute.h>
#include <gsl/gsl_roots.h>
#include <iostream>
#include <string>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

namespace THDME {

/*----------------------------------------------------------------------------*/

THDM::THDM() : _z2_symmetry(NO_SYMMETRY) { init(); }

THDM::THDM(const SM &sm) : _z2_symmetry(NO_SYMMETRY) {
  init();
  set_sm(sm);
}

// Copy-constructor
THDM::THDM(const THDM &thdm) { set_from_thdm(thdm); }

THDM &THDM::operator=(const THDM &thdm) {
  set_from_thdm(thdm);
  return *this;
}

void THDM::set_from_thdm(const THDM &thdm) {
  set_logLevel(thdm.get_logLevel());
  init();

  _renormScale = thdm.get_renormalization_scale();
  set_rgeConfig(thdm.get_rgeConfig());
  set_rgeResults(thdm.get_rgeResults());

  double y[_numParams];
  thdm.set_y(y);
  set_model_from_y(y);

  calc_kF_rF_ckm_from_etaF();
  _z2_symmetry = thdm.get_yukawa_type();

  calc_treeLvl_masses_and_mixings();
  if (thdm._mh_pole[0] > 0) // Copies "pole" masses if they have been calculated
    for (int h = 0; h <= 4; ++h)
      _mh_pole[h] = thdm._mh_pole[h];
}

THDM::~THDM() {
  // std::cout << "Destroying thdm!\n"; // DEBUG
}

bool THDM::set_param_gen(const Base_generic &in, const bool enforceTadpole) {
  clear_calculated_quantities();
  if (in.contains_nan()) {
    _console.error << "[ERROR]: NaN value detected in set_param_gen()!\n";
    return false;
  }

  _base_generic = in;

  if (enforceTadpole)
    if (!fix_treeLvl_tadpole_eqs())
      return false;

  return calc_treeLvl_masses_and_mixings();
}

Base_generic THDM::get_param_gen() const { return _base_generic; }

bool THDM::set_param_higgs(const Base_higgs &in, const bool enforceTadpole) {
  return set_param_gen(in.convert_to_generic(), enforceTadpole);
}

Base_higgs THDM::get_param_higgs() const {
  return _base_generic.convert_to_higgs();
}

// CP invariant and Z2 symmetric basis (all parameters are real)
bool THDM::set_param_hybrid(const Base_hybrid &hyb, const bool enforceTadpole) {
  return set_param_gen(hyb.convert_to_generic(_v2), enforceTadpole);
}

bool THDM::set_param_invariant(const Base_invariant &inv,
                               const bool enforceTadpole) {

  return set_param_gen(inv.convert_to_generic(_v2), enforceTadpole);
}

Base_invariant THDM::get_param_invariant() const {
  return _base_generic.convert_to_invariant(_v2);
}

Base_hybrid THDM::get_param_hybrid() const {
  // This function requires that the THDM is CP conserving and Z2 symmetric.
  if (!is_cp_conserved())
    _console.error
        << "[ERROR]: Can't retrieve hybrid basis from CP violating THDM.\n";

  Base_higgs higgs = _base_generic.convert_to_higgs();

  double mA = std::sqrt(_mHc * _mHc + 0.5 * (higgs.Z4 - real(higgs.Z5)) * _v2);

  double mh, mH, cba;

  constexpr static double MIN_Z6_VALUE = 1e-10;

  if (std::abs(higgs.Z6) > MIN_Z6_VALUE) {
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
  } else {
    double m1 = std::sqrt(higgs.Z1 * _v2);
    double m2 = std::sqrt(mA * mA + real(higgs.Z5) * _v2);

    if (m1 < m2) {
      mh = m1;
      mH = m2;
      cba = 0.;
    } else {
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

// Function to minimize with respect to x(=xi)
double xiTadpoleMin(double x, void *p) {
  struct genStruct *params = (struct genStruct *)p;
  Base_generic genBase = params->gen;
  double v2 = params->v2;
  double sinb = sin(genBase.beta);
  double cosb = std::sqrt(1. - sinb * sinb);
  complex<double> M12 = genBase.M12;
  complex<double> Lambda5 = genBase.Lambda5;
  complex<double> Lambda6 = genBase.Lambda6;
  complex<double> Lambda7 = genBase.Lambda7;

  double result = std::abs(
      ::imag(M12 * std::polar(1., x)) -
      0.5 * v2 * (std::imag(Lambda5 * std::polar(1., 2 * x)) * sinb * cosb +
                  std::imag(Lambda6 * std::polar(1., x)) * cosb * cosb +
                  std::imag(Lambda7 * std::polar(1., x)) * sinb * sinb));
  return result;
}

bool THDM::fix_treeLvl_tadpole_eqs() {
  double sinb = sin(_base_generic.beta);
  double cosb = std::sqrt(1.0 - sinb * sinb);
  double tanb = tan(_base_generic.beta);
  double cotb = 1. / tanb;

  // Tree-lvl minimum from eq.(A4-A5) and eq.(A7) in PhysRevD.72.035004
  if (abs(_base_generic.beta) > _MACHINE_PRECISION) {
    if (tanb < 1.E-3)
      _console.warning
          << "Very low tanb!"
          << "This results in very large masses from tadpole requirements.\n ";

    // We solve eq.(A7) in PhysRevD.72.035004 numerically
    // M12 must be non-zero, otherwise xi is set to zero
    if (std::abs(_base_generic.M12) > 1e-6) {
      // GSL ROOT FINDER =====================================================

      // Function to minimize with respect to x(=xi)
      double (*f)(double, void *) = [](double x, void *p) {
        struct genStruct *params = (struct genStruct *)p;
        Base_generic genBase = params->gen;
        double v2 = params->v2;
        double sinb = sin(genBase.beta);
        double cosb = std::sqrt(1. - sinb * sinb);
        complex<double> M12 = genBase.M12;
        complex<double> Lambda5 = genBase.Lambda5;
        complex<double> Lambda6 = genBase.Lambda6;
        complex<double> Lambda7 = genBase.Lambda7;

        double result =
            std::imag(M12 * std::polar(1., x)) -
            0.5 * v2 *
                (std::imag(Lambda5 * std::polar(1., 2 * x)) * sinb * cosb +
                 std::imag(Lambda6 * std::polar(1., x)) * cosb * cosb +
                 std::imag(Lambda7 * std::polar(1., x)) * sinb * sinb);
        return result;
      };
      double (*df)(double, void *) = [](double x, void *p) {
        struct genStruct *params = (struct genStruct *)p;
        Base_generic genBase = params->gen;
        double v2 = params->v2;
        double sinb = sin(genBase.beta);
        double cosb = std::sqrt(1. - sinb * sinb);
        complex<double> M12 = genBase.M12;
        complex<double> Lambda5 = genBase.Lambda5;
        complex<double> Lambda6 = genBase.Lambda6;
        complex<double> Lambda7 = genBase.Lambda7;

        static const std::complex<double> I(0., 1.);
        double result =
            std::imag(I * M12 * std::polar(1., x)) -
            0.5 * v2 *
                (std::imag(2. * I * Lambda5 * std::polar(1., 2 * x)) * sinb *
                     cosb +
                 std::imag(I * Lambda6 * std::polar(1., x)) * cosb * cosb +
                 std::imag(I * Lambda7 * std::polar(1., x)) * sinb * sinb);
        return result;
      };

      void (*fdf)(double x, void *p, double *f,
                  double *df) = [](double x, void *p, double *f, double *df) {
        struct genStruct *params = (struct genStruct *)p;
        Base_generic genBase = params->gen;
        double v2 = params->v2;
        double sinb = sin(genBase.beta);
        double cosb = std::sqrt(1. - sinb * sinb);
        complex<double> M12 = genBase.M12;
        complex<double> Lambda5 = genBase.Lambda5;
        complex<double> Lambda6 = genBase.Lambda6;
        complex<double> Lambda7 = genBase.Lambda7;
        static const std::complex<double> I(0., 1.);

        *f = std::imag(M12 * std::polar(1., x)) -
             0.5 * v2 *
                 (std::imag(Lambda5 * std::polar(1., 2 * x)) * sinb * cosb +
                  std::imag(Lambda6 * std::polar(1., x)) * cosb * cosb +
                  std::imag(Lambda7 * std::polar(1., x)) * sinb * sinb);
        *df = std::imag(I * M12 * std::polar(1., x)) -
              0.5 * v2 *
                  (std::imag(2. * I * Lambda5 * std::polar(1., 2 * x)) * sinb *
                       cosb +
                   std::imag(I * Lambda6 * std::polar(1., x)) * cosb * cosb +
                   std::imag(I * Lambda7 * std::polar(1., x)) * sinb * sinb);
      };

      // Creating gsl_function to minimize
      gsl_function_fdf F;
      F.f = f;
      F.df = df;
      F.fdf = fdf;
      genStruct genBasis = {_v2, _base_generic};
      F.params = &genBasis;

      gsl_set_error_handler_off();
      const gsl_root_fdfsolver_type *T = gsl_root_fdfsolver_newton;
      gsl_root_fdfsolver *s = gsl_root_fdfsolver_alloc(T);

      double x = 0.;
      int gslStatus = gsl_root_fdfsolver_set(s, &F, x);

      if (gslStatus) {
        _console.debug
            << "[ERROR]: \"gsl_root_fsolver_set\" failed when fixing tadpole "
               "eqs. gsl_errno="
            << gslStatus << endl;
        _console.debug << "Message: " << gsl_strerror(gslStatus) << endl;
        gsl_root_fdfsolver_free(s);
        return false;
      }

      int status;
      int iter = 0, max_iter = 200;

      double x0 = 0.;

      do {
        iter++;

        status = gsl_root_fdfsolver_iterate(s);
        x0 = x;
        x = gsl_root_fdfsolver_root(s);

        status = gsl_root_test_delta(x, x0, 0, 1e-3);

      } while (status == GSL_CONTINUE && iter < max_iter);

      gsl_root_fdfsolver_free(s);

      _base_generic.xi = fmod(x, 2. * M_PI);
      // Setting xi to zero if it is below threshold.
      static constexpr double MIN_XI = 1e-15;
      if (abs(_base_generic.xi) < MIN_XI)
        _base_generic.xi = 0.;

      if (std::abs(F.f(x, F.params)) > 1e-6) {
        _console.debug
            << "[ERROR]: Failed to set  tadpole eqs with high precision.\n";
        // printf("xi = %16.8e, f(xi) = %16.8e\n", x, F.f(x, F.params));
        return false;
      }
    } else
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

    _base_generic.M222 = real(
        conj(_base_generic.M12 * std::polar(1., _base_generic.xi)) * cotb -
        0.5 * _v2 * (_base_generic.Lambda2 * sinb * sinb +
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
  } // namespace THDME
  else {
    _console.warning
        << "[WARNING]: using Higgs basis tadpole equation because beta=0!\n";

    _base_generic.M112 = -0.5 * _base_generic.Lambda1 * _v2;
    _base_generic.M12 = -0.5 * _base_generic.Lambda6 * _v2;
  }

  return true;
}

bool THDM::set_tanb(const double &tanb) {
  _base_generic.beta = atan(tanb);
  return set_param_gen(_base_generic);
}

void THDM::chi_phase_shift(const double &chi) {
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

bool THDM::rge_update(const double y[], const double ti) {
  // Creates data files with the parameters as function of energy.
  if (!_rgeResults.evolved) {
    if (_rgeConfig.dataOutput) {
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
  for (int i = 0; i < 3; i++) {
    if (std::isnan(_mh[i])) {
      _console.warning << "[WARNING]: NaN value detected in mh[" << i
                       << "] = " << _mh[i] << " at " << exp(ti) << " GeV !!!\n";
      return false;
    }
  }

  return true;
}

void THDM::rge_finished() {

  if (_rgeConfig.dataOutput) {
    _files.close_streams();

#ifdef GNUPLOT
    _console.info << "Running GNUPLOT!\n";
    GnuPlotSystem gnuSys(_files.get_directory());
    gnuSys.set_z2_symmetry(_z2_symmetry);
    gnuSys.create_pdf_plots(_rgeConfig, _rgeResults);
#endif
  }
}

void THDM::set_y(double y[]) const {
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
    for (int j = 0; j < 3; j++) {
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

void THDM::set_model_from_y(const double y[]) {
  _g1 = y[0];
  _g2 = y[1];
  _g3 = y[2];

  complex<double> v1C = complex<double>(y[3], y[4]);
  complex<double> v2C = complex<double>(y[5], y[6]);
  _v2 = norm(v1C) + norm(v2C);
  _base_generic.beta = atan(std::abs(v2C) / std::abs(v1C));
  _base_generic.xi = arg(v2C);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
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

void THDM::set_yukawa_type(const Z2symmetry &z2) {
  clear_calculated_quantities();
  _z2_symmetry = z2;

  // Sets the kappa matrices to be diagonal proportional to the fermion masses.
  set_kF_from_fermion_masses();

  // Can't set Z2 symmetry if beta = 0.
  if (z2 != NO_SYMMETRY) {
    if (_base_generic.beta < _MACHINE_PRECISION) {
      _console.error << "[ERROR]: Can't set Yukawa symmetry because tanb is "
                        "too small; beta = "
                     << _base_generic.beta << std::endl;
      _z2_symmetry = NO_SYMMETRY;
    }
  }

  switch (_z2_symmetry) {
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
                              const std::complex<double> &aL) {
  clear_calculated_quantities();
  _z2_symmetry = NO_SYMMETRY;

  // Sets the kappa matrices to be diagonal proportional to the fermion masses.
  set_kF_from_fermion_masses();

  set_yukawa_rho(aU * _kU, aD * _kD, aL * _kL);

  calc_eta_from_kF_rF_ckm();
}

vector<complex<double>> THDM::get_aF() const {
  vector<complex<double>> aFVec = {_rU(2, 2) / _kU(2, 2), _rD(2, 2) / _kD(2, 2),
                                   _rL(2, 2) / _kL(2, 2)};

  return aFVec;
}

void THDM::set_yukawa_rho(const Eigen::Matrix3cd &rU,
                          const Eigen::Matrix3cd &rD,
                          const Eigen::Matrix3cd &rL) {
  clear_calculated_quantities();
  _rU = rU;
  _rD = rD;
  _rL = rL;
}

void THDM::set_yukawa_manual(const Eigen::Matrix3cd &rU,
                             const Eigen::Matrix3cd &rD,
                             const Eigen::Matrix3cd &rL) {
  clear_calculated_quantities();
  if (_z2_symmetry != NO_SYMMETRY) {
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
                          const Eigen::Matrix3cd &eta2L) {
  if (_z2_symmetry != NO_SYMMETRY) {
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

bool THDM::is_perturbative() const {
  double maxPert = 4. * M_PI;
  if (_base_generic.Lambda1 > maxPert || _base_generic.Lambda2 > maxPert ||
      _base_generic.Lambda3 > maxPert || _base_generic.Lambda4 > maxPert ||
      std::abs(_base_generic.Lambda5) > maxPert ||
      std::abs(_base_generic.Lambda6) > maxPert ||
      std::abs(_base_generic.Lambda7) > maxPert) {
    return false;
  }
  return true;
}

bool THDM::is_unitary() const {
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
  for (int i = 0; i < 3; i++) {
    if (std::abs(eigenS21.eigenvalues()[i]) > unitarityBound)
      unitary = false;
  }
  for (int i = 0; i < 4; i++) {
    if (std::abs(eigenS01.eigenvalues()[i]) > unitarityBound)
      unitary = false;
    if (std::abs(eigenS00.eigenvalues()[i]) > unitarityBound)
      unitary = false;
  }
  return unitary;
}

bool THDM::is_stable_z2sym() const {
  if (_base_generic.Lambda1 < 0. || _base_generic.Lambda2 < 0. ||
      _base_generic.Lambda3 <
          -sqrt(std::abs(_base_generic.Lambda1 * _base_generic.Lambda2)) ||
      _base_generic.Lambda3 + _base_generic.Lambda4 -
              std::abs(_base_generic.Lambda5) <
          -sqrt(std::abs(_base_generic.Lambda1 * _base_generic.Lambda2))) {
    return false;
  }
  return true;
}
bool THDM::is_stable() const {

  Eigen::Matrix4cd LE(4, 4);
  Eigen::Matrix4cd mink(4, 4);
  LE.setZero();
  mink.setZero();

  for (int i = 0; i < 4; i++) {
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
  for (int i = 0; i < 4; i++) {
    if (abs(imag(eigenValues[i])) > 1e-10) {
      return false;
    }
  }
  // Calculates the maximum eigenvalue
  double maxEigen = real(eigenValues[0]);
  int maxID = 0;
  for (int i = 1; i < 4; i++) {
    if (real(eigenValues[i]) > maxEigen) {
      maxEigen = real(eigenValues[i]);
      maxID = i;
    }
  }

  // 2) Check that the largest one is positive
  if (maxEigen < 0) {
    return false;
  }

  // 3) Check linear independence
  if (std::abs(eigenVectors.determinant()) < 0.001) {
    return false;
  }
  // 4) Check that the largest eigenvector is timelike and the others spacelike
  Eigen::Matrix4cd VgVdagger = eigenVectors * mink * eigenVectors.adjoint();
  for (int i = 0; i < 4; i++) {
    if (i != maxID) {
      if (real(VgVdagger(i, i)) > 0)
        return false;
    }
  }
  if (real(VgVdagger(maxID, maxID)) < 0)
    return false;
  return true;
}

bool THDM::is_cp_conserved() const {
  /* The Higgs scalar potential and vacuum is CP conserving iff eq.3.1 in
    arXiv:1011.6188v2 is satisfied. We calculate these three imaginary parts of
    combinations of quartic couplings in the Higgs basis here. */

  Base_higgs higgs = _base_generic.convert_to_higgs();
  double imag56 = abs(imag(conj(higgs.Z5) * higgs.Z6 * higgs.Z6));
  double imag57 = abs(imag(conj(higgs.Z5) * higgs.Z7 * higgs.Z7));
  double imag67 = abs(imag(conj(higgs.Z6) * higgs.Z7));

  static const double ANGLE_PRECISION = 1.E-6;

  if (imag56 > ANGLE_PRECISION || imag57 > ANGLE_PRECISION ||
      imag67 > ANGLE_PRECISION) {
    _console.debug << "The model is CP violating:\n";
    _console.debug << "imag(Z5* Z6^2) = " << imag56 << std::endl;
    _console.debug << "imag(Z5* Z7^2) = " << imag57 << std::endl;
    _console.debug << "imag(Z6* Z7) = " << imag67 << std::endl;
    return false;
  }

  return true;
}

bool THDM::enforce_z2Symmetric_potential(const bool soft) {

  if (!soft)
    _base_generic.M12 = complex<double>(0., 0.);

  _base_generic.Lambda6 = complex<double>(0., 0.);
  _base_generic.Lambda7 = complex<double>(0., 0.);

  fix_treeLvl_tadpole_eqs();

  // calc_compact_from_generic();
  // return calc_higgs_from_compact();
  return true;
}

bool THDM::delete_imag_parts() {
  _base_generic.M12 = complex<double>(real(_base_generic.M12), 0.);
  _base_generic.Lambda5 = complex<double>(real(_base_generic.Lambda5), 0.);
  _base_generic.Lambda6 = complex<double>(real(_base_generic.Lambda6), 0.);
  _base_generic.Lambda7 = complex<double>(real(_base_generic.Lambda7), 0.);

  fix_treeLvl_tadpole_eqs();

  return true;
}

bool THDM::is_pert_unit_stab() const {
  if (!is_perturbative()) {
    _console.warning << "[WARNING]: Perturbativity is violated!!\n";
    return false;
  }
  if (!is_unitary()) {
    _console.warning << "[WARNING]: Unitarity is violated!\n";
    return false;
  }
  if (!is_stable()) {
    _console.warning << "[WARNING]: Stability is violated!\n";
    return false;
  }

  /* Checks for imaginary masses. */
  for (int i = 0; i < 3; i++) {
    if (std::isnan(_mh[i]))
      return false;
  }
  if (std::isnan(_mHc))
    return false;

  _console.info << "Model is okay!\n";
  return true;
}

void THDM::calc_vev_tensors() {
  _vhat[0] = complex<double>(cos(_base_generic.beta), 0.);
  _vhat[1] = sin(_base_generic.beta) * std::polar(1., _base_generic.xi);
  _what[0] = -sin(_base_generic.beta) * std::polar(1., -_base_generic.xi);
  _what[1] = complex<double>(cos(_base_generic.beta), 0.);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      _V[i][j] = _vhat[i] * conj(_vhat[j]);
      _W[i][j] = _what[i] * conj(_what[j]);
    }
  }
}

void THDM::calc_eta_from_kF_rF_ckm() {
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

void THDM::calc_kF_rF_ckm_from_etaF() {
  calc_vev_tensors();

  _kU = conj(_vhat[0]) * _eta1U + conj(_vhat[1]) * _eta2U;
  _kD = _vhat[0] * _eta1D + _vhat[1] * _eta2D;
  _kL = _vhat[0] * _eta1L + _vhat[1] * _eta2L;
  _rU = conj(_what[0]) * _eta1U + conj(_what[1]) * _eta2U;
  _rD = _what[0] * _eta1D + _what[1] * _eta2D;
  _rL = _what[0] * _eta1L + _what[1] * _eta2L;

  BiUnitary(_kU, _kD, _kL, _rU, _rD, _rL, _VCKM, _mup, _mdn, _ml, _v2);
}

void THDM::print_features() const {
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

void THDM::calc_rgeResults() {
  THDM thdm(*this);
  //  thdm.print_all();
  thdm.set_logLevel(LOG_ERRORS);
  thdm.evolve();
  _rgeResults = thdm.get_rgeResults();
}

void THDM::print_all() const {
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
  print_oblique();
  print_CKM();
  print_yukawa();

  std::cout << "\n"
               "------------------------------------------------------------\n";
}

void THDM::print_oblique() const {
  std::vector<double> mi = get_higgs_treeLvl_masses();
  std::vector<double> qi12 = get_qi12();
  std::vector<double> qi22 = get_qi22();
  double s = calcS(mi, qi12);
  double t = calcT(mi, qi12, qi22);
  double u = calcU(mi, qi12, qi22);

  std::cout << "Oblique parameters:\n";
  for (auto &m : mi)
    if (m < 46.) {
      std::cout << " [WARNING]: Low Higgs masses!\n";
      std::cout << "            Calculation is not reliable.\n";
      break;
    }
  std::cout << "S = " << s << std::endl;
  std::cout << "T = " << t << std::endl;
  std::cout << "U = " << u << std::endl;
}

std::vector<double> THDM::get_oblique() const {
  std::vector<double> mi = get_higgs_treeLvl_masses();
  std::vector<double> qi12 = get_qi12();
  std::vector<double> qi22 = get_qi22();
  double s = calcS(mi, qi12);
  double t = calcT(mi, qi12, qi22);
  double u = calcU(mi, qi12, qi22);

  return std::vector<double>{s, t, u};
}

void THDM::print_potential() const {

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
  tab.add_row(vector<string>{"M_11^2 =", stringAuto(_base_generic.M112),
                             "Y1 =", stringAuto(higgs.Y1)});
  tab.add_row(vector<string>{"M_22^2 =", stringAuto(_base_generic.M222),
                             "Y2 =", stringAuto(higgs.Y2)});
  tab.add_row(vector<string>{"M_12^2 =", stringAuto(_base_generic.M12),
                             "Y3 =", stringAuto(higgs.Y3)});
  tab.add_row(vector<string>{"Lambda_1 =", stringAuto(_base_generic.Lambda1),
                             "Z1 =", stringAuto(higgs.Z1)});
  tab.add_row(vector<string>{"Lambda_2 =", stringAuto(_base_generic.Lambda2),
                             "Z2 =", stringAuto(higgs.Z2)});
  tab.add_row(vector<string>{"Lambda_3 =", stringAuto(_base_generic.Lambda3),
                             "Z3 =", stringAuto(higgs.Z3)});
  tab.add_row(vector<string>{"Lambda_4 =", stringAuto(_base_generic.Lambda4),
                             "Z4 =", stringAuto(higgs.Z4)});
  tab.add_row(vector<string>{"Lambda_5 =", stringAuto(_base_generic.Lambda5),
                             "Z5 =", stringAuto(higgs.Z5)});
  tab.add_row(vector<string>{"Lambda_6 =", stringAuto(_base_generic.Lambda6),
                             "Z6 =", stringAuto(higgs.Z6)});
  tab.add_row(vector<string>{"Lambda_7 =", stringAuto(_base_generic.Lambda7),
                             "Z7 =", stringAuto(higgs.Z7)});
  tab.print();

  // _console.info << inv << std::endl;
}

void THDM::print_z2Symmetry() const {
  std::cout << "Yukawa Z_2 symmetry: ";
  switch (_z2_symmetry) {
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

void THDM::print_higgs_treeLvl_masses() const {
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Table tab(4);
  tab.set_frame_style("-", "|");
  tab.set_title("Tree-lvl masses @ Q=" + stringAuto(_renormScale));
  tab.add_row({"", "[GeV]", "Mixing angles", ""}, true);
  tab.add_row(vector<string>{"m_h1:", stringAuto(_mh[0], "%5.1f"),
                             "s12:", stringAuto(inv.s12)});
  tab.add_row(vector<string>{"m_h2:", stringAuto(_mh[1], "%5.1f"),
                             "c13:", stringAuto(inv.c13)});
  tab.add_row(vector<string>{"m_h3:", stringAuto(_mh[2], "%5.1f"),
                             "cPhi:", stringAuto(inv.cPhi)});
  tab.add_row(vector<string>{"m_Hc:", stringAuto(_mHc, "%5.1f"), "", ""});
  tab.print();
  cout << "\n";
}
void THDM::print_higgs_masses() const {
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Table tab(4);
  tab.set_frame_style("-", "|");
  if (_mh_pole[0] > 0) {
    tab.set_title("\"pole\" tree-lvl masses");
    tab.add_row({"", "[GeV]", "Mixing angles", ""}, true);
    tab.add_row(vector<string>{"m_h1:", stringAuto(_mh_pole[1], "%5.1f"),
                               "s12:", stringAuto(inv.s12)});
    tab.add_row(vector<string>{"m_h2:", stringAuto(_mh_pole[2], "%5.1f"),
                               "c13:", stringAuto(inv.c13)});
    tab.add_row(vector<string>{"m_h3:", stringAuto(_mh_pole[3], "%5.1f"),
                               "cPhi:", stringAuto(inv.cPhi)});
    tab.add_row(
        vector<string>{"m_Hc:", stringAuto(_mh_pole[4], "%5.1f"), "", ""});
  } else {
    tab.set_title("Tree-lvl masses @ Q=" + stringAuto(_renormScale));
    tab.add_row({"", "[GeV]", "Mixing angles", ""}, true);
    tab.add_row(vector<string>{"m_h1:", stringAuto(_mh[0], "%5.1f"),
                               "s12:", stringAuto(inv.s12)});
    tab.add_row(vector<string>{"m_h2:", stringAuto(_mh[1], "%5.1f"),
                               "c13:", stringAuto(inv.c13)});
    tab.add_row(vector<string>{"m_h3:", stringAuto(_mh[2], "%5.1f"),
                               "cPhi:", stringAuto(inv.cPhi)});
    tab.add_row(vector<string>{"m_Hc:", stringAuto(_mHc, "%5.1f"), "", ""});
  }
  tab.print();
  cout << "\n";
}

void THDM::print_fermion_masses() const {
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

void THDM::print_neutral_higgs_eigenVectors() const {
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);
  Eigen::Matrix3d R = inv.get_R();

  Table tabEigenstate(4);
  tabEigenstate.set_frame_style("-", "|");
  tabEigenstate.add_row(
      vector<string>{"Eigenvector", "phi_1^0", "phi_2^0", "a^0"}, true);
  tabEigenstate.add_row(vector<string>{
      "h1:", stringAuto(R(0, 0)), stringAuto(R(0, 1)), stringAuto(R(0, 2))});
  tabEigenstate.add_row(vector<string>{
      "h2:", stringAuto(R(1, 0)), stringAuto(R(1, 1)), stringAuto(R(1, 2))});
  tabEigenstate.add_row(vector<string>{
      "h3:", stringAuto(R(2, 0)), stringAuto(R(2, 1)), stringAuto(R(2, 2))});
  tabEigenstate.print();
  cout << "\n";
}

bool THDM::calc_treeLvl_masses_and_mixings() {
  // 9 april 2019: Dont know why I have been doing different cases with Z5=0
  // etc.
  // Now this functions simply calculates the ordered masses from the mass
  // matrix
  // in the Higgs basis.
  Base_higgs higgs = _base_generic.convert_to_higgs();

  // constexpr static double MIN_Z = 1e-10;

  // // The general case where all neutral Higgs fields mix
  // if (std::abs(higgs.Z6) > MIN_Z)
  // {

  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> M = EigenSolver(_v2, higgs);

  // M gives ordered eigenvalues by increasing size.
  for (int i = 0; i < 3; i++) {
    if (M.eigenvalues()[i] < 0 || std::isnan(M.eigenvalues()[i])) {
      _console.debug << "[DEBUG]: negative eigenvalues in mass matrix, (mh["
                     << i << "]^2 = " << M.eigenvalues()[i] << " GeV^2). \n";
      _mh[i] = 0.;
      return false;
    } else
      _mh[i] = sqrt(M.eigenvalues()[i]);
  }
  // }
  // else if (std::abs(real(higgs.Z5)) < MIN_Z && std::abs(imag(higgs.Z5)) <
  // MIN_Z) // Z5=Z6=0
  // {
  //   double m1 = sqrt(_v2 * higgs.Z1);
  //   double m2 = sqrt(higgs.Y2 + 0.5 * (higgs.Z3 + higgs.Z4) * _v2);
  //   _mh[0] = m1 < m2 ? m1 : m2;
  //   _mh[1] = m1 < m2 ? m2 : m1;
  //   _mh[2] = m1 < m2 ? m2 : m1;
  // }
  // // Case of Z5 neq 0, Z6 = 0
  // else
  // {
  //   // TODO
  //   _console.error << "[ERROR]: The case of Z5 not equal to 0, Z6=0 has not "
  //                       "been implemented yet :( \n";
  // }

  // Charged Higgs:
  if (higgs.Y2 + 0.5 * higgs.Z3 * _v2 < 0) {
    _console.debug << "[DEBUG]: Encountered negative mass for charged Higgs!\n";
    _mHc = 0.;
    return false;
  } else
    _mHc = sqrt(higgs.Y2 + 0.5 * higgs.Z3 * _v2);

  return true;
}

void THDM::reset_higgs_pole_masses() {
  _mh_pole[0] = -1;
  for (int h = 1; h <= 4; ++h)
    _mh_pole[h] = 0.;
}
bool THDM::calc_higgs_pole_masses() {
  // Timer timer("calc_higgs_pole_masses"); // DEBUG
  _mh_pole[0] = -1.;

  for (int h = 1; h <= 4; ++h)
    _mh_pole[h] = 0.;

  // Creating copy of model for iterative evolution.
  THDM model(*this);
  RgeConfig options;
  options.dataOutput = false;
  options.consoleOutput = false;
  options.evolutionName = "temp";
  options.twoloop = true;
  options.perturbativity = true;
  options.stability = true;
  options.unitarity = true;
  options.steps = 1;
  model.set_logLevel(LOG_ERRORS);
  model.set_rgeConfig(options);

  // Finding all Higgs masses that satisfies mh(mu)=mu +/- DELTA_M
  // Returns false if it breaks pert/unit/stab or makes too many iterations.
  constexpr static double DELTA_M = 1.;
  for (int h = 1; h <= 4; ++h) {
    int tries = 0;
    constexpr static int MAX_TRIES = 100;
    for (;;) {
      double mh = model.get_higgs_treeLvl_masses()[h - 1];
      model.evolve_to(mh);
      if (!model.get_rgeResults().is_okay()) {
        _console.error << "[ERROR]: Ran into trouble when evolving model in "
                          "calc_higgs_pole_masses().\n";
        // if (_console.get_logLevel() >= LOG_INFO)
        model.get_rgeResults().print();
        return false;
      }

      mh = model.get_higgs_treeLvl_masses()[h - 1];
      double renormScale = model.get_renormalization_scale();
      double diff = std::abs(renormScale - mh);
      if (diff < DELTA_M) {
        _mh_pole[h] = mh;
        break;
      }
      if (++tries > MAX_TRIES) {
        _console.error << "[ERROR]: Ran into trouble when evolving model in "
                          "calc_higgs_pole_masses().\n";
        _console.info << "         Couldn't find the pole mass after " << tries
                      << " tries.\n";
        return false;
      }
    }
  }
  _mh_pole[0] = 1.; // Denotes that the pole masses are calculated

  return true;
}

std::vector<double> THDM::get_higgs_pole_masses() {
  if (_mh_pole[0] > 0)
    return std::vector<double>{_mh_pole[0], _mh_pole[1], _mh_pole[2],
                               _mh_pole[3], _mh_pole[4]};
  else {
    calc_higgs_pole_masses();
    return std::vector<double>{_mh_pole[0], _mh_pole[1], _mh_pole[2],
                               _mh_pole[3], _mh_pole[4]};
  }
}

double THDM::get_hmass(int h) const {
  if (h < 1 || h > 4) {
    _console.error << "[ERROR]: Wrong argument in get_hmass().\n";
    return 0.;
  }

  // If the pole masses have been calculated, use them.
  if (_mh_pole[0] > 0)
    return _mh_pole[h];

  if (h == 4)
    return _mHc;
  else
    return _mh[h - 1];
}

vector<double> THDM::get_higgs_treeLvl_masses() const {
  return vector<double>{_mh[0], _mh[1], _mh[2], _mHc};
}

complex<double> THDM::get_z2_breaking_quantity() const {
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

std::tuple<string, int, complex<double>> THDM::get_largest_diagonal_rF() const {
  complex<double> maxYuk(0., 0.);
  int maxID[2] = {0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i) {
      if (abs((*rF[k])(i, i)) > abs(maxYuk)) {
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
THDM::get_largest_nonDiagonal_rF() const {
  complex<double> maxYuk(0., 0.);
  int maxID[3] = {0, 0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        if (i != j && abs((*rF[k])(i, j)) > abs(maxYuk)) {
          maxYuk = (*rF[k])(i, j);
          maxID[0] = k;
          maxID[1] = i;
          maxID[2] = j;
        }
      }

  vector<string> rFStrings = {"rU", "rD", "rL"};
  if (maxID[0] == 0 && maxID[1] == 0 && maxID[2] == 0) {
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
THDM::get_largest_nonDiagonal_lamF() const {
  complex<double> maxYuk(0., 0.);
  int maxID[3] = {0, 0, 0};
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};
  vector<const double *> mF = {_mup, _mdn, _ml};
  double v = sqrt(_v2);

  for (int k = 0; k < 3; ++k)
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        double lambdaFij =
            v * abs((*rF[k])(i, j)) / sqrt(2. * mF[k][i] * mF[k][j]);

        if (i != j && abs((*rF[k])(i, j)) > abs(maxYuk)) {
          maxYuk = lambdaFij;
          maxID[0] = k;
          maxID[1] = i;
          maxID[2] = j;
        }
      }

  vector<string> rFStrings = {"lamU", "lamD", "lamL"};
  if (maxID[0] == 0 && maxID[1] == 0 && maxID[2] == 0) {
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
                              const int j) const {
  vector<const Eigen::Matrix3cd *> rF = {&_rU, &_rD, &_rL};
  vector<const double *> mF = {_mup, _mdn, _ml};
  double v = sqrt(_v2);

  return v * abs((*rF[(int)flavor])(i, j)) /
         sqrt(2. * mF[(int)flavor][i] * mF[(int)flavor][j]);
}

Eigen::Matrix3cd THDM::get_lamF(const FermionSector flavor) const {
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

std::tuple<string, int, double> THDM::get_largest_lambda() const {
  double maxLambda = 0.;
  int maxID = 0;

  vector<double> absLambda = {
      abs(_base_generic.Lambda1), abs(_base_generic.Lambda2),
      abs(_base_generic.Lambda3), abs(_base_generic.Lambda4),
      abs(_base_generic.Lambda5), abs(_base_generic.Lambda6),
      abs(_base_generic.Lambda7)};

  for (unsigned int i = 0; i < absLambda.size(); ++i) {
    if (absLambda[i] > maxLambda) {
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

complex<double> THDM::get_qki(int k, int i) const {

  if ((k < 1) || (k > 4) || (i < 1) || (i > 2)) {
    return complex<double>(0., 0.);
  }

  Base_invariant inv = _base_generic.convert_to_invariant(_v2);

  double s13 = -sqrt(abs(1. - inv.c13 * inv.c13));
  double c12 = sqrt(abs(1. - inv.s12 * inv.s12));

  std::vector<std::complex<double>> q;

  q.push_back(std::complex<double>(inv.c13 * c12, 0.));     // q_11
  q.push_back(std::complex<double>(inv.c13 * inv.s12, 0.)); // q_21
  q.push_back(std::complex<double>(s13, 0.));               // q_31
  q.push_back(std::complex<double>(0., 1.));                // q_41
  q.push_back(std::complex<double>(-inv.s12, -c12 * s13));  // q_12
  q.push_back(std::complex<double>(c12, -inv.s12 * s13));   // q_22
  q.push_back(std::complex<double>(0., inv.c13));           // q_32
  q.push_back(std::complex<double>(0., 0.));                // q_42

  return q[(k - 1) + 4 * (i - 1)];
}

std::vector<std::complex<double>> THDM::get_qij() const {
  Base_invariant inv = _base_generic.convert_to_invariant(_v2);

  double s13 = -sqrt(abs(1. - inv.c13 * inv.c13));
  double c12 = sqrt(abs(1. - inv.s12 * inv.s12));
  // Not needed, but here for completeness:
  // Base_higgs higgs = _base_generic.convert_to_higgs();
  // double c23 = cos(arg(higgs.Z6) - acos(inv.cPhi));
  // double s23 = sqrt(abs(1. - c23 * c23));

  std::vector<std::complex<double>> q;

  q.push_back(std::complex<double>(inv.c13 * c12, 0.));     // q_11
  q.push_back(std::complex<double>(inv.c13 * inv.s12, 0.)); // q_21
  q.push_back(std::complex<double>(s13, 0.));               // q_31
  q.push_back(std::complex<double>(0., 1.));                // q_41
  q.push_back(std::complex<double>(-inv.s12, -c12 * s13));  // q_12
  q.push_back(std::complex<double>(c12, -inv.s12 * s13));   // q_22
  q.push_back(std::complex<double>(0., inv.c13));           // q_32
  q.push_back(std::complex<double>(0., 0.));                // q_42

  return q;
}

std::vector<double> THDM::get_qi12() const {
  std::vector<std::complex<double>> q = get_qij();

  std::vector<double> qi12;

  for (int i = 0; i < 4; ++i) {
    qi12.push_back(std::norm(q[i]));
  }
  return qi12;
}

std::vector<double> THDM::get_qi22() const {
  std::vector<std::complex<double>> q = get_qij();

  std::vector<double> qi22;

  for (int i = 4; i < 8; ++i) {
    qi22.push_back(std::norm(q[i]));
  }
  return qi22;
}

Eigen::Matrix3cd THDM::get_huu(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gV_huu;
  gV_huu.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gV_huu;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);

  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gV_huu(j, k) =
          prefactor * (R(i - 1, 0) * real(_kU(j, k)) +
                       real((R(i - 1, 1) - I * R(i - 1, 2)) * _rU(j, k)));
    }
  return gV_huu;
}

Eigen::Matrix3cd THDM::get_huu_axial(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gA_huu;
  gA_huu.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gA_huu;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);
  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gA_huu(j, k) =
          prefactor * (imag((R(i - 1, 1) - I * R(i - 1, 2)) * _rU(j, k)));
    }
  return gA_huu;
}

Eigen::Matrix3cd THDM::get_hdd(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gV_hdd;
  gV_hdd.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gV_hdd;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);

  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gV_hdd(j, k) =
          prefactor * (R(i - 1, 0) * real(_kD(j, k)) +
                       real((R(i - 1, 1) + I * R(i - 1, 2)) * _rD(j, k)));
    }
  return gV_hdd;
}

Eigen::Matrix3cd THDM::get_hdd_axial(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gA_hdd;
  gA_hdd.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gA_hdd;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);
  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gA_hdd(j, k) =
          prefactor * (imag((R(i - 1, 1) + I * R(i - 1, 2)) * _rD(j, k)));
    }
  return gA_hdd;
}

Eigen::Matrix3cd THDM::get_hll(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gV_hll;
  gV_hll.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gV_hll;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);
  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gV_hll(j, k) =
          prefactor * (R(i - 1, 0) * real(_kL(j, k)) +
                       real((R(i - 1, 1) + I * R(i - 1, 2)) * _rL(j, k)));
    }
  return gV_hll;
}

Eigen::Matrix3cd THDM::get_hll_axial(int i) const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  Eigen::Matrix3cd gA_hll;
  gA_hll.setZero();
  if (i > 3 || i < 1) {
    _console.error
        << "[ERROR]: Wrong argument when retrieving fermion coupling.\n";
    return gA_hll;
  }

  static const std::complex<double> I(0., 1.);
  static const std::complex<double> prefactor = -I / std::sqrt(2);
  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k) {
      gA_hll(j, k) =
          prefactor * (imag((R(i - 1, 1) + I * R(i - 1, 2)) * _rL(j, k)));
    }
  return gA_hll;
}

std::vector<Eigen::Matrix3cd> THDM::get_fermion_couplings() const {
  std::vector<Eigen::Matrix3cd> couplingMatrices;

  for (int h = 1; h <= 3; ++h) {
    couplingMatrices.push_back(get_huu(h));
    couplingMatrices.push_back(get_huu_axial(h));
    couplingMatrices.push_back(get_hdd(h));
    couplingMatrices.push_back(get_hdd_axial(h));
    couplingMatrices.push_back(get_hll(h));
    couplingMatrices.push_back(get_hll_axial(h));
  }
  return couplingMatrices;
}

double THDM::get_mf(FermionSector fermion, int generation) const {
  if (generation > 3 || generation < 1) {
    _console.error << "[ERROR]: Wrong argument in get_mf.\n";
    return 0.;
  }
  switch (fermion) {
  case UP: {
    return _mup[generation - 1];
  }
  case DOWN: {
    return _mdn[generation - 1];
  }
  case LEPTON: {
    return _ml[generation - 1];
  }
  default:
    return 0.;
  }
}
std::vector<double> THDM::get_VV_couplings() const {
  auto R = get_rotation_matrix(_v2, get_param_higgs());
  return std::vector<double>{R(0, 0), R(1, 0), R(2, 0)};
}

complex<double> THDM::get_hvv(int h, int v) const {
  auto a_i = get_VV_couplings();

  if (h < 1 || h > 3 || v < 1 || v > 3) {
    _console.error << "[ERROR]: Wrong argument in get_hvv, h = " << h
                   << ", v = " << v << "\n";
    return 0.;
  }
  if (v == 3) {
    double mW2 = 0.25 * _v2 * _g2 * _g2;
    return complex<double>(0., 2. * a_i[h - 1] * mW2 / sqrt(_v2));
  } else if (v == 2) {
    double mZ2 = 0.5 * _v2 * (_g1 * _g1 + _g2 * _g2);
    return complex<double>(0., a_i[h - 1] * mZ2 / sqrt(_v2));
  }
  return 0.;
}
void THDM::get_coupling_vvh(int v1, int v2, int h, complex<double> &c) const {
  if (v1 == v2)
    c = get_hvv(h, v1);
  else
    c = 0.;
}

complex<double> THDM::get_coupling_hhh(int h1, int h2, int h3) const {
  // Conventions are according to hep-ph/0602242
  complex<double> I(0.0, 1.0);

  Base_invariant inv = get_param_invariant();
  Base_higgs higgs = get_param_higgs();
  double Z1 = higgs.Z1;
  double Z3 = higgs.Z3;
  double Z4 = higgs.Z4;
  complex<double> Z5 = higgs.Z5 * std::polar(1., -2. * inv.theta23);
  complex<double> Z6 = higgs.Z6 * std::polar(1., -inv.theta23);
  complex<double> Z7 = inv.Z7inv;
  double v = sqrt(_v2);

  complex<double> c = 0;
  if (h1 > h2) {
    int tmp = h1;
    h1 = h2;
    h2 = tmp;
  }
  if (h1 > h3) {
    int tmp = h1;
    h1 = h2;
    h2 = h3;
    h3 = tmp;
  }
  if (h2 > h3) {
    int tmp = h2;
    h2 = h3;
    h3 = tmp;
  }

  gsl_permutation *per = gsl_permutation_alloc(3);
  gsl_permutation_init(per);

  auto qij = get_qij();
  complex<double> qk1[4] = {0., qij[0], qij[1], qij[2]};
  complex<double> qk2[4] = {0., qij[4], qij[5], qij[6]};

  // Only neutral higgses
  int i;
  if ((h1 < 4) && (h2 < 4) && (h3 < 4))
    for (i = 0; i < 6; i++) {
      int h[3] = {h1, h2, h3};
      gsl_permute_int(gsl_permutation_data(per), h, 1, 3);
      gsl_permutation_next(per);
      c += -I * v / 2. *
           (qk1[h[0]] * conj(qk1[h[1]]) * real(qk1[h[2]]) * Z1 +
            qk2[h[0]] * conj(qk2[h[1]]) * real(qk1[h[2]]) * (Z3 + Z4) +
            real(conj(qk1[h[0]]) * qk2[h[1]] * qk2[h[2]] * Z5) +
            real((2. * qk1[h[0]] + conj(qk1[h[0]])) * conj(qk1[h[1]]) *
                 qk2[h[2]] * Z6) +
            real(conj(qk2[h[0]]) * qk2[h[1]] * qk2[h[2]] * Z7));
    }
  // Neutral and charged higgses
  if ((h1 < 4) && (h2 == 4) && (h3 == 4))
    c = -I * v * (real(qk1[h1] * Z3 + real(qk2[h1] * Z7)));

  gsl_permutation_free(per);

  return c;
}

void THDM::get_coupling_hhh(int h1, int h2, int h3, complex<double> &c) const {
  c = get_coupling_hhh(h1, h2, h3);
}

std::complex<double> THDM::get_coupling_vhh(int v, int h1, int h2) const {
  // Convention of hep-ph/0602242
  static const complex<double> I(0.0, 1.0);

  double costw = cos(atan(_g1 / _g2));
  double cos2tw = cos(2. * acos(costw));
  double e = _g2 * std::sqrt(1. - costw * costw);

  if (h1 > h2) {
    int tmp = h1;
    h1 = h2;
    h2 = tmp;
  }

  // gamma
  if (v == 1) {
    if ((h1 == 4) && (h2 == 4))
      return -I * e;
  }
  // Z
  if (v == 2) {
    if ((h1 < 4) && (h2 < 4)) {
      auto q = get_qij();
      return _g2 / 2. / costw * imag(q[h1 - 1] * conj(q[h2 - 1]) +
                                     q[4 + h1 - 1] * conj(q[4 + h2 - 1]));
    }
    if ((h1 == 4) && (h2 == 4))
      return -I * _g2 * cos2tw / 2. / costw;
  }
  // W+  (to get coupling for W- take complex conjugate)
  if ((v == 3) && (h1 < 4) && (h2 == 4)) {
    return conj(-I * _g2 / 2. * get_qki(h1, 2));
  }
  return complex<double>(0., 0.);
}
void THDM::get_coupling_vhh(int v, int h1, int h2, complex<double> &c) const {
  c = get_coupling_vhh(v, h1, h2);
}

double THDM::get_v2() const { return _v2; }

vector<double> THDM::get_gauge_couplings() const {
  return vector<double>{_g1, _g2, _g3};
}

std::vector<Eigen::Matrix3cd> THDM::get_yukawa_rho() const {
  return vector<Eigen::Matrix3cd>{_rU, _rD, _rL};
}

vector<Eigen::Matrix3cd> THDM::get_yukawa_eta() const {
  return vector<Eigen::Matrix3cd>{_eta1U, _eta1D, _eta1L,
                                  _eta2U, _eta2D, _eta2L};
}

std::vector<Eigen::Matrix3cd> THDM::get_yukawa_kF_rF() const {
  return vector<Eigen::Matrix3cd>{_kU, _kD, _kL, _rU, _rD, _rL};
}

vector<std::complex<double>> THDM::get_vevs() const {
  double v = sqrt(_v2);
  return vector<std::complex<double>>{_vhat[0] * v, _vhat[1] * v};
}

vector<double> THDM::get_mup() const {
  return vector<double>{_mup[0], _mup[1], _mup[2]};
}

vector<double> THDM::get_mdn() const {
  return vector<double>{_mdn[0], _mdn[1], _mdn[2]};
}

vector<double> THDM::get_ml() const {
  return vector<double>{_ml[0], _ml[1], _ml[2]};
}

Eigen::Matrix3cd THDM::get_vCkm() const { return _VCKM; }

void THDM::print_CKM() const { cout << " V_CKM:\n" << _VCKM << endl; }

void THDM::print_param_gen() const {
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
  cout << " eta1U\n" << _eta1U << endl;
  cout << " eta1D\n" << _eta1D << endl;
  cout << " eta1L\n" << _eta1L << endl;
  cout << " eta2U\n" << _eta2U << endl;
  cout << " eta2D\n" << _eta2D << endl;
  cout << " eta2L\n" << _eta2L << endl;
  cout << "\n";
}

void THDM::print_param_compact() const {
  cout << "\nPotential parameters in compact basis:\n";
  print_z2Symmetry();
  cout << "v = " << sqrt(_v2) << std::endl;
  cout << _base_generic.convert_to_compact();
  cout << "\n";
}

void THDM::print_param_higgs() const {
  cout << "\nPotential parameters:\n";
  print_z2Symmetry();
  cout << "v = " << sqrt(_v2) << std::endl;
  cout << "tanb = " << tan(_base_generic.beta) << std::endl;
  cout << "xi = " << _base_generic.xi << std::endl;
  cout << _base_generic.convert_to_higgs();
  cout << "\nkappa and rho Yukawa matrices:\n";
  cout << " kU\n" << _kU << endl;
  cout << " kD\n" << _kD << endl;
  cout << " kL\n" << _kL << endl;
  cout << " rU\n" << _rU << endl;
  cout << " rD\n" << _rD << endl;
  cout << " rL\n" << _rL << endl;
  cout << "\n";
}

void THDM::print_yukawa() const {
  cout << "\neta Yukawa matrices::\n";
  cout << " eta1U\n" << _eta1U << endl;
  cout << " eta1D\n" << _eta1D << endl;
  cout << " eta1L\n" << _eta1L << endl;
  cout << " eta2U\n" << _eta2U << endl;
  cout << " eta2D\n" << _eta2D << endl;
  cout << " eta2L\n" << _eta2L << endl;

  cout << "\nkappa and rho Yukawa matrices:\n";
  cout << " kU\n" << _kU << endl;
  cout << " kD\n" << _kD << endl;
  cout << " kL\n" << _kL << endl;
  cout << " rU\n" << _rU << endl;
  cout << " rD\n" << _rD << endl;
  cout << " rL\n" << _rL << endl;
  cout << "\n";
}

void THDM::clear_calculated_quantities() {
  _mh_pole[0] = -1.; // Denotes that they have not been calculated
  for (int h = 1; h <= 4; ++h)
    _mh_pole[h] = 0.;
}
void THDM::init() {
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

bool THDM::reset_to_saved_state() {
  _rgeResults.reset();

  if (_ySaved != nullptr) {
    set_model_from_y(_ySaved); // Set parameters in generic basis
    _renormScale = _savedRenormScale;

    calc_treeLvl_masses_and_mixings();
    calc_kF_rF_ckm_from_etaF();
  } else {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
  return true;
}

bool THDM::reset(const double y[], const double &renormScale) {
  _rgeResults.reset();
  _renormScale = renormScale;

  if (y != nullptr) {
    set_model_from_y(y); // Set parameters in generic basis

    calc_treeLvl_masses_and_mixings();
    calc_kF_rF_ckm_from_etaF();
  } else {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
  return true;
}

void THDM::set_v2(const double &v2) { _v2 = v2; }

void THDM::set_gauge_couplings(const vector<double> &couplings) {
  if (couplings.size() != 3) {
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
  if (mup.size() != 3 || mdn.size() != 3 || ml.size() != 3) {
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

void THDM::set_vCkm(const Eigen::Matrix3cd &VCKM) {
  _VCKM = VCKM;
  set_yukawa_type(_z2_symmetry);
}

void THDM::set_sm(const SM &sm) {
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

  calc_eta_from_kF_rF_ckm();
}

void THDM::set_sm(const double &mu, const double &v2,
                  const std::vector<double> &g_i,
                  const std::vector<double> &mup,
                  const std::vector<double> &mdn, const std::vector<double> &ml,
                  const Eigen::Matrix3cd &VCKM) {
  _renormScale = mu;
  _v2 = v2;
  set_gauge_couplings(g_i);
  set_fermion_masses(mup, mdn, ml);
  set_vCkm(VCKM);

  set_kF_from_fermion_masses();
  calc_eta_from_kF_rF_ckm();
}

void THDM::set_kF_from_fermion_masses() {
  _kU.setZero();
  _kD.setZero();
  _kL.setZero();

  for (int i = 0; i < 3; i++) {
    _kU(i, i) = complex<double>(_mup[i] * sqrt(2. / _v2), 0.);
    _kD(i, i) = complex<double>(_mdn[i] * sqrt(2. / _v2), 0.);
    _kL(i, i) = complex<double>(_ml[i] * sqrt(2. / _v2), 0.);
  }
}

void THDM::create_data_files() {
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

void THDM::write_to_data_files(const double &t) {
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
  masses.insert(masses.begin(), exp(t));
  _files.add_line(_massesFile, masses);
}

void THDM::write_slha_file(const string &fileName) const {

  vector<SLHA_BLOCK> blocks; // All blocks that will be in the file.

  blocks.emplace_back("THDME", "Features at current scale");
  blocks.back().add_parameter(0, get_renormalization_scale(),
                              "Renormalization scale");
  blocks.back().add_parameter(1, (int)_z2_symmetry,
                              "Yukawa symmetry (0=none or Type 1-4)");
  blocks.back().add_parameter(2, is_cp_conserved(),
                              "CP conserving (0=false, 1=true)");
  blocks.back().add_parameter(3, is_perturbative() ? 1 : 0,
                              "Perturbative (0=false, 1=true)");
  blocks.back().add_parameter(4, is_unitary() ? 1 : 0,
                              "Unitary, tree-lvl (0=false, 1=true)");
  blocks.back().add_parameter(5, is_stable() ? 1 : 0,
                              "Stable, tree-lvl  (0=false, 1=true)");
  blocks.back().add_parameter(6, is_stable_z2sym() ? 1 : 0,
                              "Stable, tree-lvl and z2sym  (0=false, 1=true)");

  // If the model has undergone RG evolution
  if (_rgeResults.evolved) {
    blocks.emplace_back("RGE", "Results of RGE evolution");
    blocks.back().add_parameter(0, _rgeResults.e0, "Start scale");
    blocks.back().add_parameter(1, _rgeResults.ef, "Finish scale");
    blocks.back().add_parameter(
        2, _rgeResults.ef_pert,
        "Perturbativity breakdown scale (-1 = no violation)");
    blocks.back().add_parameter(
        3, _rgeResults.ef_unit,
        "Unitarity breakdown scale (-1 = no violation)");
    blocks.back().add_parameter(
        4, _rgeResults.ef_stab,
        "Stability breakdown scale (-1 = no violation)");
  }

  // Scalar potential
  blocks.emplace_back("MIJ2", "Mij^2");
  blocks.back().add_parameter(1, _base_generic.M112, "M112");
  blocks.back().add_parameter(2, _base_generic.M222, "M222");

  blocks.emplace_back("MINPAR", "Input parameters (real part)");
  blocks.back().add_parameter(1, _base_generic.Lambda1, "Lambda1");
  blocks.back().add_parameter(2, _base_generic.Lambda2, "Lambda2");
  blocks.back().add_parameter(3, _base_generic.Lambda3, "Lambda3");
  blocks.back().add_parameter(4, _base_generic.Lambda4, "Lambda4");
  blocks.back().add_parameter(5, real(_base_generic.Lambda5), "re(Lambda5)");
  blocks.back().add_parameter(6, real(_base_generic.Lambda6), "re(Lambda6)");
  blocks.back().add_parameter(7, real(_base_generic.Lambda7), "re(Lambda7)");
  blocks.back().add_parameter(9, real(_base_generic.M12), "re(M12)");
  blocks.back().add_parameter(10, tan(_base_generic.beta), "TanBeta");

  blocks.emplace_back("IMMINPAR", "Input parameters (imag part");
  blocks.back().add_parameter(5, imag(_base_generic.Lambda5), "im(Lambda5)");
  blocks.back().add_parameter(6, imag(_base_generic.Lambda6), "im(Lambda6)");
  blocks.back().add_parameter(7, imag(_base_generic.Lambda7), "im(Lambda7)");
  blocks.back().add_parameter(9, imag(_base_generic.M12), "im(M12)");

  // Yukawa sector.
  // addYukMatrix creates two blocks for each matrix (real and imag).
  auto addYukMatrix = [](vector<SLHA_BLOCK> &blocks,
                         const Eigen::Matrix3cd &mat, const string &blockName,
                         const string &matName) {
    blocks.emplace_back(blockName, matName + " Yukawa matrix (real part)");
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        blocks.back().add_parameter(i + 1, j + 1, real(mat(i, j)),
                                    "re[" + matName + "(" + to_string(i) + "," +
                                        to_string(j) + ")]");
      }
    blocks.emplace_back("IM" + blockName,
                        matName + " Yukawa matrix (imag part)");
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) {
        blocks.back().add_parameter(i + 1, j + 1, imag(mat(i, j)),
                                    "im[" + matName + "(" + to_string(i) + "," +
                                        to_string(j) + ")]");
      }
  };

  addYukMatrix(blocks, _eta1U, "ETA1UIN", "eta1U");
  addYukMatrix(blocks, _eta1D, "ETA1DIN", "eta1D");
  addYukMatrix(blocks, _eta1L, "ETA1LIN", "eta1L");
  addYukMatrix(blocks, _eta2U, "ETA2UIN", "eta2U");
  addYukMatrix(blocks, _eta2D, "ETA2DIN", "eta2D");
  addYukMatrix(blocks, _eta2L, "ETA2LIN", "eta2L");

  double v = sqrt(_v2);

  blocks.emplace_back("HMIXIN", "VEVs");
  blocks.back().add_parameter(102, cos(_base_generic.beta) * v, "re(v1)");
  blocks.back().add_parameter(
      103, real(std::polar(1., _base_generic.xi) * sin(_base_generic.beta)) * v,
      "re(v2)");

  blocks.emplace_back("IMHMIXIN", "VEVs");
  blocks.back().add_parameter(102, 0., "im(v1)");
  blocks.back().add_parameter(
      103, imag(std::polar(1., _base_generic.xi) * sin(_base_generic.beta)) * v,
      "im(v2)");

  blocks.emplace_back("GAUGEIN", "Gauge couplings");
  blocks.back().add_parameter(1, _g1, "g1");
  blocks.back().add_parameter(2, _g2, "g2");
  blocks.back().add_parameter(3, _g3, "g3");

  SLHA_FILE file("2HDM SLHA file created by 2HDME");
  file.add_blocks(blocks);
  file.save_to_file(fileName);

  _console.info << "SLHA output file, " << fileName << ", created!\n";
}

bool THDM::set_from_slha_file(const string &fileName) {
  SLHA_FILE file;
  if (!file.load_file(fileName))
    return false;

  _renormScale = file.get_param("THDME", 0);
  _z2_symmetry = (Z2symmetry)file.get_param("THDME", 1);

  double y[_numParams];
  y[0] = file.get_param("GAUGEIN", 1);
  y[1] = file.get_param("GAUGEIN", 2);
  y[2] = file.get_param("GAUGEIN", 3);
  y[3] = file.get_param("HMIXIN", 102);
  y[4] = file.get_param("IMHMIXIN", 102);
  y[5] = file.get_param("HMIXIN", 103);
  y[6] = file.get_param("IMHMIXIN", 103);
  y[7] = file.get_param("MINPAR", 10);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      y[8 + i * 3 + j] = file.get_param("ETA1UIN", i, j);
      y[17 + i * 3 + j] = file.get_param("IMETA1UIN", i, j);
      y[26 + i * 3 + j] = file.get_param("ETA1DIN", i, j);
      y[35 + i * 3 + j] = file.get_param("IMETA1DIN", i, j);
      y[44 + i * 3 + j] = file.get_param("ETA1LIN", i, j);
      y[53 + i * 3 + j] = file.get_param("IMETA1LIN", i, j);
      y[62 + i * 3 + j] = file.get_param("ETA2UIN", i, j);
      y[71 + i * 3 + j] = file.get_param("IMETA2UIN", i, j);
      y[80 + i * 3 + j] = file.get_param("ETA2DIN", i, j);
      y[89 + i * 3 + j] = file.get_param("IMETA2DIN", i, j);
      y[98 + i * 3 + j] = file.get_param("ETA2LIN", i, j);
      y[107 + i * 3 + j] = file.get_param("IMETA2LIN", i, j);
    }

  y[116] = file.get_param("MIJ2", 1);
  y[117] = file.get_param("MIJ2", 2);
  y[118] = file.get_param("MINPAR", 9);
  y[119] = file.get_param("IMMINPAR", 9);
  y[120] = file.get_param("MINPAR", 1);
  y[121] = file.get_param("MINPAR", 2);
  y[122] = file.get_param("MINPAR", 3);
  y[123] = file.get_param("MINPAR", 4);
  y[124] = file.get_param("MINPAR", 5);
  y[125] = file.get_param("IMMINPAR", 5);
  y[126] = file.get_param("MINPAR", 6);
  y[127] = file.get_param("IMMINPAR", 6);
  y[128] = file.get_param("MINPAR", 7);
  y[129] = file.get_param("IMMINPAR", 7);

  set_model_from_y(y);
  calc_kF_rF_ckm_from_etaF();

  return calc_treeLvl_masses_and_mixings();
}

//-2HDMC-functions-------------------------------------------------------------

void THDM::get_coupling_hdu(int h, int d, int u, std::complex<double> &cs,
                            std::complex<double> &cp) const {

  complex<double> I(0.0, 1.0);

  complex<double> x(0., 0.);
  complex<double> y(0., 0.);
  cs = 0.;
  cp = 0.;

  if (h != 4) // Only Hc couples u and d
    return;

  if ((u <= 3) && (u >= 1) && (d <= 3) && (d >= 1)) {

    Eigen::Matrix3cd gR = _VCKM * _rD;
    Eigen::Matrix3cd gL = _rU.adjoint() * _VCKM;

    complex<double> rd = gR(u - 1, d - 1);
    complex<double> ru = gL(u - 1, d - 1);

    cs = -I * 0.5 * (rd - ru);
    cp = -I * 0.5 * (rd + ru);
  }
}

void THDM::get_coupling_hdd(int h, int f1, int f2, complex<double> &cs,
                            complex<double> &cp) const {
  cs = 0.;
  cp = 0.;

  if ((h < 1) || (h > 3))
    return;

  if ((f1 <= 3) && (f1 >= 1) && (f2 <= 3) && (f2 >= 1)) {
    cs = get_hdd(h)(f1 - 1, f2 - 1);
    cp = get_hdd_axial(h)(f1 - 1, f2 - 1);
  }
}

void THDM::get_coupling_huu(int h, int f1, int f2, complex<double> &cs,
                            complex<double> &cp) const {
  cs = 0.;
  cp = 0.;

  if ((h < 1) || (h > 3))
    return;

  if ((f1 <= 3) && (f1 >= 1) && (f2 <= 3) && (f2 >= 1)) {
    cs = get_huu(h)(f1 - 1, f2 - 1);
    cp = get_huu_axial(h)(f1 - 1, f2 - 1);
  }
}

void THDM::get_coupling_hll(int h, int f1, int f2, complex<double> &cs,
                            complex<double> &cp) const {
  cs = 0.;
  cp = 0.;

  if ((h < 1) || (h > 3))
    return;

  if ((f1 <= 3) && (f1 >= 1) && (f2 <= 3) && (f2 >= 1)) {
    cs = get_hll(h)(f1 - 1, f2 - 1);
    cp = get_hll_axial(h)(f1 - 1, f2 - 1);
  }
}
void THDM::get_coupling_hln(int h, int l, int n, complex<double> &cs,
                            complex<double> &cp) const {
  cs = 0.;
  cp = 0.;

  if (h != 4)
    return;

  complex<double> I(0., 1.);

  if ((n <= 3) && (n >= 1) && (l <= 3) && (l >= 1)) {
    cs = -I * 0.5 * _rL(n - 1, l - 1);
    cp = -I * 0.5 * _rL(n - 1, l - 1);
  }
}
} // namespace THDME