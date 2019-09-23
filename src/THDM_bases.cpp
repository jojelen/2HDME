// THDM_bases.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Miscellaneous functions related to THDM
 * @author: Joel Oredsson
 *============================================================================*/
#include "THDM_bases.h"
#include "HelpFunctions.h"
#include "Structures.h"

#include <Eigen/Dense>

using namespace std;

namespace THDME {

// "<<" operator overloading

std::ostream &operator<<(std::ostream &oS, const Base_generic &gen) {
    oS << "Generic basis:\n"
       << "beta = " << gen.beta << ", tanb = " << tan(gen.beta) << endl
       << "M112 = " << gen.M112 << " GeV^2" << endl
       << "M222 = " << gen.M222 << " GeV^2" << endl
       << "M12 = " << gen.M12 << " GeV^2" << endl
       << "Lambda1 = " << gen.Lambda1 << endl
       << "Lambda2 = " << gen.Lambda2 << endl
       << "Lambda3 = " << gen.Lambda3 << endl
       << "Lambda4 = " << gen.Lambda4 << endl
       << "Lambda5 = " << gen.Lambda5 << endl
       << "Lambda6 = " << gen.Lambda6 << endl
       << "Lambda7 = " << gen.Lambda7 << endl;
    return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_compact &comp) {
    oS << "Compact basis:\n"
       << "tanb = " << tan(comp.beta) << endl
       << "Y_11 = " << comp.Y[0][0] << " GeV^2" << endl
       << "Y_12 = " << comp.Y[0][1] << " GeV^2" << endl
       << "Y_21 = " << comp.Y[1][0] << " GeV^2" << endl
       << "Y_22 = " << comp.Y[1][1] << " GeV^2" << endl
       << "\nZ_1111 = " << comp.Z[0][0][0][0] << endl
       << "Z_2222 = " << comp.Z[1][1][1][1] << endl
       << "Z_1122 = Z_2211 = " << comp.Z[0][0][1][1] << endl
       << "Z_1221 = Z_2112 = " << comp.Z[0][1][1][0] << endl
       << "Z_1212 = " << comp.Z[0][1][0][1] << endl
       << "Z_2121 = " << comp.Z[1][0][1][0] << endl
       << "Z_1112 = Z_1211 = " << comp.Z[0][0][0][1] << endl
       << "Z_1121 = Z_2111 = " << comp.Z[0][0][1][0] << endl
       << "Z_2212 = Z_1222 = " << comp.Z[0][1][1][1] << endl
       << "Z_2221 = Z_2122 = " << comp.Z[1][1][1][0] << endl;
    return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_higgs &higgs) {
    oS << "Higgs basis:\n"
       << "Y1 = " << higgs.Y1 << endl
       << "Y2 = " << higgs.Y2 << endl
       << "Y3 = " << higgs.Y3 << endl
       << "Z1 = " << higgs.Z1 << endl
       << "Z2 = " << higgs.Z2 << endl
       << "Z3 = " << higgs.Z3 << endl
       << "Z4 = " << higgs.Z4 << endl
       << "Z5 = " << higgs.Z5 << endl
       << "Z6 = " << higgs.Z6 << endl
       << "Z7 = " << higgs.Z7 << endl;
    return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_invariant &inv) {
    oS << "Invariant basis (theta23 = " << inv.theta23 << " ):\n"
       << "mh[0] = " << inv.mh[0] << endl
       << "mh[1] = " << inv.mh[1] << endl
       << "mh[2] = " << inv.mh[2] << endl
       << "mHc = " << inv.mHc << endl
       << "Z2 = " << inv.Z2 << endl
       << "Z3 = " << inv.Z3 << endl
       << "Z7inv =" << inv.Z7inv << endl
       << "s12 = " << inv.s12 << endl
       << "c13 = " << inv.c13 << endl
       << "tanb = " << tan(inv.beta) << endl;
    return oS;
}

std::ofstream &operator<<(std::ofstream &of, const Base_invariant &inv) {
    of << "Invariant basis (theta23 = " << inv.theta23 << " ):\n"
       << "mh[0] = " << inv.mh[0] << " GeV" << endl
       << "mh[1] = " << inv.mh[1] << " GeV" << endl
       << "mh[2] = " << inv.mh[2] << " GeV" << endl
       << "mHc = " << inv.mHc << " GeV" << endl
       << "Z2 = " << inv.Z2 << endl
       << "Z3 = " << inv.Z3 << endl
       << "Z7inv =" << inv.Z7inv << endl
       << "s12 = " << inv.s12 << endl
       << "c13 = " << inv.c13 << endl
       << "tanb = " << tan(inv.beta) << endl;
    return of;
}

std::ostream &operator<<(std::ostream &oS, const Base_hybrid &hyb) {
    oS << "Hybrid basis:\n"
       << "mh = " << hyb.mh << " GeV" << endl
       << "mH = " << hyb.mH << " GeV" << endl
       << "Z4 = " << hyb.Z4 << endl
       << "Z5 = " << hyb.Z5 << endl
       << "Z7 =" << hyb.Z7 << endl
       << "cba = " << hyb.cba << endl
       << "(sba = " << std::sqrt(std::abs(1. - hyb.cba * hyb.cba)) << ")\n"
       << "tanb = " << hyb.tanb << endl;
    return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_physical &phy) {
    oS << "Physical basis:\n"
       << "mh = " << phy.mh << endl
       << "mH = " << phy.mH << endl
       << "mA = " << phy.mA << endl
       << "mHc = " << phy.mHc << endl
       << "sba = " << phy.sba << endl
       << "lambda6 = " << phy.lambda6 << endl
       << "lambda7 =" << phy.lambda7 << endl
       << "M12 = " << phy.M12 << endl
       << "tanb = " << phy.tanb << endl;
    return oS;
}

std::ostream &operator<<(std::ostream &oS, const Base_c2hdm &c2hdm) {
    oS << "C2HDM basis:\n"
       << "mh_1 = " << c2hdm.mh[0] << " GeV" << endl
       << "mh_2 = " << c2hdm.mh[1] << " GeV" << endl
       << "mHc = " << c2hdm.mHc << " GeV" << endl
       << "M12 = " << c2hdm.M12 << " GeV^2" << endl
       << "alpha_1 = " << c2hdm.alpha[0] << endl
       << "alpha_2 = " << c2hdm.alpha[1] << endl
       << "alpha_3 = " << c2hdm.alpha[2] << endl
       << "tanb = " << c2hdm.tanb << endl;
    return oS;
}
std::ostream &operator<<(std::ostream &oS, const Base_s3 &s3) {
    oS << "Generic basis:\n"
       << "beta = " << s3.beta << ", tanb = " << s3.tanb << endl
       << "mu12 = " << s3.mu1 << " GeV^2" << endl
       << "mu22 = " << s3.mu2 << " GeV^2" << endl
       << "mu12 = " << s3.mu12 << " GeV^2" << endl
       << "lambda1 = " << s3.lambda1 << endl
       << "lambda2 = " << s3.lambda2 << endl
       << "lambda3 = " << s3.lambda3 << endl;
    return oS;
}
//--Base_generic----------------------------------------------------------------

bool Base_generic::contains_nan() const {
    if (std::isnan(xi)) return true;
    if (std::isnan(beta)) return true;
    if (std::isnan(M112)) return true;
    if (std::isnan(M222)) return true;
    if (std::isnan(Lambda1)) return true;
    if (std::isnan(Lambda2)) return true;
    if (std::isnan(Lambda3)) return true;
    if (std::isnan(Lambda4)) return true;
    if (std::isnan(real(Lambda5))) return true;
    if (std::isnan(imag(Lambda5))) return true;
    if (std::isnan(real(Lambda6))) return true;
    if (std::isnan(imag(Lambda6))) return true;
    if (std::isnan(real(Lambda7))) return true;
    if (std::isnan(imag(Lambda7))) return true;

    return false;
}

void Base_generic::generate_random_softCpConserved(const gsl_rng *rng) {
    static const double_range betaRange = {atan(0.1), atan(50.)};
    static const double_range MijRange = {5000., 1.e5};
    static const double_range lambdaRange = {-M_PI, M_PI};

    beta =
        betaRange.min + (betaRange.max - betaRange.min) * gsl_rng_uniform(rng);
    M12 = complex<double>(
        MijRange.min + (MijRange.max - MijRange.min) * gsl_rng_uniform(rng),
        0.);
    Lambda1 = lambdaRange.min +
              (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
    Lambda2 = lambdaRange.min +
              (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
    Lambda3 = lambdaRange.min +
              (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
    Lambda4 = lambdaRange.min +
              (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng);
    Lambda5 = complex<double>(
        lambdaRange.min +
            (lambdaRange.max - lambdaRange.min) * gsl_rng_uniform(rng),
        0.);
    Lambda6 = complex<double>(0., 0.);
    Lambda7 = complex<double>(0., 0.);
}

Base_compact Base_generic::convert_to_compact() const {
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

Base_higgs Base_generic::convert_to_higgs() const {
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

    higgs.Z1 = Lambda1 * pow(cb, 4.) + Lambda2 * pow(sb, 4.) +
               0.5 * Lambda345 * pow(sin(2. * (beta)), 2.) +
               2. * sin(2. * (beta)) * (cb * cb * real(Lambda6 * expIxi) +
                                        sb * sb * real(Lambda7 * expIxi));

    higgs.Z2 = Lambda1 * pow(sb, 4.) + Lambda2 * pow(cb, 4.) +
               0.5 * Lambda345 * pow(sin(2. * (beta)), 2.) -
               2. * sin(2. * (beta)) * (sb * sb * real(Lambda6 * expIxi) +
                                        cb * cb * real(Lambda7 * expIxi));

    higgs.Z3 = 0.25 * pow(sin(2. * (beta)), 2.) *
                   (Lambda1 + Lambda2 - 2. * Lambda345) +
               Lambda3 -
               sin(2. * (beta)) * cos(2. * (beta)) *
                   real((Lambda6 - Lambda7) * expIxi);

    higgs.Z4 = 0.25 * pow(sin(2. * (beta)), 2.) *
                   (Lambda1 + Lambda2 - 2. * Lambda345) +
               Lambda4 -
               sin(2. * (beta)) * cos(2. * (beta)) *
                   real((Lambda6 - Lambda7) * expIxi);

    higgs.Z5 =
        std::polar(1., -2. * xi) *
        (0.25 * pow(sin(2. * (beta)), 2.) *
             (Lambda1 + Lambda2 - 2. * Lambda345) +
         real(Lambda5 * std::polar(1., 2. * xi)) +
         I * cos(2. * (beta)) * imag(Lambda5 * std::polar(1., 2. * (xi))) -
         sin(2. * (beta)) * cos(2. * (beta)) *
             real((Lambda6 - Lambda7) * expIxi) -
         I * sin(2. * (beta)) * imag((Lambda6 - Lambda7) * expIxi));

    higgs.Z6 = std::polar(1., -xi) *
               (-0.5 * sin(2. * (beta)) *
                    (Lambda1 * cb * cb - Lambda2 * sb * sb -
                     Lambda345 * cos(2. * (beta)) -
                     I * imag(Lambda5 * std::polar(1., 2. * xi))) +
                cb * cos(3. * (beta)) * real(Lambda6 * expIxi) +
                sb * sin(3. * (beta)) * real(Lambda7 * expIxi) +
                I * cb * cb * imag(Lambda6 * expIxi) +
                I * sb * sb * imag(Lambda7 * expIxi));

    higgs.Z7 = std::polar(1., -xi) *
               (-0.5 * sin(2. * (beta)) *
                    (Lambda1 * sb * sb - Lambda2 * cb * cb +
                     Lambda345 * cos(2. * (beta)) +
                     I * imag(Lambda5 * std::polar(1., 2. * xi))) +
                sb * sin(3. * (beta)) * real(Lambda6 * expIxi) +
                cb * cos(3. * (beta)) * real(Lambda7 * expIxi) +
                I * sb * sb * imag(Lambda6 * expIxi) +
                I * cb * cb * imag(Lambda7 * expIxi));

    return higgs;
}

Base_invariant Base_generic::convert_to_invariant(const double &v2) const {
    Base_invariant inv = convert_to_higgs().convert_to_invariant(v2);
    return inv;
}

vector<double> Base_generic::convert_to_vector() const {
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

Base_generic Base_higgs::convert_to_generic() const {
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

    gen.M12 = std::polar(1., -xi) *
              (0.5 * (Y1 - Y2) * sin(2. * (-beta)) -
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

    gen.Lambda3 = 0.25 * pow(sin(2. * (-beta)), 2.) * (Z1 + Z2 - 2. * Z345) +
                  Z3 -
                  sin(2. * (-beta)) * cos(2. * (-beta)) *
                      real((Z6 - Z7) * std::polar(1., xi));

    gen.Lambda4 = 0.25 * pow(sin(2. * (-beta)), 2.) * (Z1 + Z2 - 2. * Z345) +
                  Z4 -
                  sin(2. * (-beta)) * cos(2. * (-beta)) *
                      real((Z6 - Z7) * std::polar(1., xi));

    gen.Lambda5 =
        std::polar(1., -2. * xi) *
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
              Z345 * cos(2. * (-beta)) -
              I * imag(Z5 * std::polar(1., 2. * xi))) +
         cos(-beta) * cos(3. * (-beta)) * real(Z6 * std::polar(1., xi)) +
         sin(-beta) * sin(3. * (-beta)) * real(Z7 * std::polar(1., xi)) +
         I * pow(cos(-beta), 2.) * imag(Z6 * std::polar(1., xi)) +
         I * pow(sin(-beta), 2.) * imag(Z7 * std::polar(1., xi)));

    gen.Lambda7 =
        std::polar(1., -xi) *
        (-0.5 * sin(2. * (-beta)) *
             (Z1 * pow(sin(-beta), 2.) - Z2 * pow(cos(-beta), 2.) +
              Z345 * cos(2. * (-beta)) +
              I * imag(Z5 * std::polar(1., 2. * xi))) +
         sin(-beta) * sin(3. * (-beta)) * real(Z6 * std::polar(1., xi)) +
         cos(-beta) * cos(3. * (-beta)) * real(Z7 * std::polar(1., xi)) +
         I * pow(sin(-beta), 2.) * imag(Z6 * std::polar(1., xi)) +
         I * pow(cos(-beta), 2.) * imag(Z7 * std::polar(1., xi)));
    return gen;
}

Base_invariant Base_higgs::convert_to_invariant(const double &v2) const {
    Base_invariant inv;
    //  Algorithm:
    //  1.) Calculate ordered eigenvalues of neutral Higgs mass matrix.
    //  2.) Fix U(2) invariant angles that rotates to a diagonal mass matrix.

    // The general case where all neutral Higgs fields mix

    Eigen::Matrix3d mMatrix(3, 3);  // Mass matrix
    mMatrix.setZero();
    mMatrix << v2 * Z1, v2 * real(Z6), -v2 * imag(Z6), v2 * real(Z6),
        v2 * 0.5 * (Z3 + Z4 + real(Z5)) + Y2, -v2 * 0.5 * imag(Z5),
        -v2 * imag(Z6), -v2 * 0.5 * imag(Z5),
        v2 * 0.5 * (Z3 + Z4 - real(Z5)) + Y2;

    // Since the matrix is symmetric it gives real ordered eigenvalues.
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> M(mMatrix);

    for (int i = 0; i < 3; i++) {
        if (M.eigenvalues()[i] < 0) {
            // cout << "[WARNING]: negative eigenvalues in mass matrix, (mh[" <<
            // i
            //      << "]^2 = " << M.eigenvalues()[i] << " GeV^2). \n"; // DEBUG
            inv.mh[i] = 0.;
        } else
            inv.mh[i] = sqrt(M.eigenvalues()[i]);
    }

    //--------- Calculating mixing angles --------------------------------------

    // If Z6 = 0 and Z1*v2 is the smallest eigenvalue.
    if (Z1 * v2 - inv.mh[0] * inv.mh[0] < 1e-6) {
        inv.s12 = 0.;
        inv.c13 = 1.;
        inv.cPhi = 1.;
    } else {
        // double s132Old =
        //     ((Z1 * v2 - inv.mh[0] * inv.mh[0]) * (Z1 * v2 - inv.mh[1] *
        //     inv.mh[1])
        //     +
        //      std::abs(Z6) * std::abs(Z6) * v2 * v2) /
        //     ((inv.mh[2] * inv.mh[2] - inv.mh[0] * inv.mh[0]) *
        //      (inv.mh[2] * inv.mh[2] - inv.mh[1] * inv.mh[1]));
        // New method to minimize round off errors:
        double numerator = std::pow(Z1 * v2, 2) +
                           std::pow(inv.mh[0] * inv.mh[1], 2) +
                           std::pow(std::abs(Z6) * v2, 2);
        numerator -= Z1 * v2 * inv.mh[0] * inv.mh[0];
        numerator -= Z1 * v2 * inv.mh[1] * inv.mh[1];
        double denominator = std::pow(inv.mh[2] * inv.mh[2], 2) +
                             std::pow(inv.mh[0] * inv.mh[1], 2);
        denominator -= std::pow(inv.mh[2] * inv.mh[1], 2);
        denominator -= std::pow(inv.mh[2] * inv.mh[0], 2);

        double s132 = numerator / denominator;

        // DEBUG
        // printf("numerator = %16.8e\n", numerator);
        // printf("numerator = %16.8e\n", denominator);
        // printf("s132_old = %16.8e\n", s132Old);
        // printf("s132_new = %16.8e\n", s132);

        double tan2Phi =
            imag(conj(Z5) * Z6 * Z6) /
            (real(conj(Z5) * Z6 * Z6) +
             pow(std::norm(Z6), 2) * v2 / (inv.mh[2] * inv.mh[2] - Z1 * v2));

        // s13 needs to be sizeable to not run into numerical problems.
        // Therefore we check if it is, before calculating inv.c13 and inv.cPhi.
        // In case it is not, we set them manually to 1.
        static double SIZABLE = 1e-15;
        double s13 = 0.;
        if (s132 > SIZABLE) {
            s13 = -std::sqrt(s132);  // s13 is negative for ordered masses.
            inv.c13 = std::sqrt(1. - s132);

            // The angle Phi in [0,pi] is calculated by first calculating
            // tan2Phi and sinPhi with eq. C22 & C23 in Phys.Rev.D 74, 015018.
            // With those, one can then determine the sign of cos(phi) in the
            // end.

            // This will always be positive for ordered masses:
            double sinPhi = (Z1 * v2 - inv.mh[2] * inv.mh[2]) *
                            (s13 / inv.c13) / (std::abs(Z6) * v2);

            // printf("sinPhi = %16.8e\n", sinPhi); // DEBUG

            // Fixing cos(Phi):
            static double ONE_LIMIT = 1. - SIZABLE;
            if (sinPhi > 0.7071067811865475 /* =sin(pi/4) */) {
                if (sinPhi > ONE_LIMIT)
                    inv.cPhi = 0.;
                else if (tan2Phi < 0.)
                    inv.cPhi = sqrt(1. - sinPhi * sinPhi);
                else
                    inv.cPhi = -sqrt(1. - sinPhi * sinPhi);
            } else if (std::abs(sinPhi) < SIZABLE)
                inv.cPhi = 1.;
            else {
                if (sinPhi < -ONE_LIMIT)
                    inv.cPhi = 0.;
                else if (tan2Phi < 0.)
                    inv.cPhi = -sqrt(1. - sinPhi * sinPhi);
                else
                    inv.cPhi = sqrt(1. - sinPhi * sinPhi);
            }

            // DEBUG
            // printf("s13 = %16.8e\n", s13);
            // printf("s132 = %16.8e\n", s132);
            // printf("c13 = %16.8e\n", inv.c13);
            // printf("sinPhi = %16.8e\n", sinPhi);
            // printf("cosPhi = %16.8e\n", inv.cPhi);
        } else {
            inv.c13 = 1.;

            if (tan2Phi < 0)
                inv.cPhi = -1.;
            else
                inv.cPhi = 1.;
        }

        // The angle s12 is determined from eq.C25 in Phys.Rev.D 74, 015018.
        // Here, we also make sure that the angle is sizeable for numerical
        // reasons.
        double s122 = ((Z1 * v2 - inv.mh[0] * inv.mh[0]) *
                           (inv.mh[2] * inv.mh[2] - Z1 * v2) -
                       std::abs(Z6) * std::abs(Z6) * v2 * v2) /
                      (inv.c13 * inv.c13 *
                       (inv.mh[1] * inv.mh[1] - inv.mh[0] * inv.mh[0]) *
                       (inv.mh[2] * inv.mh[2] - inv.mh[1] * inv.mh[1]));

        if (s122 < SIZABLE) {
            inv.s12 = 0.;
        } else /*s122 > 0.*/
        {
            inv.s12 = sign(inv.cPhi) *
                      sign(inv.mh[1] * inv.mh[1] - inv.mh[0] * inv.mh[0]) *
                      sqrt(s122);
        }
    }
    // Charged Higgs:
    if (Y2 + 0.5 * Z3 * v2 < 0) {
        // cout << "[WARNING]: Encountered negative mass for charged Higgs!\n";
        // //
        // DEBUG
        inv.mHc = 0.;
    } else
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

Base_compact Base_higgs::convert_to_compact() const {
    Base_compact compact = convert_to_generic().convert_to_compact();
    return compact;
}

vector<double> Base_higgs::convert_to_vector() const {
    return vector<double>{xi,       beta,     tan(beta), Y1,       Y2,
                          real(Y3), imag(Y3), Z1,        Z2,       Z3,
                          Z4,       real(Z5), imag(Z5),  real(Z6), imag(Z6),
                          real(Z7), imag(Z7)};
}

//--Base_invariant--------------------------------------------------------------

Base_generic Base_invariant::convert_to_generic(const double &v2) const {
    Base_generic gen = convert_to_higgs(v2).convert_to_generic();
    return gen;
}

Base_compact Base_invariant::convert_to_compact(const double &v2) const {
    Base_compact compact = convert_to_generic(v2).convert_to_compact();
    return compact;
}

Base_higgs Base_invariant::convert_to_higgs(const double &v2) const {
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

    for (int i = 0; i < 3; ++i) massDiag(i, i) = mh[i] * mh[i];

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

Eigen::Matrix3d Base_invariant::get_R() const {
    Eigen::Matrix3d mMatrix(3, 3);
    mMatrix.setZero();

    double s13 = -std::sqrt(std::abs(1. - c13 * c13));
    double c12 = std::sqrt(std::abs(1. - s12 * s12));
    double c23 = cos(theta23);
    double s23 = sin(theta23);
    mMatrix(0, 0) = c13 * c12;
    mMatrix(0, 1) = -c23 * s12 - c12 * s13 * s23;
    mMatrix(0, 2) = -c12 * c23 * s13 + s12 * s23;
    mMatrix(1, 0) = c13 * s12;
    mMatrix(1, 1) = c12 * c23 - s12 * s13 * s23;
    mMatrix(1, 2) = -c23 * s12 * s13 - c12 * s23;
    mMatrix(2, 0) = s13;
    mMatrix(2, 1) = c13 * s23;
    mMatrix(2, 2) = c13 * c23;

    return mMatrix;
}

vector<double> Base_invariant::convert_to_vector() const {
    return vector<double>{xi,    beta,  tan(beta), cPhi,        mHc,
                          mh[0], mh[1], mh[2],     s12,         c13,
                          Z2,    Z3,    theta23,   real(Z7inv), imag(Z7inv)};
}

//--Base_hybrid-----------------------------------------------------------------

void Base_hybrid::generate_random_softCpConserved(const gsl_rng *rng) {
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

Base_generic Base_hybrid::convert_to_generic(const double &v2) const {
    Base_generic gen;

    if (v2 < 0.1) {
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

Base_higgs Base_hybrid::convert_to_higgs(const double &v2) const {
    Base_higgs higgs;

    // Beware that some parts of 2HDMC uses different convention where sba is in
    // range -1 to 1. Here, sba is in 0 to 1 and cba is -1 to 1.
    double sba = sqrt(1. - cba * cba);

    static const double VSMALL = 1E-10;

    if ((mh < 0) || (std::abs(cba) > 1) || (std::abs(mh - mH) < VSMALL) ||
        (std::abs(tanb - 1.) < VSMALL)) {
        cout << "[ERROR]: Can't convert hybrid basis to Higgs basis\n";
        return higgs;
    }

    if (v2 < 0.1) {
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

Base_invariant Base_hybrid::convert_to_invariant(const double &v2) const {
    return convert_to_higgs(v2).convert_to_invariant(v2);
}

std::vector<double> Base_hybrid::convert_to_vector() const {
    return std::vector<double>{xi, beta, tanb, mh, mH, cba, Z4, Z5, Z7};
}

//--Base_physical--------------------------------------------------------------

Base_generic Base_physical::convert_to_generic(const double &v2) const {
    Base_generic gen;

    if (v2 < 0.1) {
        cout << "[ERROR]: Invalid v2 given to convert_to_generic().\n";
        return gen;
    }

    gen.M12 = complex<double>(M12, 0.);
    gen.Lambda6 = complex<double>(0., 0.);
    gen.Lambda7 = complex<double>(0., 0.);

    gen.beta = atan(tanb);
    gen.xi = 0.;

    double sb = sin(gen.beta);
    double sb2 = sb * sb;
    double cb = cos(gen.beta);
    double cb2 = cb * cb;
    double tb = tan(gen.beta);
    double ctb = 1. / tb;

    double alpha = -asin(sba) + gen.beta;
    double sa = sin(alpha);
    double sa2 = sa * sa;
    double ca = cos(alpha);
    double ca2 = ca * ca;

    double cba = std::sqrt(std::abs(1. - sba * sba));

    gen.Lambda1 = (mH * mH * ca2 + mh * mh * sa2 - M12 * tb) / v2 / cb2 -
                  1.5 * lambda6 * tb + 0.5 * lambda7 * tb * tb * tb;
    gen.Lambda2 = (mH * mH * sa2 + mh * mh * ca2 - M12 * ctb) / v2 / sb2 +
                  0.5 * lambda6 * ctb * ctb * ctb - 1.5 * lambda7 * ctb;
    gen.Lambda3 =
        ((mH * mH - mh * mh) * ca * sa + 2. * mHc * mHc * sb * cb - M12) / v2 /
            sb / cb -
        0.5 * lambda6 * ctb - 0.5 * lambda7 * tb;
    gen.Lambda4 = ((mA * mA - 2. * mHc * mHc) * cb * sb + M12) / v2 / sb / cb -
                  0.5 * lambda6 * ctb - 0.5 * lambda7 * tb;
    gen.Lambda5 = complex<double>((M12 - mA * mA * sb * cb) / v2 / sb / cb -
                                      0.5 * lambda6 * ctb - 0.5 * lambda7 * tb,
                                  0.);

    gen.M112 =
        M12 * tanb -
        0.5 * v2 *
            (gen.Lambda1 * cb * cb +
             (gen.Lambda3 + gen.Lambda4 + real(gen.Lambda5)) * sb * sb +
             (2. * lambda6 + lambda6) * sb * cb + lambda7 * sb * sb * tanb);

    gen.M222 =
        -0.5 / sb * (pow(mh, 2) * ca * sba + pow(mH, 2) * sa * cba) + M12 * ctb;
    return gen;
}

Base_higgs Base_physical::convert_to_higgs(const double &v2) const {
    return convert_to_generic(v2).convert_to_higgs();
}

Base_invariant Base_physical::convert_to_invariant(const double &v2) const {
    return convert_to_higgs(v2).convert_to_invariant(v2);
}

//--Base_c2hdm-----------------------------------------------------------------

Base_c2hdm::Base_c2hdm() {
    type = C2HDM;
    xi = 0.;
    mh.resize(2);
    alpha.resize(3);
}

Base_generic Base_c2hdm::convert_to_generic(const double &v2) const {
    Base_generic gen;
    gen.beta = atan(tanb);
    gen.xi = xi;
    gen.M12 = complex<double>(0.5 * M12, 0.);

    double c1 = cos(alpha[0]);
    double c2 = cos(alpha[1]);
    double c3 = cos(alpha[2]);
    double s1 = sin(alpha[0]);
    double s2 = sin(alpha[1]);
    double s3 = sin(alpha[2]);

    double cb = cos(gen.beta);
    double sb = std::sqrt(1. - cb * cb);

    double nu = M12 / (2. * cb * sb);

    double m1 = mh[0] * mh[0];
    double m2 = mh[1] * mh[1];

    Eigen::Matrix3d R;
    R.setZero();

    R(0, 0) = c1 * c2;
    R(0, 1) = s1 * c2;
    R(0, 2) = s2;
    R(1, 0) = -(c1 * s2 * s3 + s1 * c3);
    R(1, 1) = c1 * c3 - s1 * s2 * s3;
    R(1, 2) = c2 * s3;
    R(2, 0) = -c1 * s2 * c3 + s1 * s3;
    R(2, 1) = -(c1 * s3 + s1 * s2 * c3);
    R(2, 2) = c2 * c3;

    double m3 = (m1 * R(0, 2) * (R(0, 1) * tanb - R(0, 0)) +
                 m2 * R(1, 2) * (R(1, 1) * tanb - R(1, 0))) /
                (R(2, 2) * (R(2, 0) - R(2, 1) * tanb));

    gen.Lambda1 =
        (c1 * c1 * c2 * c2 * m1 + std::pow(c1 * s2 * s3 + s1 * c3, 2) * m2 +
         std::pow(c1 * s2 * c3 - s1 * s3, 2) * m3 - sb * sb * nu) /
        (cb * cb * v2);

    gen.Lambda2 =
        (s1 * s1 * c2 * c2 * m1 + std::pow(c1 * c3 - s1 * s2 * s3, 2) * m2 +
         std::pow(c1 * s3 + s1 * s2 * c3, 2) * m3 - cb * cb * nu) /
        (sb * sb * v2);

    gen.Lambda3 =
        (c1 * s1 * (c2 * c2 * m1 + (s2 * s2 * s3 * s3 - c3 * c3) * m2 +
                    (s2 * s2 * c3 * c3 - s3 * s3) * m3) +
         s2 * c3 * s3 * (c1 * c1 - s1 * s1) * (m3 - m2)) /
            (cb * sb * v2) +
        (2 * mHc * mHc - nu) / v2;

    gen.Lambda4 = (s2 * s2 * m1 + c2 * c2 * s3 * s3 * m2 +
                   c2 * c2 * c3 * c3 * m3 + nu - 2 * mHc * mHc) /
                  v2;

    gen.Lambda5 = std::complex<double>(
        (-s2 * s2 * m1 - c2 * c2 * s3 * s3 * m2 - c2 * c2 * c3 * c3 * m3 + nu) /
            v2,
        -(cb * (c1 * c2 * s2 * m1 - c2 * s3 * (c1 * s2 * s3 + s1 * c3) * m2 +
                c2 * c3 * (s1 * s3 - c1 * s2 * c3) * m3) +
          sb * (s1 * c2 * s2 * m1 + c2 * s3 * (c1 * c3 - s1 * s2 * s3) * m2 -
                c2 * c3 * (c1 * s3 + s1 * s2 * c3) * m3)) /
            (cb * sb * v2));
    gen.Lambda6 = std::complex<double>(0., 0.);
    gen.Lambda7 = std::complex<double>(0., 0.);

    return gen;
}

Base_higgs Base_c2hdm::convert_to_higgs(const double &v2) const {
    return convert_to_generic(v2).convert_to_higgs();
}

Base_invariant Base_c2hdm::convert_to_invariant(const double &v2) const {
    return convert_to_generic(v2).convert_to_invariant(v2);
}

std::vector<double> Base_c2hdm::convert_to_vector() const {
    return vector<double>{mh[0],    mh[1],    mHc,      M12,
                          alpha[0], alpha[1], alpha[2], tanb};
}
//-----------------------------------------------------------------------------

Base_s3::Base_s3(double v, double tb, double mh, double mH, double mA,
                 double mHc) {
    beta = atan(tb);
    tanb = tb;
    double s2b = sin(2. * beta);
    double v2 = v * v;
    mu12 = 0.5 * mH * mH * s2b;
    lambda3 = 0.5 * (2. * mu12 / s2b - mHc * mHc) / v2;
    lambda1 = 0.5 * (mh * mh) / v2 - lambda3;
    lambda2 = 0.5 * (2. * mu12 / s2b - mA * mA) / v2 - lambda3;
    // Tadpoles
    mu1 = mu12 * tanb - (lambda1 + lambda3) * v2;
    mu2 = mu12 / tanb - (lambda1 + lambda3) * v2;
}
Base_generic Base_s3::convert_to_generic(const double &v2) const {
    Base_generic gen;
    gen.beta = atan(tanb);
    gen.xi = xi;
    gen.M12 = mu12;
    gen.M112 = mu1;
    gen.M222 = mu2;
    gen.Lambda1 = 2. * (lambda1 + lambda3);
    gen.Lambda2 = 2. * (lambda1 + lambda3);
    gen.Lambda3 = 2. * (lambda1 - lambda3);
    gen.Lambda4 = 2. * (lambda3 - lambda2);
    gen.Lambda5 = 2. * (lambda2 + lambda3);
    gen.Lambda6 = 0.;
    gen.Lambda7 = 0.;
    return gen;
}
Base_compact Base_s3::convert_to_compact(const double &v2) const {
    return convert_to_generic(v2).convert_to_compact();
}
Base_higgs Base_s3::convert_to_higgs(const double &v2) const {
    return convert_to_generic(v2).convert_to_higgs();
}
Base_invariant Base_s3::convert_to_invariant(const double &v2) const {
    return convert_to_generic(v2).convert_to_invariant(v2);
}
}  // namespace THDME
