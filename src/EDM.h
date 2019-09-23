// EDM.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Class to calculate electric dipole moments
 * @author: Joel Oredsson
 *
 * Class that calculates the Electrons Electric Dipole Moment (eEDM).
 * See arXiv:1909.05735 for computation details.
 * Many analytic expressions are taken from appendix A in phys.rev.D89 115023.
 *
 * By default, the class creates 6 THDM objects defined at the scales mt, mw,
 * mh1, mh2, mh3, mHc. This can however fail if it cannot perform any of the RG
 * evolutions (this is done in compute_thdms). In every loop computation, the
 * couplings are taken from the THDM defined at the heaviest participating
 * particle. One can turn off this running, which dramatically speeds up the
 * calculation, in the constructor by setting run = false.
 *
 * Example of usage:
 *   EDM edm(thdm);
 *   edm.print_edm();
 *
 * or, without running,
 *   EDM edm(thdm, false);
 *   edm.print_edm();
 *
 *============================================================================*/
#pragma once

#include "SLHA.h"
#include "THDM.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>
#include <complex>
#include <tuple>
#include <vector>

namespace THDME {

// Electric dipole moment class
class EDM {
   public:
    /*
    * @brief: Constructor
    *
    * The constructor sets all the parameters that are needed for computing all
    * the EDMs from a THDM object. Set run = false to turn of running couplings
    * for each diagram.
    */
    EDM(const THDM &thdm, bool run = true);
    ~EDM();

    // Returns true if the calculations of the EDMs were successful.
    bool is_initialized() const;

    /*
     * @brief: Prints to the console.
     *
     * print_edm prints the total eEDM together with the 10 largest
     * contributions.
     *
     * print_parameters prints all member variables at some moment (note however
     * that they change if _runningTHDM is set to true).
     */
    void print_edm() const;
    void print_parameters() const;

    // Returns a SLHA block containing the total eEDM.
    // See SLHA.h for how to use SLHA_BLOCK
    SLHA_BLOCK get_slha_block() const;

    /*
     * @brief: Returns eEDM
     *
     * If the EDM object is initialized, get_electron_edm returns the total
     * eEDM, while get_de returns a vector containing all the 35 contributions.
     * If the EDM object is not initialized, they return zeros.
     */
    double get_electron_edm() const;
    std::vector<double> get_de() const;

   private:
    /*
     * @brief: Initializes the class from a THDM object
     *
     * If _runningTHDM=true, it saves 6 different THDM objects that are evolved
     * to the mt, mw and Higgs masses respectively.
     *
     * If _runningTHDM=false, it sets its parameters from the given THDM.
     */
    bool compute_thdms(const THDM &thdm);

    /*
     * @brief: Sets all couplings
     *
     * All couplings that are saved as member variables are retrieved from the
     * given THDM.
     */
    void set_parameters(const THDM &thdm);

    /*
     * @brief: Computes eEDM Feynman diagrams
     *
     * Computes all Barr-Zee 2-loop contributions to the eEDM and saves them in
     * _BarrZee_VS_l (V for vector, S for scalar, l for loop). Calls
     * save_diagrams in the end, which saves the results in terms of eEDM in
     * units of [e cm] in _allDiagrams.
     *
     * compute_electron_edm simply add up all the components of _allDiagrams and
     * saves the total eEDM in _de.
     */
    void compute_wilson_coefficients();
    void save_diagrams();
    void compute_electron_edm();

   private:
    bool
        _initialized;  // set to true if the class is constructed without error.

    // Running THDMs. Defining 6 THDM objects at (mt, mw, mph1,mph2,mph3,mph4)
    bool _runningTHDM;
    std::vector<THDM> _thdms;

    //---------------------------------------------------------------------------
    // Parameters fixed by THDM object:
    double _e, _g;                       // Gauge couplings, set from THDM
    double _sW, _cW, _tW, _cotW, _secW;  // Calculated from _e, _g.
    double _v2;                          // VEV squared
    double
        _gZff[3];  // g(T_3^f - 2 Q_f s_w^2)/(2 c_w) index = up, down, electron

    // Fermion coupling parameters _c[f][i]:
    // c_{f,i}, \tilde{c}_{f,i} in eq.22 in phys.rev.D89 115023
    // f = fermion (u,c,t,d,s,b,e,muon,tau), i = neutral higgs
    double _c[9][3];
    double _cTilde[9][3];
    std::vector<double> _a;  // VVh_i neutral gauge boson coupling
    double _lambdaBar[3];    // H^+H^-h_i coupling
    Eigen::Matrix3cd _VCKM;
    std::complex<double> _rU33, _rD33, _rL11;
    std::vector<double> _mh, _mup, _mdn, _ml;  // Masses set from thdm

    //---------------------------------------------------------------------------
    // These are the separate diagram contributions to the _deltaf_tot. They are
    // 2-loop Barr-Zee diagrams; where the last word in the name denotes the
    // loop
    // particle connecting to the external photon.
    // Array index [f][h]: [fermion=u,d,e][higgs=h_1,h_2,h_3].
    double _barrZee_gh_t[3][3];
    double _barrZee_gh_b[3][3];
    double _barrZee_gh_tau[3][3];
    double _barrZee_Zh_t[3][3];
    double _barrZee_Zh_b[3][3];
    double _barrZee_Zh_tau[3][3];
    double _barrZee_gh_W[3][3];
    double _barrZee_Zh_W[3][3];
    double _barrZee_gh_Hc[3][3];
    double _barrZee_Zh_Hc[3][3];
    double _barrZee_Wh_h[3][3];  // loop of h_i and Hc.
    double _barrZee_Wh_tb[3];

    // All diagrams are collected in one vector that will be ordered according
    // to size of contribution. The string will denote the name of the diagram.
    // This is only for the electrons EDM.
    std::vector<std::tuple<double, std::string>> _allDiagrams;
    //---------------------------------------------------------------------------
    // absolute value of electrons EDM, which is decomposed into the separate
    // contributions:
    //   de = {gh_1, Zh_1, Wh_1, gh_2, Zh_2, Wh_2, gh_3, Zh_3, Wh_3, total}
    std::vector<double> _de;
};

//=============================================================================
// Functions to perform numerical integration

// If success is given, it sets it to false if any of the integrations fail.
double integrate_1D(const gsl_function *f, double a, double b,
                    bool *success = nullptr);

// The names are according to the equations in phys.rev.D89 115023.
double fz(double z);
double gz(double z);

double fTilde(double x, double y);
double gTilde(double x, double y);

double I4(double m1, double m2, double mHc, double mW, double mZ);
double I5(double m1, double m2, double mHc, double mW, double mZ);

// Functions needed for the integration
// They hold the parameters of the integrands
struct integration_z {
    double z;
};
struct integration_I {
    double m1, m2, mW, mHc;
};

// All integrands:
double b2_integrand(double x, void *p);
double b3_integrand(double x, void *p);
double i4_integrand(double x, void *p);
double i5_integrand(double x, void *p);

// Integrals for the W-Hc (fermion loop) contribution.
// They are integrated over x=[0,1].
// G1 = x^2(1+x)*G
// G2 = x(1-x^2)*G
// G3 = x^2(1-x)*G
// G4 = x*(1-x)^2G
// where G is eq.29 in arXiv:1502.04199
double G29(double x, double o1, double o2);
double Gi(int i, double o1, double o2);  // i=1,2,3,4 for G1, G2 etc.
struct integration_G {
    double o1, o2;
};
double G1_integrand(double x, void *p);
double G2_integrand(double x, void *p);
double G3_integrand(double x, void *p);
double G4_integrand(double x, void *p);

}  // namespace THDME

