// HelpFunctions.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Miscellaneous functions
 * @author: Joel Oredsson
 *============================================================================*/
#include "HelpFunctions.h"
#include "EDM.h"
#include "Globals.h"

#if defined VEVACIOUS
#include "Vevacious.h"
#endif

#include <gsl/gsl_errno.h>
#include <gsl/gsl_rng.h>
#include <sys/stat.h>
#include <time.h>
#include <Eigen/Dense>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "unistd.h"

using namespace std;

namespace THDME {

double readLine(std::ifstream &ifStream, unsigned int wordID) {
    std::string line, temp;
    std::getline(ifStream, line);
    std::istringstream iss(line);

    for (unsigned int i = 1; i < wordID; ++i) {
        iss >> temp;
    }
    double number;
    iss >> number;
    return number;
}

bool file_exists(const std::string &file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

std::string extract_folder_path(const std::string &path) {
    int slashPosition = 0;
    std::string newFolder;
    std::string find = "/";
    for (int i = path.length() - 1; i > -1; i--) {
        if (path.at(i) == find.at(0)) {
            slashPosition = i;
            break;
        }
    }
    newFolder.assign(path, 0, slashPosition);
    return newFolder;
}

bool create_folder_path(const std::string &path) {
    std::string newFolder = "mkdir -p " + path;

    char *cstr = new char[newFolder.length() + 1];
    std::strcpy(cstr, newFolder.c_str());
    const int dir_err = system(cstr);
    if (-1 == dir_err) {
        std::cout << "Error creating directory!\n";
        return false;
    }
    return true;
}

bool change_directory(const std::string &cdCommand) {
    if (chdir(cdCommand.c_str()) == -1) return false;
    return true;
}

//------------------------------------------------------------------------------

Eigen::Matrix3cd randomMatrix3cd(gsl_rng *rng) {
    Eigen::Matrix3cd randomMatrix;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            randomMatrix(i, j) =
                gsl_rng_uniform(rng) *
                std::polar(1., gsl_rng_uniform(rng) * 2. * M_PI);
        }
    }
    return randomMatrix;
}

//------------------------------------------------------------------------------

// LO RG evolution of alphaS=g3^3/4pi
double alpha_s(const double &mu) {
    static std::vector<double> matchingScales = {Global::mb, Global::mt};

    if (Global::mb <= mu && mu <= Global::mt) {
        double b0 = (33. - 2. * 5) / (12. * M_PI);
        return Global::alpha_s_mZ /
               (1. +
                Global::alpha_s_mZ * b0 *
                    std::log(mu * mu / (Global::mZ * Global::mZ)));
    } else if (mu > Global::mt) {
        const static double b0 = (33. - 2. * 5) / (12. * M_PI);
        const static double b0_6 = (33. - 2. * 6) / (12. * M_PI);
        double alpha_s_mt =
            Global::alpha_s_mZ /
            (1. +
             Global::alpha_s_mZ * b0 *
                 std::log(Global::mt * Global::mt / (Global::mZ * Global::mZ)));
        return alpha_s_mt / (1. +
                             alpha_s_mt * b0_6 *
                                 std::log(mu * mu / (Global::mt * Global::mt)));
    } else if (1. < mu && mu < Global::mb) {
        const static double b0 = (33. - 2. * 5) / (12. * M_PI);
        const static double b0_4 = (33. - 2. * 4) / (12. * M_PI);
        const static double alpha_s_mb =
            Global::alpha_s_mZ /
            (1. +
             Global::alpha_s_mZ * b0 *
                 std::log(Global::mb * Global::mb / (Global::mZ * Global::mZ)));
        return alpha_s_mb / (1. +
                             alpha_s_mb * b0_4 *
                                 std::log(mu * mu / (Global::mb * Global::mb)));
    } else
        std::cout << "[ERROR]: Wrong argument in alpha_s(mu).\n";

    return 0.;
}

void print_prog_bar(int percent) {
    std::string bar;
    static int barLength = 10;
    static int barPercent = 100 / barLength;

    for (int i = 0; i < barLength; i++) {
        if (i < (percent / barPercent)) {
            bar.replace(i, 1, "=");
        } else if (i == (percent / barPercent)) {
            bar.replace(i, 1, ">");
        } else {
            bar.replace(i, 1, " ");
        }
    }

    std::cout << "\r"
                 "["
              << bar << "] ";
    std::cout.width(3);
    std::cout << percent << "%";
}

void print_prog_number(const std::string message, const double num) {
    std::cout.width(20);
    std::cout << std::flush;

    std::cout << "\r" << message << ": ";
    std::cout.width(12);
    std::cout << num << std::flush;
}

//------------------------------------------------------------------------------

// Converts double to a string with scientific notation for big numbers
std::string stringAuto(const double &number, const char *format) {
    // std::ostringstream streamObj; // Create an output string stream
    // streamObj << number;          // Add double to stream
    // return streamObj.str();       // Get string from output string stream

    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    if (format != nullptr)
        snprintf(buffer, sizeof(buffer), format, number);
    else
        snprintf(buffer, sizeof(buffer), "%12.4e", number);

    return std::string(buffer);
}

std::string stringAuto(const std::complex<double> &cNumber) {
    std::ostringstream streamObj;  // Create an output string stream
    streamObj << cNumber;          // Add double to stream
    return streamObj.str();        // Get string from output string stream
}

std::vector<std::string> convertToStringVec(
    const std::vector<double> doubleVec) {
    std::vector<std::string> stringVec;

    for (auto &number : doubleVec) {
        std::ostringstream streamObj;
        streamObj << number;
        stringVec.push_back(streamObj.str());
    }
    return stringVec;
}
//------------------------------------------------------------------------------

void programTime(bool exact) {
    // seconds of CPU time the program has occupied.
    // the actual time is sec/(number of threads).
    float sec = ((float)clock()) / CLOCKS_PER_SEC;
    static time_t timeZero = time(NULL);

    // Conversions
    int min = (int)floor(sec / 60);
    int hours = (int)floor(min / 60);
    min %= 60;
    sec = sec - 60 * 60 * hours - 60 * min;
    if (exact)
        printf(" CPU time = %i h %i m %f s.\n", hours, min, sec);
    else
        printf(" CPU time = %i h %i m %.f s.\n", hours, min, sec);

    float seconds = difftime(time(NULL), timeZero);
    min = (int)floor(seconds / 60);
    hours = (int)floor(min / 60);
    min %= 60;
    seconds = seconds - 60 * 60 * hours - 60 * min;
    printf("Real time = %i h %i m %.f s.\n", hours, min, seconds);
}

Timer::Timer() : _message("Timer:") {
    _start = std::chrono::high_resolution_clock::now();
}

Timer::Timer(const std::string &message) : _message(message) {
    _start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    // std::cout << "Destroying timer!\n"; // DEBUG
    printDuration();
    printf("\nFinished %s!\n", _message.c_str());
}

float Timer::getSeconds() const {
    std::chrono::duration<float> duration =
        std::chrono::high_resolution_clock::now() - _start;
    return duration.count();
}

void Timer::printDuration() const {
    std::chrono::duration<float> duration =
        std::chrono::high_resolution_clock::now() - _start;
    float seconds = duration.count();
    int min = (int)floor(seconds / 60);
    int hours = (int)floor(min / 60);
    min %= 60;
    seconds = seconds - 60 * 60 * hours - 60 * min;
    printf("%s", _message.c_str());
    printf(" %i h %i m %.3f s.\n", hours, min, seconds);
}

std::string date() {
    std::time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buf[100] = {0};
    std::strftime(buf, sizeof(buf), "%d %h %Y at %H:%M", std::localtime(&now));
    std::string dateString(buf);

    return dateString;
}

//------------------------------------------------------------------------------

int jac(double t, const double y[], double *dfdy, double dfdt[], void *params) {
    return GSL_SUCCESS;
}

double RAND() {
    static gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);
    static bool initialized = false;
    if (!initialized) {
        int seed = time(0);
        gsl_rng_set(rng, seed);  // Seed the random number generator
        initialized = true;
    }
    return gsl_rng_uniform(rng);
}
//------------------------------------------------------------------------------

void BiUnitary(Eigen::Matrix3cd &kU, Eigen::Matrix3cd &kD, Eigen::Matrix3cd &kL,
               Eigen::Matrix3cd &rU, Eigen::Matrix3cd &rD, Eigen::Matrix3cd &rL,
               Eigen::Matrix3cd &VCKM, double mup[], double mdn[], double ml[],
               double &v2) {
    using namespace std;
    using namespace Eigen;

    // Minimum value of fermion mass squared before regarded as massless.
    static constexpr double MIN_M2 = 1e-15;

    // Temporary matrices of kappa and rho matrices. These should be the
    // potentially non-diagonal matrices in the fermion flavor eigenbasis.
    Matrix3cd kU_temp = kU;
    Matrix3cd kD_temp = kD;
    Matrix3cd kL_temp = kL;
    Matrix3cd rU_temp = rU;
    Matrix3cd rD_temp = rD;
    Matrix3cd rL_temp = rL;

    // Find biunitary transformations

    // Leptons
    ComplexEigenSolver<Matrix3cd> kLHkL;
    kL.setZero();
    Matrix3cd kL_inverse;
    kL_inverse.setZero();
    kLHkL.compute(kL_temp.adjoint() * kL_temp);
    for (unsigned int i = 0; i < 3; i++) {
        double ml2 = real(kLHkL.eigenvalues()[i]);
        kL(i, i) = ml2 > MIN_M2 ? sqrt(ml2) : 0.;
        kL_inverse(i, i) = ml2 > MIN_M2 ? 1. / kL(i, i) : 1.;
        ml[i] = real(kL(i, i)) * sqrt(v2 / 2.);
    }
    if (ml[0] < MIN_M2 && ml[1] < MIN_M2 && ml[2] < MIN_M2)
        rL.setZero();
    else {
        ComplexEigenSolver<Matrix3cd> kLkLH;
        kLkLH.compute(kL_temp * kL_temp.adjoint());
        Matrix3cd VL_L = kLkLH.eigenvectors().adjoint();
        Matrix3cd VL_R = kL_inverse * VL_L * kL_temp;
        rL = VL_L * rL_temp * VL_R.adjoint();
    }

    if (kU.isZero(1e-10) && kD.isZero(1e-10)) {
        kU.setZero();
        kD.setZero();
        rU.setZero();
        rD.setZero();
        for (int f = 0; f < 3; ++f) {
            mup[f] = 0.;
            mdn[f] = 0.;
        }
        VCKM.setIdentity();
    } else {
        // Up-Quarks
        ComplexEigenSolver<Matrix3cd> kUHkU;
        kU.setZero();
        Matrix3cd kU_inverse;
        kU_inverse.setZero();
        kUHkU.compute(kU_temp.adjoint() * kU_temp);
        for (unsigned int i = 0; i < 3; i++) {
            double mup2 = real(kUHkU.eigenvalues()[i]);
            kU(i, i) = mup2 > MIN_M2 ? sqrt(mup2) : 0.;
            kU_inverse(i, i) = mup2 > MIN_M2 ? 1. / kU(i, i) : 1.;
            mup[i] = real(kU(i, i)) * sqrt(v2 / 2.);
        }
        ComplexEigenSolver<Matrix3cd> kUkUH;
        kUkUH.compute(kU_temp * kU_temp.adjoint());
        Matrix3cd VU_L = kUkUH.eigenvectors().adjoint();
        Matrix3cd VU_R = kU_inverse * VU_L * kU_temp;

        // Down-Quarks
        ComplexEigenSolver<Matrix3cd> kDHkD;
        kD.setZero();
        Matrix3cd kD_inverse;
        kD_inverse.setZero();
        kDHkD.compute(kD_temp.adjoint() * kD_temp);
        for (unsigned int i = 0; i < 3; i++) {
            double mdn2 = real(kDHkD.eigenvalues()[i]);
            kD(i, i) = mdn2 > MIN_M2 ? sqrt(mdn2) : 0.;
            kD_inverse(i, i) = mdn2 > MIN_M2 ? 1. / kD(i, i) : 1.;
            mdn[i] = real(kD(i, i)) * sqrt(v2 / 2.);
        }
        ComplexEigenSolver<Matrix3cd> kDkDH;
        kDkDH.compute(kD_temp * kD_temp.adjoint());
        Matrix3cd VD_L = kDkDH.eigenvectors().adjoint();
        Matrix3cd VD_R = kD_inverse * VD_L * kD_temp;

        // Calculate new temporary CKM matrix
        VCKM = VU_L * VD_L.adjoint();

        /**
         * CKM conventions.
         *
         * If Global::CKM_PDG_CONVENTION=true, then the CKM matrix is
         * transformed.
         *
         * We will rephase the quarks to get the CKM matrix to
         * the PDG phase convention. This is done with two diagonal
         * unitary matrices that transforms the left and right
         * handed quarks.
         */
        Matrix3cd P_U, P_D;
        if (Global::CKM_PDG_CONVENTION) {
            toPdgConventions(VCKM, P_U, P_D);
        } else {
            P_U.setIdentity();
            P_D.setIdentity();
        }
        // The rho matrices are also transformed.
        rU = P_U * VU_L * rU_temp * VU_R.adjoint() * P_U.adjoint();
        rD = P_D * VD_L * rD_temp * VD_R.adjoint() * P_D.adjoint();
    }
}
void toPdgConventions(Eigen::Matrix3cd &VCKM) {
    Eigen::Matrix3cd PU, PD;
    toPdgConventions(VCKM, PU, PD);
}

void toPdgConventions(Eigen::Matrix3cd &VCKM, Eigen::Matrix3cd &PU,
                      Eigen::Matrix3cd &PD) {
    PU.setZero();
    PD.setZero();
    Eigen::Matrix3cd VCKM_temp = VCKM;
    // We need to derive the phases of these matrices, P_U, P_D.
    double s13 = std::abs(VCKM(0, 2));
    double c13 = sqrt(1. - s13 * s13);
    double s23 = std::abs(VCKM(1, 2)) / c13;
    double c23 = sqrt(1. - s23 * s23);
    double s12 = std::abs(VCKM(0, 1)) / c13;
    double c12 = sqrt(1. - s12 * s12);
    double cosdelta =
        (s12 * s12 * s23 * s23 + c12 * c12 * c23 * c23 * s13 * s13 -
         std::abs(VCKM(2, 0)) * std::abs(VCKM(2, 0))) /
        (2. * s12 * s23 * c12 * c23 * s13);
    double sindelta = sqrt(std::abs(1. - cosdelta * cosdelta));
    std::complex<double> eIdelta = std::complex<double>(cosdelta, sindelta);

    PD(0, 0) = VCKM(0, 0) / std::abs(VCKM(0, 0));
    PD(1, 1) = VCKM(0, 1) / std::abs(VCKM(0, 1));
    PD(2, 2) = eIdelta * VCKM(0, 2) / std::abs(VCKM(0, 2));

    PU(0, 0) = std::complex<double>(1., 0.);
    PU(1, 1) = conj(VCKM(1, 2)) * PD(2, 2) / std::abs(VCKM(1, 2));
    PU(2, 2) = conj(VCKM(2, 2)) * PD(2, 2) / std::abs(VCKM(2, 2));

    // The CKM matrix transforms during quark rephasing as
    VCKM = PU * VCKM_temp * PD.adjoint();
}
void getCkmParams(const Eigen::Matrix3cd &VCKM, double &s12, double &s13,
                  double &s23, double &delta) {
    s13 = std::abs(VCKM(0, 2));
    double c13 = sqrt(1. - s13 * s13);
    s23 = std::abs(VCKM(1, 2)) / c13;
    double c23 = sqrt(1. - s23 * s23);
    s12 = std::abs(VCKM(0, 1)) / c13;
    double c12 = sqrt(1. - s12 * s12);
    double cosdelta =
        (s12 * s12 * s23 * s23 + c12 * c12 * c23 * c23 * s13 * s13 -
         std::abs(VCKM(2, 0)) * std::abs(VCKM(2, 0))) /
        (2. * s12 * s23 * c12 * c23 * s13);
    double sindelta = sqrt(std::abs(1. - cosdelta * cosdelta));
    std::complex<double> eIdelta = std::complex<double>(cosdelta, sindelta);
    delta = std::arg(eIdelta);
}

Eigen::Matrix3cd createCkmMatrix(double lambda, double A, double rhobar,
                                 double etabar) {
    double s12 = lambda;
    double s23 = A * lambda * lambda;
    double c12 = sqrt(1. - s12 * s12);
    double c23 = sqrt(1. - s23 * s23);
    double preFactor =
        (A * pow(lambda, 3) * sqrt(1. - A * A * pow(lambda, 4))) /
        ((1. - lambda * lambda) *
         (1. - 2. * A * A * pow(lambda, 4) * rhobar +
          pow(A, 4) * pow(lambda, 8) * (pow(rhobar, 2) + pow(etabar, 2))));
    const std::complex<double> s13eid = std::complex<double>(
        preFactor *
            (rhobar -
             A * A * pow(lambda, 4) * (pow(rhobar, 2) + pow(etabar, 2))),
        preFactor * etabar);
    double c13 = std::sqrt(std::abs(1. - std::abs(s13eid) * std::abs(s13eid)));

    Eigen::Matrix3cd CKM;
    CKM.setZero();
    CKM(0, 0) = std::complex<double>(c12 * c13, 0.);
    CKM(0, 1) = std::complex<double>(s12 * c13, 0.);
    CKM(0, 2) = conj(s13eid);
    CKM(1, 0) = std::complex<double>(-s12 * c23 - c12 * s23 * real(s13eid),
                                     -c12 * s23 * imag(s13eid));
    CKM(1, 1) = std::complex<double>(c12 * c23 - s12 * s23 * real(s13eid),
                                     -s12 * s23 * imag(s13eid));
    CKM(1, 2) = std::complex<double>(s23 * c13, 0.);
    CKM(2, 0) = std::complex<double>(s12 * s23 - c12 * c23 * real(s13eid),
                                     -c12 * c23 * imag(s13eid));
    CKM(2, 1) = std::complex<double>(-c12 * s23 - s12 * c23 * real(s13eid),
                                     -s12 * c23 * imag(s13eid));
    CKM(2, 2) = std::complex<double>(c23 * c13, 0.);
    return CKM;
}

Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> EigenSolver(
    double v2, const Base_higgs &higgs) {
    Eigen::Matrix3d mMatrix(3, 3);
    mMatrix.setZero();
    mMatrix << v2 * higgs.Z1, v2 * real(higgs.Z6), -v2 * imag(higgs.Z6),
        v2 * real(higgs.Z6),
        v2 * 0.5 * (higgs.Z3 + higgs.Z4 + real(higgs.Z5)) + higgs.Y2,
        -v2 * 0.5 * imag(higgs.Z5), -v2 * imag(higgs.Z6),
        -v2 * 0.5 * imag(higgs.Z5),
        v2 * 0.5 * (higgs.Z3 + higgs.Z4 - real(higgs.Z5)) + higgs.Y2;

    // Set up an EigenSolver for the mass matrix. Since the matrix is symmetric,
    // it gives real eigenvalues that are ordered.
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> M(mMatrix);

    return M;
}

Eigen::Matrix3d get_rotation_matrix(double v2, const Base_higgs &higgs) {
    auto M = EigenSolver(v2, higgs);
    Eigen::Matrix3d R = M.eigenvectors().adjoint().eval();

    // The rotation matrix is not unique. Here we put it in our conventional
    // form:
    if (R.determinant() < 0) R *= -1;
    if (R(0, 0) < 0)
        for (int i = 0; i < 3; ++i) R(0, i) *= -1;
    if (R(2, 0) > 0)
        for (int i = 0; i < 3; ++i) R(2, i) *= -1;
    if (R.determinant() < 0)
        for (int i = 0; i < 3; ++i) R(1, i) *= -1;
    return R;
}

bool fix_tadpole_real_vev(Base_generic &gen, const double &v2) {
    using namespace std;

    gen.xi = 0.;

    const double sinb = sin(gen.beta);
    const double cosb = std::sqrt(1.0 - sinb * sinb);
    const double tanb = tan(gen.beta);
    const double cotb = 1. / tanb;

    static constexpr double MACHINE_PRECISION = 1e-12;

    double M12 = std::abs(gen.M12);

    if (M12 < MACHINE_PRECISION || v2 < MACHINE_PRECISION) {
        std::cout
            << "[ERROR]: M12 or v2 too small in fix_tadpole_real_vev().\n";
        return false;
    }
    // Tree-lvl minimum from eq.(A4-A5) and eq.(A7) in PhysRevD.72.035004
    if (abs(gen.beta) > MACHINE_PRECISION) {
        double sinArgM12 = 0.5 * v2 * (std::imag(gen.Lambda5) * sinb * cosb +
                                       std::imag(gen.Lambda6) * cosb * cosb +
                                       std::imag(gen.Lambda7) * sinb * sinb) /
                           M12;
        if (std::abs(sinArgM12) > 1.) {
            //std::cout << "[ERROR]: No solution for arg(M12) in "
            //             "fix_tadpole_real_vev().\n";  // DEBUG
            return false;
        }
        double cosArgM12 = std::abs(1. - sinArgM12 * sinArgM12);
        gen.M12 = std::complex<double>(M12 * cosArgM12, M12 * sinArgM12);

        gen.M112 =
            real(gen.M12 * tanb -
                 0.5 * v2 *
                     (gen.Lambda1 * cosb * cosb +
                      (gen.Lambda3 + gen.Lambda4 + gen.Lambda5) * sinb * sinb +
                      (2. * gen.Lambda6 + conj(gen.Lambda6)) * sinb * cosb +
                      gen.Lambda7 * sinb * sinb * tanb));

        gen.M222 = real(
            conj(gen.M12) * cotb -
            0.5 * v2 *
                (gen.Lambda2 * sinb * sinb +
                 (gen.Lambda3 + gen.Lambda4 + conj(gen.Lambda5)) * cosb * cosb +
                 (gen.Lambda7 + 2. * conj(gen.Lambda7)) * sinb * cosb +
                 conj(gen.Lambda6) * cosb * cosb * cotb));
    }  // namespace THDME
    else {
        gen.M112 = -0.5 * gen.Lambda1 * v2;
        gen.M12 = -0.5 * gen.Lambda6 * v2;
    }

    return true;
}

bool satisfies_tadpole_equations(const Base_generic &gen,
                                 const std::complex<double> v1,
                                 const std::complex<double> v2) {
    using namespace std;
    double sinb = sin(gen.beta);
    double cosb = std::sqrt(1.0 - sinb * sinb);
    double tanb = tan(gen.beta);
    double cotb = 1. / tanb;
    double V2 = std::norm(v1) + std::norm(v2);

    // Tree-lvl minimum from eq.(A4-A5) and eq.(A7) in PhysRevD.72.035004
    double tadpole[3] = {0., 0., 0.};
    tadpole[0] =
        gen.M112 -
        real(gen.M12 * std::polar(1., gen.xi) * tanb -
             0.5 * V2 *
                 (gen.Lambda1 * cosb * cosb +
                  (gen.Lambda3 + gen.Lambda4 +
                   gen.Lambda5 * std::polar(1., 2. * gen.xi)) *
                      sinb * sinb +
                  (2. * gen.Lambda6 * std::polar(1., gen.xi) +
                   conj(gen.Lambda6) * std::polar(1., -gen.xi)) *
                      sinb * cosb +
                  gen.Lambda7 * sinb * sinb * tanb * std::polar(1., gen.xi)));

    tadpole[1] =
        gen.M222 -
        real(conj(gen.M12 * std::polar(1., gen.xi)) * cotb -
             0.5 * V2 * (gen.Lambda2 * sinb * sinb +
                         (gen.Lambda3 + gen.Lambda4 +
                          conj(gen.Lambda5) * std::polar(1., -2. * gen.xi)) *
                             cosb * cosb +
                         (gen.Lambda7 * std::polar(1., gen.xi) +
                          2. * conj(gen.Lambda7) * std::polar(1., -gen.xi)) *
                             sinb * cosb +
                         conj(gen.Lambda6) * cosb * cosb *
                             std::polar(1., -gen.xi) * cotb));
    tadpole[2] =
        std::imag(gen.M12 * std::polar(1., gen.xi)) -
        0.5 * V2 *
            (std::imag(gen.Lambda5 * std::polar(1., 2. * gen.xi)) * sinb *
                 cosb +
             std::imag(gen.Lambda6 * std::polar(1., gen.xi)) * cosb * cosb +
             std::imag(gen.Lambda7 * std::polar(1., gen.xi)) * sinb * sinb);

    printf("t1 = %16.8e\n", tadpole[0]);
    printf("t2 = %16.8e\n", tadpole[1]);
    printf("t3 = %16.8e\n", tadpole[2]);
    return true;
}

void export_potential_csv(const Base_higgs &higgs,
                          const std::string &fileName) {
    auto potential = [higgs](double v2) {
        return higgs.Y1 * 0.5 * v2 + 0.25 * 0.5 * higgs.Z1 * v2 * v2;
    };
    create_folder_path("output/potentials");
    std::ofstream myfile;
    myfile.open("output/potentials/" + fileName + ".csv");
    myfile << "v"
           << "\t"
           << "pot"
           << "\n";

    constexpr static double END_SCALE = 1e6;
    constexpr static int POINTS = 300;
    for (int i = 0; i < POINTS; ++i) {
        double v2 = i * END_SCALE / (POINTS - 1);
        myfile << std::sqrt(v2) << "\t" << potential(v2) << "\n";
    }
    myfile.close();
}

//--Table-class-----------------------------------------------------------------

bool Table::_colorMode = true;

Table::Table(const int columns)
    : _borderChar("||"),
      _lineChar("="),
      _frameColor(RED),
      _columns(columns),
      _rows(0),
      _titleBool(false),
      _titleString("") {
    _columnSize.resize(columns, 0);
    _content.resize(columns);
}

Table::~Table() {}

void Table::add_row(const std::vector<std::string> &stringVec,
                    bool boldAndunderLine) {
    if (stringVec.size() > _columns) {
        std::cout << "[ERROR]: Couldn't add row to table.\n";
        return;
    }

    if (boldAndunderLine)
        _boldAndunderLine.push_back(true);
    else
        _boldAndunderLine.push_back(false);

    if (stringVec.size() != _columns)
        std::cout << "[WARNING]: number of strings does not match number of "
                     "columns!\n";

    for (unsigned int i = 0; i < stringVec.size(); i++) {
        _content[i].push_back(stringVec[i]);

        // Updates the column size if it should be increased.
        if (stringVec[i].length() > _columnSize[i])
            _columnSize[i] = stringVec[i].length();
    }

    ++_rows;
}

void Table::add_row(const std::vector<double> &doubleVec,
                    bool boldAndunderLine) {
    if (doubleVec.size() > _columns) {
        std::cout << "[ERROR]: Couldn't add row to table.\n";
        return;
    }

    if (boldAndunderLine)
        _boldAndunderLine.push_back(true);
    else
        _boldAndunderLine.push_back(false);

    if (doubleVec.size() != _columns)
        std::cout << "[WARNING]: number of doubles does not match number of "
                     "columns!\n";

    for (unsigned int i = 0; i < doubleVec.size(); i++) {
        _content[i].push_back(stringAuto(doubleVec[i]));

        // Updates the column size if it should be increased.
        if (_content[i].back().length() > _columnSize[i])
            _columnSize[i] = _content[i].back().length();
    }
    _rows++;
}

// Print the Table
void Table::print() const {
    std::string temp;
    int spaces = 0;
    int tableLength = 2 * _borderChar.length() + 1 + _columns;

    // Calculate table length
    for (unsigned int i = 0; i < _columns; i++) {
        tableLength += _columnSize[i];
    }

    if (_titleBool) drawTitle(tableLength);

    drawLine(tableLength, _lineChar);

    // Draw all rows
    for (unsigned int i = 0; i < _rows; i++) {
        drawColor(_borderChar + " ");

        for (unsigned int j = 0; j < _columns; j++) {
            /* First: calculate the number of spaces in the cell.
              The content is centered, so half spaces is printed first
              and half after the content. */
            spaces = 1 + _columnSize[j] - _content[j][i].length();
            for (int k = 0; k < spaces / 2; k++) {
                std::cout << " ";
            }

            // Incase the row should be boldfaced and with an underline
            if (_boldAndunderLine[i] && _colorMode) {
                temp = "\x1B[4m\x1B[1m" + _content[j][i] + "\x1B[0m";
                std::cout << temp;
            } else  // print in normal format
                std::cout << _content[j][i];

            for (int k = 0; k < spaces / 2; k++) {
                std::cout << " ";
            }  // Spaces after content

            // An extra space for uneven lengths
            if (spaces % 2 != 0) std::cout << " ";
        }

        drawColor(_borderChar + "\n");
    }

    drawLine(tableLength, _lineChar);
}

void Table::set_frame_color(const FrameColor &color) { _frameColor = color; }

void Table::set_frame_style(const std::string &line,
                            const std::string &border) {
    _lineChar = line;
    _borderChar = border;
}

void Table::set_title(const std::string &title) {
    _titleBool = true;
    _titleString = title;
}

void Table::drawTitle(int length) const {
    int spaces =
        1 + length - _titleString.length();  // Number of spaces in title
    for (int k = 0; k < spaces / 2; k++) {
        std::cout << " ";
    }  // Spaces before title
    std::string temp = _titleString;

    if (_colorMode) {
        temp.insert(0, "\x1B[4m\x1B[1m");
        temp.append("\x1B[0m");
    }
    std::cout << temp;

    for (int k = 0; k < spaces / 2; k++) {
        std::cout << " ";
    }  // Spaces after title

    if (spaces % 2 != 0) std::cout << " ";  // An extra space for uneven lengths

    std::cout << "\n";
}
void Table::drawColor(std::string text) const {
    if (_colorMode) {
        switch (_frameColor) {
            case RED:
                text.insert(0, "\x1B[31m");
                break;
            case BLUE:
                text.insert(0, "\x1B[34m");
                break;
            case GREEN:
                text.insert(0, "\x1B[32m");
                break;
            case YELLOW:
                text.insert(0, "\x1B[33m");
                break;
            default:
                break;
        }
        text.append("\x1B[0m");
    }
    std::cout << text;
}

void Table::change_colorMode(const bool mode) { Table::_colorMode = mode; }

void Table::drawLine(int length, const std::string &lineChar) const {
    for (int i = 0; i < length; i++) {
        drawColor(lineChar);
    }
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------

void printFancyBasesTable(const Base_generic &gen, double v2) {
    using namespace std;

    Base_higgs higgs = gen.convert_to_higgs();
    Base_invariant inv = gen.convert_to_invariant(v2);

    Table tab(6);
    tab.add_row(
        vector<string>{"Generic", "(tanb=" + stringAuto(tan(gen.beta)) + ")",
                       "Higgs", "", "Invariant", ""},
        true);
    tab.add_row(vector<string>{"M112 =", stringAuto(gen.M112),
                               "Y1 =", stringAuto(higgs.Y1),
                               "mh1 = ", stringAuto(inv.mh[0])});
    tab.add_row(vector<string>{"M222 =", stringAuto(gen.M222),
                               "Y2 =", stringAuto(higgs.Y2),
                               "mh2 = ", stringAuto(inv.mh[1])});
    tab.add_row(vector<string>{"M122 =", stringAuto(gen.M12),
                               "Y3 =", stringAuto(higgs.Y3),
                               "mh3 = ", stringAuto(inv.mh[2])});
    tab.add_row(vector<string>{"lam1 =", stringAuto(gen.Lambda1),
                               "Z1 =", stringAuto(higgs.Z1),
                               "mHc = ", stringAuto(inv.mHc)});
    tab.add_row(vector<string>{"lam2 =", stringAuto(gen.Lambda2),
                               "Z2 =", stringAuto(higgs.Z2),
                               "s12 = ", stringAuto(inv.s12)});
    tab.add_row(vector<string>{"lam3 =", stringAuto(gen.Lambda3),
                               "Z3 =", stringAuto(higgs.Z3),
                               "c13 = ", stringAuto(inv.c13)});
    tab.add_row(vector<string>{"lam4 =", stringAuto(gen.Lambda4),
                               "Z4 =", stringAuto(higgs.Z4),
                               " Z2 = ", stringAuto(inv.Z2)});
    tab.add_row(vector<string>{"lam5 =", stringAuto(gen.Lambda5),
                               "Z5 =", stringAuto(higgs.Z5),
                               " Z3 = ", stringAuto(inv.Z3)});
    tab.add_row(vector<string>{"lam6 =", stringAuto(gen.Lambda6),
                               "Z6 =", stringAuto(higgs.Z6),
                               " Z7 = ", stringAuto(inv.Z7inv)});
    tab.add_row(vector<string>{"lam7 =", stringAuto(gen.Lambda7),
                               "Z7 =", stringAuto(higgs.Z7),
                               "t23 = ", stringAuto(inv.theta23)});
    tab.print();
}

//-----------------------------------------------------------------------------
}  // namespace THDME
