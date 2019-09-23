// EDM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: EDM class
 * @author: Joel Oredsson
 *
 * See header file for further details.
 *
 *============================================================================*/
#include "EDM.h"
#include <algorithm>
#include "Globals.h"
#include "HelpFunctions.h"

using namespace std;

namespace THDME {

EDM::EDM(const THDM &thdm, bool run) : _initialized(false), _runningTHDM(run) {
    if (compute_thdms(thdm)) {
        compute_wilson_coefficients();
        compute_electron_edm();
        _initialized = true;
    } else {
        printf("[ERROR]: Couldn't initialize EDM!\n");
    }
}

EDM::~EDM() {}

bool EDM::is_initialized() const { return _initialized; }

void EDM::print_edm() const {
    if (_initialized) {
        // Sorting the diagrams according to largest contribution
        auto sortTuples = [](const std::tuple<double, std::string> &t1,
                             const std::tuple<double, std::string> &t2) {
            return (std::abs(std::get<0>(t1)) > std::abs(std::get<0>(t2)));
        };
        vector<tuple<double, string>> orderedDiagrams = _allDiagrams;
        sort(orderedDiagrams.begin(), orderedDiagrams.end(), sortTuples);

        Table tab(2);
        tab.set_frame_style("-", "|");
        tab.set_title("Electron EDM (10 largest diagrams)");
        tab.set_frame_color(BLUE);

        using l = vector<string>;
        tab.add_row(l{"Diagram", "|d_e| [e cm]"}, true);
        for (int i = 0; i < 10; ++i)
            tab.add_row(l{std::get<1>(orderedDiagrams[i]),
                          stringAuto(std::get<0>(orderedDiagrams[i]))});

        tab.add_row(l{"Total", stringAuto(_de[0])}, true);
        tab.print();
        cout << "\n";
    } else
        printf("EDM: Failed to be initialized:(\n");
}

SLHA_BLOCK EDM::get_slha_block() const {
    SLHA_BLOCK block("EDM", "Electric dipole moment of electron");
    if (_initialized) {
        block.add_parameter(1, _de[0], "abs(d_e) [e cm]");
    } else
        block.add_parameter(1, -1., "EDM not initialized");
    return block;
}

double EDM::get_electron_edm() const {
    if (!_initialized) {
        return 0.;
    }

    return _de[0];
}

std::vector<double> EDM::get_de() const {
    if (!_initialized) {
        std::vector<double> zeros;
        zeros.resize(35, 0.);
        return zeros;
    }
    return _de;
}

bool EDM::compute_thdms(const THDM &thdm) {
    // Fixes masses and Yukawa matrices
    _mh = thdm.get_higgs_treeLvl_masses();
    for (auto &mh : _mh)
        if (std::isnan(mh)) {
            printf("NaN mh when calculating EDM\n");
            return false;
        } else if (mh < 10.) {
            printf("Small mh = %16.8e GeV when calculating EDM\n", mh);
            return false;
        }
    // If running is enable, create 6 THDM objects defined at the energy scales
    // mt, mW, mh1, mh2, mh3, mHc. Returns false if this fails, i.e. cant
    // calculate the pole masses.
    if (_runningTHDM) {
        _thdms.clear();
        _thdms.emplace_back(thdm);
        _thdms.back().set_logLevel(LOG_ERRORS);
        RgeConfig options;
        options.dataOutput = false;
        options.consoleOutput = false;
        options.evolutionName = "";
        options.twoloop = true;
        options.perturbativity = true;
        options.stability = true;
        options.unitarity = true;
        options.finalEnergyScale = Global::mt;
        options.steps = 1;
        _thdms.back().set_rgeConfig(options);
        if (!_thdms.back().calc_higgs_pole_masses()) {
            std::cout << "[ERROR]: Couldn't calc higgs pole masses in EDM.\n";
            return false;
        }
        auto mh = _thdms.back().get_higgs_pole_masses();

        // Evolving to mt
        if (!_thdms.back().evolve()) {
            std::cout << "[ERROR]: Couldn't evolve thdm in EDM.\n";
            return false;
        }
        // Creating a new one and evolving it to mW
        _thdms.emplace_back(_thdms.back());
        if (!_thdms.back().evolve_to(Global::mW)) {
            std::cout << "[ERROR]: Couldn't evolve thdm in EDM.\n";
            return false;
        }
        // Creating four new ones and evolving to the pole Higgs masses
        for (int h = 1; h <= 4; ++h) {
            _thdms.emplace_back(_thdms.back());
            if (!_thdms.back().evolve_to(mh[h])) {
                std::cout << "[ERROR]: Couldn't evolve thdm in EDM.\n";
                return false;
            }
        }

        // Also setting the Higgs masses to the "pole" ones
        for (int h = 0; h < 4; ++h) {
            _mh[h] = mh[h + 1];
            if (_mh[h] < 1.) {
                std::cout << "[ERROR]: Too small Higgs masses in EDM.\n";
                return false;
            }
        }
    } else
        set_parameters(thdm);

    return true;
}

void EDM::set_parameters(const THDM &thdm) {
    _mup = thdm.get_mup();
    _mdn = thdm.get_mdn();
    _ml = thdm.get_ml();

    vector<double> mf;
    auto rF = thdm.get_yukawa_rho();

    // Fixing gauge couplings
    double tW = thdm.get_gauge_couplings()[0] / thdm.get_gauge_couplings()[1];
    double sW = sin(atan(tW));
    double cW = sqrt(1. - sW * sW);
    _e = sW * thdm.get_gauge_couplings()[1];
    _g = thdm.get_gauge_couplings()[1];
    _v2 = thdm.get_v2();
    double v = sqrt(_v2);

    _sW = _e / _g;
    _cW = std::sqrt(1. - sW * sW);
    _tW = sW / cW;
    _cotW = 1. / tW;
    _secW = 1. / cW;

    _gZff[0] = _g * (0.5 - 2. * Global::Qf[0] * sW * sW) / (2. * cW);   // up
    _gZff[1] = _g * (-0.5 - 2. * Global::Qf[1] * sW * sW) / (2. * cW);  // down
    _gZff[2] =
        _g * (-0.5 - 2. * Global::Qf[2] * sW * sW) / (2. * cW);  // electrons

    // Couplings to fermions for neutral Higgses
    const static complex<double> prefactor(0., 1.);
    complex<double> cS, cP;
    const static double ml[3] = {Global::me, Global::mmuon, Global::mtau};

    for (int h = 0; h < 3; ++h)
        for (int f = 0; f < 3; ++f) {
            // up-sector
            thdm.get_coupling_huu(h + 1, f + 1, f + 1, cS, cP);
            _c[f][h] = (v / _mup[f]) * real(prefactor * cS);
            _cTilde[f][h] = (v / _mup[f]) * real(prefactor * cP);
            // dn-sector
            thdm.get_coupling_hdd(h + 1, f + 1, f + 1, cS, cP);
            _c[3 + f][h] = (v / _mdn[f]) * real(prefactor * cS);
            _cTilde[3 + f][h] = (v / _mdn[f]) * real(prefactor * cP);
            // l-sector
            thdm.get_coupling_hll(h + 1, f + 1, f + 1, cS, cP);
            if (_ml[f] < 1e-15) _ml[f] = ml[f];
            _c[6 + f][h] = (v / _ml[f]) * real(prefactor * cS);
            _cTilde[6 + f][h] = (v / _ml[f]) * real(prefactor * cP);
        }

    _a = thdm.get_VV_couplings();

    auto qij = thdm.get_qij();  // (qi1, qi2) 8 components

    Base_invariant inv = thdm.get_param_invariant();
    for (int i = 0; i < 3; ++i)
        _lambdaBar[i] = real(qij[i]) * inv.Z3 + real(qij[4 + i] * inv.Z7inv);

    _VCKM = thdm.get_vCkm();
    _rU33 = thdm.get_rFij(UP, 2, 2);
    _rD33 = thdm.get_rFij(DOWN, 2, 2);
    _rL11 = thdm.get_rFij(LEPTON, 0, 0);
}

void EDM::compute_wilson_coefficients() {
    // Calculating diagrams for 3 different neutral Higgses.
    // It is written so that it can calculate the EDM for u and d as well, but
    // right now it only does it for the electron (i.e. f starts at 2).

    // If _running_thdm = true, we use the couplings from the THDM defined at
    // the largest mass scale in the diagram.
    for (int h = 0; h < 3; ++h) {
        for (int f = 2; f < 3; ++f) {
            if (_runningTHDM) {
                set_parameters(_thdms[h + 2]);
            }
            // 2-loop Barr-Zee gamma-h (b loop)
            double zbi = _mdn[2] * _mdn[2] / (_mh[h] * _mh[h]);
            _barrZee_gh_b[f][h] =
                (-Global::nC * Global::Qf[f] * Global::Qf[1] * Global::Qf[1] *
                 _e * _e / (64. * std::pow(M_PI, 4.))) *
                (fz(zbi) * _c[5][h] * _cTilde[3 * f][h] +
                 gz(zbi) * _cTilde[5][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee gamma-h (tau loop)
            double ztaui = _ml[2] * _ml[2] / (_mh[h] * _mh[h]);
            _barrZee_gh_tau[f][h] =
                (-Global::Qf[f] * Global::Qf[2] * Global::Qf[2] * _e * _e /
                 (64. * std::pow(M_PI, 4.))) *
                (fz(ztaui) * _c[8][h] * _cTilde[3 * f][h] +
                 gz(ztaui) * _cTilde[8][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee Z-h (b loop)
            double mbMZ = _mdn[2] * _mdn[2] / (Global::mZ * Global::mZ);
            _barrZee_Zh_b[f][h] =
                (-Global::nC * Global::Qf[f] * _gZff[f] * _gZff[1] /
                 (64. * std::pow(M_PI, 4.))) *
                (fTilde(zbi, mbMZ) * _c[5][h] * _cTilde[3 * f][h] +
                 gTilde(zbi, mbMZ) * _cTilde[5][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee Z-h (tau loop)
            double mtauMZ = _ml[2] * _ml[2] / (Global::mZ * Global::mZ);
            _barrZee_Zh_tau[f][h] =
                (-Global::Qf[f] * _gZff[f] * _gZff[2] /
                 (64. * std::pow(M_PI, 4.))) *
                (fTilde(ztaui, mtauMZ) * _c[8][h] * _cTilde[3 * f][h] +
                 gTilde(ztaui, mtauMZ) * _cTilde[8][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee gamma-h (W loop)
            if (_runningTHDM) {
                if (_mh[h] < Global::mW) set_parameters(_thdms[1]);
            }
            double zWi = Global::mW * Global::mW / (_mh[h] * _mh[h]);
            _barrZee_gh_W[f][h] =
                (Global::Qf[f] * _e * _e / (256. * std::pow(M_PI, 4.))) *
                (((6. + 1. / zWi) * fz(zWi) + (10. - 1. / zWi) * gz(zWi)) *
                 _a[h] * _cTilde[3 * f][h]);

            // 2-loop Barr-Zee Z-h (W loop)
            _barrZee_Zh_W[f][h] =
                (_gZff[f] * _e * _cotW / (256. * std::pow(M_PI, 4.))) *
                (((6. - _secW * _secW + (2. - _secW * _secW) / (2. * zWi)) *
                      fTilde(zWi, _cW * _cW) +
                  (10. - 3. * _secW * _secW -
                   (2. - _secW * _secW) / (2. * zWi)) *
                      gTilde(zWi, _cW * _cW)) *
                 _a[h] * _cTilde[3 * f][h]);

            // 2-loop Barr-Zee gamma-h (t loop)
            if (_runningTHDM) {
                if (_mh[h] > Global::mt)
                    set_parameters(_thdms[h + 2]);
                else
                    set_parameters(_thdms[0]);
            }
            double zti = _mup[2] * _mup[2] / (_mh[h] * _mh[h]);
            _barrZee_gh_t[f][h] =
                (-Global::nC * Global::Qf[f] * Global::Qf[0] * Global::Qf[0] *
                 _e * _e / (64. * std::pow(M_PI, 4.))) *
                (fz(zti) * _c[2][h] * _cTilde[3 * f][h] +
                 gz(zti) * _cTilde[2][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee Z-h (t loop)
            double mtMZ = _mup[2] * _mup[2] / (Global::mZ * Global::mZ);
            _barrZee_Zh_t[f][h] =
                (-Global::nC * Global::Qf[f] * _gZff[f] * _gZff[0] /
                 (64. * std::pow(M_PI, 4.))) *
                (fTilde(zti, mtMZ) * _c[2][h] * _cTilde[3 * f][h] +
                 gTilde(zti, mtMZ) * _cTilde[2][h] * _c[3 * f][h]);

            // 2-loop Barr-Zee gamma-h (Hc loop)
            if (_runningTHDM) {
                if (_mh[h] > _mh[3])
                    set_parameters(_thdms[h + 2]);
                else
                    set_parameters(_thdms[5]);
            }
            double zHi = _mh[3] * _mh[3] / (_mh[h] * _mh[h]);
            _barrZee_gh_Hc[f][h] =
                (Global::Qf[f] * _e * _e * _v2 /
                 (256. * std::pow(M_PI, 4.) * _mh[3] * _mh[3])) *
                (fz(zHi) - gz(zHi)) * _lambdaBar[h] * _cTilde[3 * f][h];

            // 2-loop Barr-Zee Z-h (Hc loop)
            double mHcMZ = _mh[3] * _mh[3] / (Global::mZ * Global::mZ);
            _barrZee_Zh_Hc[f][h] =
                (_gZff[f] * _e * _cotW * 0.5 * (1. - _tW * _tW) * _v2 /
                 (256. * std::pow(M_PI, 4.) * _mh[3] * _mh[3])) *
                (fTilde(zHi, mHcMZ) - gTilde(zHi, mHcMZ)) * _lambdaBar[h] *
                _cTilde[3 * f][h];

            // 2-loop W-h (h,Hc loop)
            double sf = (f == 0) ? -1. : 1.;  // This is -1 for up quarks and 1
                                              // for dn quarks and leptons.
            _barrZee_Wh_h[f][h] =
                (sf / (512 * std::pow(M_PI, 4.))) *
                (_e * _e * _a[h] * _cTilde[3 * f][h] *
                     I4(_mh[h], _mh[3], _mh[3], Global::mW, Global::mZ) /
                     (2. * _sW * _sW) -
                 I5(_mh[h], _mh[3], _mh[3], Global::mW, Global::mZ) *
                     _lambdaBar[h] * _cTilde[3 * f][h]);
        }
    }

    // 2-loop W-h (t,b fermion loop)
    for (int f = 2; f < 3; ++f) {
        if (_runningTHDM) {
            if (_mh[3] > Global::mt)
                set_parameters(_thdms[5]);
            else
                set_parameters(_thdms[0]);
        }
        double mt2 = Global::mt * Global::mt;
        double mb2 = Global::mb * Global::mb;
        double mw2 = Global::mW * Global::mW;
        double mHc2 = _mh[3] * _mh[3];
        double o1_mH = mt2 / mHc2;
        double o2_mH = mb2 / mHc2;
        double o1_mW = mt2 / mw2;
        double o2_mW = mb2 / mw2;

        _barrZee_Wh_tb[f] =
            ((Global::nC * (_e * _e / (4. * M_PI)) * std::norm(_VCKM(2, 2))) /
             (128. * pow(M_PI, 3) * _sW * _sW * (mHc2 - mw2))) *
            (imag(conj(_rL11) * conj(_rU33)) * Global::mt * Global::Qf[0] *
                 (Gi(1, o1_mH, o2_mH) - Gi(1, o1_mW, o2_mW)) +
             imag(conj(_rL11) * conj(_rU33)) * Global::mt * Global::Qf[1] *
                 (Gi(2, o1_mH, o2_mH) - Gi(2, o1_mW, o2_mW)) +
             imag(conj(_rL11) * _rD33) * Global::mb * Global::Qf[0] *
                 (Gi(3, o1_mH, o2_mH) - Gi(3, o1_mW, o2_mW)) +
             imag(conj(_rL11) * _rD33) * Global::mb * Global::Qf[1] *
                 (Gi(4, o1_mH, o2_mH) - Gi(4, o1_mW, o2_mW)));
    }

    save_diagrams();
}

void EDM::save_diagrams() {
    static const double GeVinvToCm = 1.98e-14;
    static const double dim =
        GeVinvToCm * 2. * Global::me / (Global::v * Global::v);
    for (int h = 0; h < 3; ++h) {
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_gh_t[2][h],
                                               "gh" + to_string(h + 1) + "_t"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_gh_b[2][h],
                                               "gh" + to_string(h + 1) + "_b"));
        _allDiagrams.push_back(std::make_tuple(
            dim * _barrZee_gh_tau[2][h], "gh" + to_string(h + 1) + "_tau"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_Zh_t[2][h],
                                               "Zh" + to_string(h + 1) + "_t"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_Zh_b[2][h],
                                               "Zh" + to_string(h + 1) + "_b"));
        _allDiagrams.push_back(std::make_tuple(
            dim * _barrZee_Zh_tau[2][h], "Zh" + to_string(h + 1) + "_tau"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_gh_W[2][h],
                                               "gh" + to_string(h + 1) + "_W"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_Zh_W[2][h],
                                               "Zh" + to_string(h + 1) + "_W"));
        _allDiagrams.push_back(std::make_tuple(
            dim * _barrZee_gh_Hc[2][h], "gh" + to_string(h + 1) + "_Hc"));
        _allDiagrams.push_back(std::make_tuple(
            dim * _barrZee_Zh_Hc[2][h], "Zh" + to_string(h + 1) + "_Hc"));
        _allDiagrams.push_back(std::make_tuple(dim * _barrZee_Wh_h[2][h],
                                               "Wh_h" + to_string(h + 1) + ""));
    }
    _allDiagrams.push_back(
        std::make_tuple(GeVinvToCm * _barrZee_Wh_tb[2], "Wh_tb"));
}
void EDM::compute_electron_edm() {
    _de.clear();
    double electron_edm = 0.;
    for (auto &diagram : _allDiagrams) electron_edm += std::get<0>(diagram);

    _de.push_back(std::abs(electron_edm));
    for (auto &diagram : _allDiagrams) _de.push_back(std::get<0>(diagram));
}

void EDM::print_parameters() const {
    printf(
        "\n===============================================================\n");
    if (!_initialized) {
        printf("EDM failed to be initialized:(\n");
        printf(
            "\n==============================================================="
            "\n");
        return;
    }
    printf("Parameters of EDM:\n");
    for (int f = 0; f < 9; f += 1)
        for (int i = 0; i < 3; ++i) {
            printf("c[%i][%i] = %16.8e\n", f, i, _c[f][i]);
            printf("cTilde[%i][%i] = %16.8e\n", f, i, _cTilde[f][i]);
        }

    for (int i = 0; i < 3; ++i) printf("a[%i] = %16.8e\n", i, _a[i]);

    for (int i = 0; i < 3; ++i)
        printf("lambdaBar[%i] = %16.8e\n", i, _lambdaBar[i]);

    for (int i = 0; i < 3; ++i) printf("_gZff[%i] = %16.8e\n", i, _gZff[i]);

    printf("e = %16.8e\n", _e);
    printf("g = %16.8e\n", _g);
    printf("sW^2 = %16.8e\n", _e * _e / (_g * _g));
    printf("v2 = %16.8e\n", _v2);
    printf("nC = %i\n", Global::nC);
    for (int i = 0; i < 3; ++i) printf("Qf[%i] = %16.8e\n", i, Global::Qf[i]);
    printf("mZ = %16.8e\n", Global::mZ);
    printf("mW = %16.8e\n", Global::mW);

    cout << "&_mup[0] = " << &_mup[0] << endl;

    for (int i = 0; i < 4; ++i) printf("mh_%i =%16.8e GeV, ", i + 1, _mh[i]);
    printf("\n");
    for (int i = 0; i < 3; ++i) printf("mup_%i=%16.8e GeV, ", i + 1, _mup[i]);
    printf("\n");
    for (int i = 0; i < 3; ++i) printf("mdn_%i=%16.8e GeV, ", i + 1, _mdn[i]);
    printf("\n");
    for (int i = 0; i < 3; ++i) printf("ml_%i =%16.8e GeV, ", i + 1, _ml[i]);
    printf("\n");

    printf(
        "\n===============================================================\n");
}

//-----------------------------------------------------------------------------

double integrate_1D(const gsl_function *func, double a, double b,
                    bool *success) {
    constexpr static double EPSABS = 1e-12;
    constexpr static double EPSREL = 1e-12;

    double result = 0;
    double abserr = 0;
    size_t neval = 0;

    gsl_set_error_handler_off();

    static size_t limit = 200;  // Nr of subintervals in integration
    gsl_integration_workspace *workspace =
        gsl_integration_workspace_alloc(limit);

    int gslStatus = gsl_integration_qag(func, a, b, EPSABS, EPSREL, limit, 6,
                                        workspace, &result, &abserr);
    gsl_integration_workspace_free(workspace);

    if (gslStatus) {
        printf("[ERROR]: GSL failed in integrating function, gsl_errno=%d\n",
               gslStatus);
        printf("         %s\n", gsl_strerror(gslStatus));
        // DEBUG:
        printf("result: %16.8e\n", result);
        printf("abserr: %16.8e\n", abserr);
        printf("neval: %lu\n", neval);
        printf("Integrand value at %f = %16.8e\n", (b - a) / 2.,
               func->function((b - a) / 2., func->params));

        if (success != nullptr) *success = false;
        if (!std::isnan(result))
            return result;
        else
            return 0.;
    }

    return result;
}

double fz(double z) {
    gsl_function F;
    integration_z params = {z};

    F.params = &params;
    F.function = &b2_integrand;

    bool success = true;
    double result = integrate_1D(&F, 0, 1, &success);

    if (!success) {
        printf("Failed in integrating fz(%f)\n", z);
    }
    return result;
}

double gz(double z) {
    gsl_function F;
    integration_z params = {z};

    F.params = &params;
    F.function = &b3_integrand;

    bool success = true;
    double result = integrate_1D(&F, 0, 1, &success);
    if (!success) {
        printf("Failed in integrating gz(%f)\n", z);
    }
    return result;
}

double fTilde(double x, double y) {
    return (y * fz(x) / (y - x)) + (x * fz(y) / (x - y));
}

double gTilde(double x, double y) {
    return (y * gz(x) / (y - x)) + (x * gz(y) / (x - y));
}

double I4(double m1, double m2, double mHc, double mW, double mZ) {
    gsl_function F;
    integration_I params_1 = {mW, m1, mW, mHc};

    F.params = &params_1;
    F.function = &i4_integrand;

    bool success = true;

    double term1 = integrate_1D(&F, 0, 1, &success);

    integration_I params_2 = {m2, m1, mW, mHc};
    F.params = &params_2;

    double term2 = integrate_1D(&F, 0, 1, &success);

    if (!success) {
        printf("Failed in integrating I4 with parameters:\n");
        printf("m1=%.2f, m2=%.2f, mHc=%.2f, mW=%.2f, mZ=%.2f\n", m1, m2, mHc,
               mW, mZ);
    }

    return mW * mW * (term1 - term2) / (mHc * mHc - mW * mW);
}

double I5(double m1, double m2, double mHc, double mW, double mZ) {
    gsl_function F;
    integration_I params_1 = {mW, m1, mW, mHc};

    F.params = &params_1;
    F.function = &i5_integrand;

    bool success = true;
    double term1 = integrate_1D(&F, 0, 1, &success);

    integration_I params_2 = {m2, m1, mW, mHc};
    F.params = &params_2;

    double term2 = integrate_1D(&F, 0, 1, &success);

    if (!success) {
        printf("Failed in integrating I5 with parameters:\n");
        printf("m1=%.2f, m2=%.2f, mHc=%.2f, mW=%.2f, mZ=%.2f\n", m1, m2, mHc,
               mW, mZ);
    }

    return mW * mW * (term1 - term2) / (mHc * mHc - mW * mW);
}

double b2_integrand(double x, void *p) {
    integration_z *params = (integration_z *)p;
    double z = params->z;

    return 0.5 * z * (1. - 2. * x * (1. - x)) * log(x * (1. - x) / z) /
           (x * (1. - x) - z);
}

double b3_integrand(double x, void *p) {
    integration_z *params = (integration_z *)p;
    double z = params->z;

    return 0.5 * z * log(x * (1. - x) / z) / (x * (1. - x) - z);
}

double i4_integrand(double x, void *p) {
    integration_I *params = (integration_I *)p;
    double m1 = params->m1;
    double m2 = params->m2;
    double mW = params->mW;
    double mHc = params->mHc;

    double factor1 = (1. - x) * (1. - x);
    double factor2 = (x - 4. + x * (mHc * mHc - m2 * m2) / (mW * mW));
    double factor3 =
        m1 * m1 / (mW * mW * (1. - x) + m2 * m2 * x - m1 * m1 * x * (1. - x));
    double factor4 =
        log((mW * mW * (1. - x) + m2 * m2 * x) / (m1 * m1 * x * (1. - x)));

    return factor1 * factor2 * factor3 * factor4;
}

double i5_integrand(double x, void *p) {
    integration_I *params = (integration_I *)p;
    double m1 = params->m1;
    double m2 = params->m2;
    double mW = params->mW;

    double factor1 =
        m1 * m1 * x * (1. - x) * (1. - x) /
        (mW * mW * (1. - x) + m2 * m2 * x - m1 * m1 * x * (1. - x));
    double factor2 =
        log((mW * mW * (1. - x) + m2 * m2 * x) / (m1 * m1 * x * (1. - x)));

    return factor1 * factor2;
}

double G29(double x, double o1, double o2) {
    return log((o1 * x + o2 * (1. - x)) / (x * (1 - x))) /
           (x * (1. - x) - o1 * x - o2 * (1. - x));
}

double Gi(int i, double o1, double o2) {
    if (i < 1 || i > 4) {
        printf("Wrong argument in Gi!\n");
        printf("i=%i, o1=%.2f, o2=%.2f", i, o1, o2);
        return 0.;
    }
    gsl_function F;
    integration_G params_1 = {o1, o2};

    F.params = &params_1;
    switch (i) {
        case 1: {
            F.function = &G1_integrand;
            break;
        }
        case 2: {
            F.function = &G2_integrand;
            break;
        }
        case 3: {
            F.function = &G3_integrand;
            break;
        }
        case 4: {
            F.function = &G4_integrand;
            break;
        }
    }

    bool success = true;
    double result = integrate_1D(&F, 0, 1, &success);

    if (!success) {
        printf("Failed in integrating Gi with parameters:\n");
        printf("i=%i, o1=%.4f, o2=%.4f", i, o1, o2);
    }

    return result;
}

double G1_integrand(double x, void *p) {
    integration_G *params = (integration_G *)p;
    double o1 = params->o1;
    double o2 = params->o2;

    return x * x * (1. + x) * G29(x, o1, o2);
}

double G2_integrand(double x, void *p) {
    integration_G *params = (integration_G *)p;
    double o1 = params->o1;
    double o2 = params->o2;

    return x * (1. - x * x) * G29(x, o1, o2);
}

double G3_integrand(double x, void *p) {
    integration_G *params = (integration_G *)p;
    double o1 = params->o1;
    double o2 = params->o2;

    return x * x * (1. - x) * G29(x, o1, o2);
}

double G4_integrand(double x, void *p) {
    integration_G *params = (integration_G *)p;
    double o1 = params->o1;
    double o2 = params->o2;

    return x * (1. - x) * (1. - x) * G29(x, o1, o2);
}
}  // namespace THDME
