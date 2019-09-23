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
 * Basis of CP violating softly Z2 broken THDM:
 *   -> C2HDM
 *
 *============================================================================*/
#pragma once

#include <gsl/gsl_rng.h>
#include <complex>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Eigen/Dense>

namespace THDME {

/**
 * @brief Different type of Z_2 symmetries of the THDMs Yukawa sector.
 */
enum Z2symmetry {
    NO_SYMMETRY = 0,
    TYPE_I = 1,
    TYPE_II = 2,
    TYPE_III = 3,
    TYPE_IV = 4
};

/**
 * @brief Bases that are implemented in the general complex 2HDM.
 */
enum BaseType {
    GENERIC,
    COMPACT,
    HIGGS,
    HYBRID,
    INVARIANT,
    C2HDM,
    PHYSICAL,
    S3
};

enum FermionSector { UP, DOWN, LEPTON };

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
    double xi = 0., beta = 0.;
};

struct Base_generic;
struct Base_compact;
struct Base_higgs;
struct Base_invariant;
struct Base_hybrid;

/******************************************************************************
 * @brief: Generic 2HDM potential
 *
 * tanb is defined as the ratio of the Higgs doublets in this basis.
 */
struct Base_generic : ThdmBasis {
    double M112 = 0., M222 = 0, Lambda1 = 0., Lambda2 = 0., Lambda3 = 0.,
           Lambda4 = 0;
    std::complex<double> M12 = 0., Lambda5 = 0., Lambda6 = 0., Lambda7 = 0.;

    Base_generic() { type = GENERIC; }

    bool contains_nan() const;  // Returns true/false if any parameter is NaN.

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_generic &gen);

    void generate_random_softCpConserved(const gsl_rng *rng);

    Base_compact convert_to_compact() const;
    Base_higgs convert_to_higgs() const;
    Base_invariant convert_to_invariant(const double &v2) const;

    std::vector<double> convert_to_vector() const;
};

// Needed for given parameters to gsl functions
struct genStruct {
    double v2;
    Base_generic gen;
};

/******************************************************************************
 * @brief: Compact basis/notation for the generic potential
 */
struct Base_compact : ThdmBasis {
    std::complex<double> Y[2][2], Z[2][2][2][2];

    Base_compact() { type = COMPACT; }

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_compact &comp);
};

/******************************************************************************
 * @brief: Higgs basis
 *
 * The Higgs basis is defined as the basis where only the first Higgs doublet
 * acquires a VEV. It is unique, up to a phase transformation of the second
 * Higgs.
 *
 */
struct Base_higgs : ThdmBasis {
    double mHc = 0, Y1 = 0, Y2 = 0, Z1 = 0, Z2 = 0, Z3 = 0, Z4 = 0;
    std::complex<double> Y3 = 0, Z5 = 0, Z6 = 0, Z7 = 0;

    Base_higgs() { type = HIGGS; }

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_higgs &higgs);

    Base_generic convert_to_generic() const;
    Base_compact convert_to_compact() const;
    Base_invariant convert_to_invariant(const double &v2) const;

    std::vector<double> convert_to_vector() const;
};

/******************************************************************************
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
 *             sin(13) is negative for ordered masses.
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

    double cPhi;  // cPhi is not needed as a free parameter, but is an U(2)
                  // invariant one.

    Base_invariant() : theta23(0.) { type = INVARIANT; }

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS,
                                    const Base_invariant &inv);

    // Overloading the << operator for ofstream
    friend std::ofstream &operator<<(std::ofstream &of,
                                     const Base_invariant &inv);

    Base_generic convert_to_generic(const double &v2) const;
    Base_compact convert_to_compact(const double &v2) const;
    Base_higgs convert_to_higgs(const double &v2) const;

    Eigen::Matrix3d get_R() const;

    std::vector<double> convert_to_vector() const;
};

/******************************************************************************
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

    Base_hybrid() { type = HYBRID; }

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_hybrid &hyb);

    // Sets parameters to random values.
    void generate_random_softCpConserved(const gsl_rng *rng);

    Base_generic convert_to_generic(const double &v2) const;
    Base_higgs convert_to_higgs(const double &v2) const;
    Base_invariant convert_to_invariant(const double &v2) const;

    std::vector<double> convert_to_vector() const;
};

/******************************************************************************
 * @brief: Physical basis
 *
 * CP conserving hybrid basis.
 *
 * It is a combination of tree-lvl masses and Higgs basis quartic couplings.
 *
 * CP conservation is assumed, thus all parameters are real.
 * Degrees of freedom in scalar potential = 9.
 *
 * @params:
 *   mh: Lightest neutral CP even Higgs boson mass.
 *   mH: Heaviest neutral CP even Higgs boson mass.
 *   mA: CP odd Higgs boson mass.
 *   mHc: Charged Higgs boson mass.
 *   sba: sin(beta-alpha)
 *   lambda6, lambda7: Real quartic couplings.
 *   M12: m_{12}^2
 *   tanb: tan(beta)
 */
struct Base_physical : ThdmBasis {
    double mh, mH, mA, mHc, sba, lambda6, lambda7, M12, tanb;

    Base_physical() { type = PHYSICAL; }

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_physical &phy);

    Base_generic convert_to_generic(const double &v2) const;
    Base_higgs convert_to_higgs(const double &v2) const;
    Base_invariant convert_to_invariant(const double &v2) const;
};
/******************************************************************************
 * @brief: C2HDM basis
 *
 * Soflty broken Z_2 CP violating basis.
 *
 * It is a combination of tree-lvl masses and diagonalizing angles.
 *
 * Degrees of freedom in scalar potential = 8.
 *
 * @params:
 *   mh[2]: Higgs boson tree-level mass in ascending order.
 *          The third one is a dependent quantity.
 *   mHc: Charged Higgs boson tree-level mass.
 *   M12: Mass parameter, 2 * m_{12}^2.
 *   alpha[3]: Angles that diagonalizes the neutral Higgs mass matrix.
 *             Their range is [-pi/2, pi/2].
 *   tanb: tan(beta)
 */
struct Base_c2hdm : ThdmBasis {
    std::vector<double> mh, alpha;
    double mHc, M12, tanb;

    Base_c2hdm();

    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_c2hdm &c2hdm);

    // Sets parameters to random values.
    void generate_random(const gsl_rng *rng);

    Base_generic convert_to_generic(const double &v2) const;
    Base_higgs convert_to_higgs(const double &v2) const;
    Base_invariant convert_to_invariant(const double &v2) const;

    std::vector<double> convert_to_vector() const;
};

/******************************************************************************
 * @brief: S_3 symmetric 2HDM potential
 *
 * tanb is defined as the ratio of the Higgs doublets in this basis.
 * This basis can be found in for example arXiv:1705.07784.
 */
struct Base_s3 : ThdmBasis {
    double mu1 = 0., mu2 = 0., mu12 = 0., lambda1 = 0., lambda2 = 0.,
           lambda3 = 0., tanb = 0.;

    Base_s3() { type = S3; }
    Base_s3(double v, double tb, double mh, double mH, double mA, double mHc);
    // Overloading the << operator for ostream
    friend std::ostream &operator<<(std::ostream &oS, const Base_s3 &s3);

    Base_generic convert_to_generic(const double &v2) const;
    Base_compact convert_to_compact(const double &v2) const;
    Base_higgs convert_to_higgs(const double &v2) const;
    Base_invariant convert_to_invariant(const double &v2) const;
};

}  // namespace THDME
