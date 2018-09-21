// THDM_bases.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Miscellaneous functions related to THDM
 * @author: Joel Oredsson
 *
 * Collection of different bases for THDM as structs.
 * Also other stuff...
 * 
 * Bases of general THDM:
 *   -> Generic
 *   -> Compact
 *   -> Higgs
 *   -> Invariant
 * 
 * Basis of CP conserving softly Z2 broken THDM:
 *   -> Hybrid
 *
 *============================================================================*/
#pragma once

#include <gsl/gsl_rng.h>
#include <fstream>
#include <iostream>
#include <complex>
#include <string>
#include <Eigen/Dense>

namespace THDME {

/**
 * @brief Different type of Z_2 symmetries of the THDMs Yukawa sector.
 */
enum Z2symmetry { NO_SYMMETRY, TYPE_I, TYPE_II, TYPE_III, TYPE_IV };

/**
 * @brief Bases that are implemented in the general complex 2HDM.
 */
enum BaseType { GENERIC, COMPACT, HIGGS, HYBRID, INVARIANT };

enum FermionSector
{
  UP, DOWN, LEPTON
};

//--THDM-bases------------------------------------------------------------------

/**
 * @brief Base struct for all bases.
 *
 * Base struct for all THDM bases. They are all characterized by
 * different BaseType.
 *
 * @params:
 *   xi: phase of VEV.
 *   beta: angle related to generic basis with tan(beta) = v_2 / v_1.
 */
struct ThdmBasis {
  BaseType type;
  double xi, beta;
};

struct Base_generic;
struct Base_compact;
struct Base_higgs;
struct Base_invariant;
struct Base_hybrid;

/**
 * @brief: Generic 2HDM potential
 *
 * tanb is defined as the ratio of the Higgs doublets in this basis.
 */
struct Base_generic : ThdmBasis {
  double M112, M222, Lambda1, Lambda2, Lambda3, Lambda4;
  std::complex<double> M12, Lambda5, Lambda6, Lambda7;

  Base_generic() { type = GENERIC; xi = 0.; }

  // Overloading the << operator for ostream
  friend std::ostream &operator<<(std::ostream &oS, const Base_generic &gen);
  
  void generate_random_softCpConserved(const gsl_rng *rng);

  Base_compact convert_to_compact() const;
  Base_higgs convert_to_higgs() const;
  Base_invariant convert_to_invariant(const double &v2) const;

  std::vector<double> convert_to_vector() const;
};

/**
 * @brief: Compact basis/notation for the generic potential
 */
struct Base_compact : ThdmBasis {
  std::complex<double> Y[2][2], Z[2][2][2][2];

  Base_compact() { type = COMPACT; xi = 0.; }

  // Overloading the << operator for ostream
  friend std::ostream &operator<<(std::ostream &oS, const Base_compact &comp);
};

/**
 * @brief: Higgs basis
 *
 * The Higgs basis is defined as the basis where only the first Higgs doublet
 * acquires a VEV. It is unique, up to a phase transformation of the second
 * Higgs.
 * 
 */
struct Base_higgs : ThdmBasis {
  double mHc, Y1, Y2, Z1, Z2, Z3, Z4;
  std::complex<double> Y3, Z5, Z6, Z7;

  Base_higgs() { type = HIGGS; xi = 0.; }

  // Overloading the << operator for ostream
  friend std::ostream &operator<<(std::ostream &oS, const Base_higgs &higgs);

  Base_generic convert_to_generic() const;
  Base_compact convert_to_compact() const;
  Base_invariant convert_to_invariant(const double &v2) const;

  std::vector<double> convert_to_vector() const;
};

/**
 * @brief: U(2) Higgs flavor invariant basis
 *
 * This basis is specified by the eigenvalues of the Higgs mass matrix;
 * combined with scalar mixing angles and some quartic couplings.
 * The rotation to the Higgs basis is specified in Phys.Rev.D 74, 015018.
 *
 * @params:
 *   mh[3]: Neutral Higgs tree-lvl masses (Different ordering corresponds to
 *          different potential parameters)
 *   s12, c13: Sin/Cos of angles of a SO(3) matrix that diagonalizes the neutral
 *             Higgs mass matrix, range = -pi/2 to pi/2.
 *   mHc: Charged Higgs mass.
 *   Z2, Z3: Quartic couplings in the Higgs basis.
 *   Z7inv: Invariant combination Z7*exp(-itheta23).
 * 
 *   theta23: This parameter is arbitrary, corresponds to the shift chi
 *            in the Higgs basis. Setting it, only gives different Higgs bases.
 */
struct Base_invariant : ThdmBasis {
  double mHc, mh[3], s12, c13, Z2, Z3, theta23;
  std::complex<double> Z7inv;

  double cPhi; // cPhi is not needed as a free parameter, but is an U(2)
               // invariant one.

  Base_invariant() : theta23(0.) { type = INVARIANT; xi = 0.; }

  // Overloading the << operator for ostream
  friend std::ostream &operator<<(std::ostream &oS, const Base_invariant &inv);

  // Overloading the << operator for ofstream
  friend std::ofstream &operator<<(std::ofstream &of,
                                   const Base_invariant &inv);

  Base_generic convert_to_generic(const double &v2) const;
  Base_compact convert_to_compact(const double &v2) const;
  Base_higgs convert_to_higgs(const double &v2) const;

  std::vector<double> convert_to_vector() const;
};

/**
 * @brief: Hybrid basis
 *
 * Soflty broken Z_2 CP conserving hybrid basis from arxiv:1507.04281.
 * 
 * It is a combination of tree-lvl masses and Higgs basis quartic couplings.
 * Compared to the invariant basis, it is convenient to substitute mA and mHc
 * for Z4 and Z5; because it is easier to find parameter points with small
 * quartic couplings.
 * 
 * CP conservation is assumed, thus all parameters are real.
 * Degrees of freedom in scalar potential = 7.
 *
 * @params:
 *   mh: Lightest neutral CP even Higgs boson mass.
 *   mH: Heaviest neutral CP even Higgs boson mass.
 *   Z4, Z5, Z7: Real quartic couplings.
 *   cba: cos(beta-alpha)
 *   tanb: tan(beta)
 */
struct Base_hybrid : ThdmBasis {
  double mh, mH, cba, Z4, Z5, Z7, tanb;

  Base_hybrid() { type = HYBRID; xi = 0.; }

  // Overloading the << operator for ostream
  friend std::ostream &operator<<(std::ostream &oS, const Base_hybrid &hyb);
  
  // Sets parameters to random values.
  void generate_random_softCpConserved(const gsl_rng *rng);

  Base_generic convert_to_generic(const double &v2) const;
  Base_higgs convert_to_higgs(const double &v2) const;
  Base_invariant convert_to_invariant(const double &v2) const;

  std::vector<double> convert_to_vector() const;
};
}