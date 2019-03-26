// THDM.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: General (complex) 2HDM class
 * @author: Joel Oredsson
 *
 * Derived class:
 *  BaseModel <- RgeModel <- THDM
 *
 * Example usage: See DemoRGE.cpp
 *
 *============================================================================*/
#pragma once

#include "LoggingSystem.h"
#include "RgeModel.h"
#include "SM.h"
#include "Structures.h"
#include "THDM_bases.h"

#ifdef SPHENO
#include "SPheno.h"
#endif

#include <Eigen/Dense>
#include <complex>
#include <tuple>
#include <vector>

namespace THDME {

/**
 * @brief: General complex two-Higgs doublet model
 */
class THDM : public RgeModel {
public:
  /**
   * @brief: SM constructor
   *
   * Initialize a THDM. Sets the VEV and gauge couplings to be SM like at
   * the scale MZ.
   */
  THDM();
  THDM(const SM &sm);

  /**
   * @brief: Copy and assignment constructor
   *
   * Calls set_from_thdm() that copies a thdm.
   */
  THDM(const THDM &thdm);
  THDM &operator=(const THDM &thdm);
  void set_from_thdm(const THDM &thdm);

  ~THDM();

  //---------------------------------------------------------------------------
  // Setting the Standard Model parameters
  //---------------------------------------------------------------------------

  /**
   * @brief: Sets SM parameters.
   *
   * Matches 2HDM to the given SM. Sets the squared VEV and gauge couplings.
   * From the CKM matrix and the fermion masses, it calculates the Yukawa
   * matrices.
   * 
   * @params:
   *   mu: Renormalization scale at which the THDM is defined.
   *   v2: Squared VEV.
   *   g_i: gauge couplings g1, g2, g3.
   *   mup, mdn, ml: Fermion masses.
   *   VCKM: CKM matrix.
   */
  void set_sm(const SM &sm);
  void set_sm(const double &mu, const double &v2, const std::vector<double> &g_i,
              const std::vector<double> &mup, const std::vector<double> &mdn,
              const std::vector<double> &ml, const Eigen::Matrix3cd &VCKM);

  /**
   * @brief: Simple functions to set/get specific values manually:
   */
  void set_v2(const double &v2);
  void set_gauge_couplings(const std::vector<double> &coupling);
  void set_fermion_masses(const std::vector<double> &mup,
                          const std::vector<double> &mdn,
                          const std::vector<double> &ml);
  void set_vCkm(const Eigen::Matrix3cd &VCKM);

  //---------------------------------------------------------------------------
  // Setting the scalar potential
  //---------------------------------------------------------------------------

  /**
   * @brief: Initializes the 2HDM from a Base_generic.
   *
   * @returns true if the parameters are set correctly.
   */
  bool set_param_gen(const Base_generic &in, const bool enforceTadpole = true);
  Base_generic get_param_gen() const;

  /**
   * @brief: Initializes the 2HDM from a Base_higgs.
   *
   * @returns true if the parameters are set correctly.
   */
  bool set_param_higgs(const Base_higgs &in, const bool enforceTadpole = true);
  Base_higgs get_param_higgs() const;

  /**
   * @brief: Initializes a CP conserving 2HDM from some
   *        real parameters.
   *
   * This function is taken from the 2HDMC class.
   *
   * @returns true if the parameters are set correctly.
   */
  bool set_param_hybrid(const Base_hybrid &hyb,
                        const bool enforceTadpole = true);
  Base_hybrid get_param_hybrid() const;

  /**
   * @brief: Initializes the 2HDM from a Base_invariant.
   *
   * @returns true if the parameters are set correctly.
   */
  bool set_param_invariant(const Base_invariant &inv,
                           const bool enforceTadpole = true);
  Base_invariant get_param_invariant() const;

  /**
   * @brief: Enforces a Z_2 symmetry
   *
   * Enforces an exact Z_2 symmetry and sets the symmetry breaking parameters to
   * zero.
   *
   * soft = true means that M12 is not forced to zero.
   *
   * @returns calc_higgs_from_compact()
   */
  bool enforce_z2Symmetric_potential(const bool soft = false);

  /**
   * @brief: Sets imaginary parts to zero
   *
   * Gets rid of numerical junk in the imaginary parts of generic basis.
   * Recalculates all parameters and @returns true if the Higgs masses are
   * valid.
   */
  bool delete_imag_parts();

  //---------------------------------------------------------------------------
  // Setting the Yukawa sector
  //---------------------------------------------------------------------------

  /**
   * @brief: Fixes the Yukawa sector based on a Z2 symmetry.
   *
   * Sets/gets the Yukawa matrices according to the given Z2symmetry that is
   * either NO_SYMMETRY, TYPE_I, TYPE_II, TYPE_III, TYPE_IV.
   *
   * If one chooses NO_SYMMETRY, the Yukawa sector is unchanged.
   * Use set_yukawa_rho to set the rho matrices.
   */
  void set_yukawa_type(const Z2symmetry &z2);
  Z2symmetry get_yukawa_type() const;

  /**
   * @brief: Sets flavor aligned Yukawa sector
   *
   */
  void set_yukawa_aligned(const std::complex<double> &aU,
                          const std::complex<double> &aD,
                          const std::complex<double> &aL);

  std::vector<std::complex<double>> get_aF() const;
  /**
   * @brief: Sets the rho/eta Yukawa matrices.
   *
   * There is no Z_2 symmetry in the Yukawa sector, one needs to set the rho
   * Yukawa matrices manually. If a Z_2 symmetry is already set, this function
   * overwrites it.
   */
  void set_yukawa_manual(const Eigen::Matrix3cd &rU, const Eigen::Matrix3cd &rD,
                         const Eigen::Matrix3cd &rL);

  void
  set_yukawa_eta(const Eigen::Matrix3cd &eta1U, const Eigen::Matrix3cd &eta1D,
                 const Eigen::Matrix3cd &eta1L, const Eigen::Matrix3cd &eta2U,
                 const Eigen::Matrix3cd &eta2D, const Eigen::Matrix3cd &eta2L);

  /**
   * @brief: Sets tanb.
   *
   * Sets tanb = v2 / v1, which relates the generic potential to the Higgs
   * potential through a SU(2) Higgs flavor transformation.
   */
  void set_tanb(const double &tanb);

  /**
   * @brief: U(1) transformation of H_2
   *
   * The Higgs basis is unique up to a phase of the second Higgs doublet,
   * H_2 -> exp(i chi) H_2.
   * The Higgs basis parameters transforms as
   * Y3, Z6, Z7 -> exp( -i chi) Y3, Z6, Z7,
   * Z5 -> exp( -2 i chi) Z5.
   */
  void chi_phase_shift(const double &chi);

  //---Override-functions-------------------------------------------------------

  /**
   * @brief: Functions used by RGE_system when performing RG evolution.
   *
   * rge_update: Updates the parameters of the model.
   *             @returns true if the update didn't result in any NaN values.
   * rge_finished: Closes the open filestreams of the model.
   */
  bool rge_update(const double y[], const double ti) override;
  void rge_finished() override;

  /**
   * @brief: Checks perturbativity
   *
   *   This method checks whether the couplings of the Higgs fullfills
   * perturbativity.
   *
   * @returns false if the perturbativity constraint is violated;
   * true otherwise.
   */
  bool is_perturbative() const override;

  /**
   * @brief: Checks perturbativity
   *
   * This method checks whether the couplings of the Higgs fullfills unitarity
   *
   * @returns false if the unitarity constraint is violated; true otherwise.
   */
  bool is_unitary() const override;

  /**
   * @brief: Checks stability
   *
   *   This method checks whether the Higgs potential is stable
   * according to the algorithm layed out in
   * Phys.Rev.D75 035001 (Ivanov, 2005)
   *
   * @returns Boolean false if the stability is violated, true otherwise
   */
  bool is_stable() const override;

  /**
   * @brief: Sets parameters of the THDM to and from an array.
   *
   *   All parameters that are evolved with RGEs are stored in an array y[].
   * This array is fed into the gsl_solver, along with the RGEs for each
   * of its components.
   *
   *   set_y fills the argument array with the parameters.
   *
   *   set_param_from_y sets the member variables from an array y.
   */
  void set_y(double y[]) const override;
  void set_model_from_y(const double y[]) override;

  /**
   * @brief: Resets the model
   *
   * Sets parameters to zero, sm parameters to the sm at EW scale.
   * If one supplies a pointer to a double array y[], it resets all the
   * parameters to those as well.
   *
   * @returns false if it can't calculate Higgs masses when given a y[].
   * */
  bool reset(const double y[], const double &renormScale) override;
  bool reset_to_saved_state() override;

  /**
   * @brief: Prints the gauge CKM matrix, gauge couplings, potential
   * parameters in Higgs basis, Higgs masses and Yukawa couplings.
   */
  void print_all() const override;

  /*------------------------------------------------------------------------*/

  /**
   * @brief: Checks if the 2HDM potential is CP violating.
   *
   *   Checks the base invariant CP quantities in hep-ph/0506227 that should
   * be zero for the model to be CP conserving.
   *
   * @returns true if CP is conserved; false otherwise
   */
  bool is_cp_conserved() const;

  /**
   * @brief: Checks tree-lvl Z_2 symmetric stability condition
   *
   * @returns true if stable; false otherwise
   */
  bool is_stable_z2sym() const;

  /**
   * @brief: Checks if the model is "good"
   *
   * Checks perturbativity, unitary, stability and real positive Higgs masses.
   *
   * @returns true it satisfies the above criteria and false otherwise.
   */
  bool is_pert_unit_stab() const;

  /**
   * @brief Higgs coupling q_{ki} factors
   *
   * This method returns the invariants q_{ki} hep-ph/0602242
   * which are used for the triple and quartic Higgs couplings.
   *
   * @params:
   *   k Higgs index (1--4)
   *   i Coupling index (1--2)
   *
   * @returns The value of the coefficient q_{ki}
   */
  std::complex<double> get_qki(int k, int i) const;
  // Retrievies base invariant angles that can be found for example in
  // arXiv:1011.6188.
  // qi12 and qi22 refers to q_{i1}^2 and |q_{i2}|^2 respectively.
  std::vector<std::complex<double>> get_qij() const; // (qi1, qi2) 8 components
  std::vector<double> get_qi12() const;
  std::vector<double> get_qi22() const;

  // Retrieve couplings to fermions:
  // The 3x3 matrix is the coupling of (h_i ff) and is diagonal in cases of no
  // FCNCs.
  Eigen::Matrix3cd get_huu(int i) const;
  Eigen::Matrix3cd get_hdd(int i) const;
  Eigen::Matrix3cd get_hll(int i) const;
  // Coupling matrices of (h_i f igamma_5 f), i.e. the axial current.
  Eigen::Matrix3cd get_huu_axial(int i) const;
  Eigen::Matrix3cd get_hdd_axial(int i) const;
  Eigen::Matrix3cd get_hll_axial(int i) const;

  /**
   * @brief: Retrieves all the fermion couplings
   *
   * Components:
   * h_1uu, h_1uu_axial, h_1dd, h_1 dd_axial, h_1 ll, h_1 ll_axial
   * + h_2 and h_3. Total of 18 matrices.
   *
   * The normalized ones are multiplied by a v/sqrt(mf_i mf_j) factor!
   */
  std::vector<Eigen::Matrix3cd> get_fermion_couplings() const;
  std::vector<Eigen::Matrix3cd> get_fermion_couplings_normalized() const;

  /**
   * @brief: Retrieves the coupling of h_i to the WW and ZZ bosons
   *
   *  Parameterized as in eq.2.7 of arXiv:1503.01114v2, i.e. the a_i
   *   Lagrangian = a_i h_i ( 2 (mW^2) WW + mZ^2 ZZ)/v.
   *
   *  @returns a_i.
   */
  std::vector<double> get_VV_couplings() const;     // a_i
  std::complex<double> get_hvv(int h, int v) const; // the full coupling
  void get_coupling_vvh(int v1, int v2, int h, std::complex<double> &c) const;

  double get_v2() const;
  std::vector<double> get_gauge_couplings() const;
  std::vector<double> get_mup() const;
  std::vector<double> get_mdn() const;
  std::vector<double> get_ml() const;
  Eigen::Matrix3cd get_vCkm() const;
  std::vector<Eigen::Matrix3cd> get_yukawa_rho() const;
  std::vector<Eigen::Matrix3cd> get_yukawa_eta() const;
  std::vector<Eigen::Matrix3cd> get_yukawa_kF_rF() const;
  std::vector<std::complex<double>> get_vevs() const;
  std::vector<double> get_higgs_treeLvl_masses() const;
  std::complex<double> get_z2_breaking_quantity() const;
  std::vector<double> get_oblique() const; // returns (S,T,U)

  std::tuple<std::string, int, std::complex<double>>
  get_largest_diagonal_rF() const;
  std::tuple<std::string, int, int, std::complex<double>>
  get_largest_nonDiagonal_rF() const;
  std::tuple<std::string, int, double> get_largest_lambda() const;

  // Returns largest non-diagonal Yukawa element in Cheng-Sher parametrization
  std::tuple<std::string, int, int, std::complex<double>>
  get_largest_nonDiagonal_lamF() const;

  // Returns a specific lambda_ij Yukawa element
  double get_lamF_element(const FermionSector flavor, const int i,
                          const int j) const;
  // Or a complete matrix
  Eigen::Matrix3cd get_lamF(const FermionSector flavor) const;

  /**
   * @brief: Prints to console
   */
  void print_higgs_masses() const;
  void print_fermion_masses() const;
  void print_potential() const;
  void print_yukawa() const;
  void print_CKM() const;
  void print_param_gen() const;
  void print_param_compact() const;
  void print_param_higgs() const;
  void print_features() const;
  void print_oblique() const;

//----------------------------------------------------------------------------
/**
 * @brief: SPheno
 *
 * loopLvl sets what level of quantum corrected mass is being fitted.
 *   0 = tree-lvl
 *   1 = 1-loop calculated with SPheno
 *   2 = Include some 2-loop contributions for neutral Higgs, also done by
 *   SPheno.
 */
#ifdef SPHENO
  bool run_spheno(const int massLoopLvl); // @returns false if it failed
  std::vector<double> get_spheno_output() const;
  void print_spheno_results() const;
#endif

  //----------------------------------------------------------------------------

  /**
   * @brief: Calculates results from RG evolution
   *
   * Creates a copy of thdm that it evolves and updates _rgeResults without
   * updating the parameters of thdm.
   */
  void calc_rgeResults();

  /**
   * @brief: Writes a SLHA file
   *
   * Writes all the information of the THDM to a SLHA file.
   *
   * The SLHA file can be used as input to SPheno.
   *
   * sphenoLoopLvl determines the loop level of pole mass calculations in
   * SPheno. If sphenoLoopLvl = -1, no SPheno blocks are written.
   */
  void
  write_slha_file(const int sphenoLoopLvl,
                  const std::string &fileName = "LesHouches.in.THDM_GEN") const;
  bool set_from_slha_file(const std::string &fileName);

  /**
   * @brief: Prints Higgs boson mass eigenstates
   *
   *   The neutral Higgs bosons mass eigenstates h1, h2, h3, are ordered in
   * masses. They are a linear combination, set by the mixing angles theta_12,
   * theta_13, theta_23, of the Higgs doublets scalar components phi_1^0,
   * phi_2^0 and pseudo-scalar a^0.
   */
  void print_neutral_higgs_eigenVectors() const;

  /**
   * @brief: Calculates all Higgs tree-lvl masses and mixing angles
   *
   * Calculates (m_h1, m_h2, m_h3, m_Hc, theta12, theta13, theta23)
   *
   * @returns true if all Higgs masses are positive.
   */
  bool calc_treeLvl_masses_and_mixings();

  // Similar functions to 2HDMC
  double get_hmass(int h) const;

  /**
   * @brief: Couplings of vector bosons to pairs of Higgses
   *
   * Calculates the coupling V h_1 h_2 between two physical Higgs states,
   * specified by h1 and h2, and one vector boson V. Conventions are
   * according to hep-ph/0602242.
   *
   * @params:
   *   h1 Index of first Higgs boson (1,2,3,4 = h_1,h_2,h_3,H+)
   *   h2 Index of second Higgs boson (1,2,3,4 = h_1,h_2,h_3,H+)
   *   v  Index of vector boson (1,2,3 = gamma, Z, W^+)
   *   c  Returned (std::complex) value for coupling
   */
  std::complex<double> get_coupling_vhh(int v, int h1, int h2) const;
  void get_coupling_vhh(int v, int h1, int h2, std::complex<double> &c) const;
  std::complex<double> get_coupling_hhh(int h1, int h2, int h3) const;
  void get_coupling_hhh(int h1, int h2, int h3, std::complex<double> &c) const;

  /**
   * @brief: Couplings of Higgses to mixed type fermions
   *
   * Calculates the coupling hdu between one physical Higgs state,
   * specified by h (only relevant one is charged Higgs), and two quarks
   * f1 and f2.
   * 
   * Includes QCD running of Yukawa couplings to the Higgs mass scale.
   *
   * @params:
   *   h  Index of Higgs boson (1,2,3,4 = h_1, h_2, h_3, Hc)
   *   d Down-type fermion (1,2,3 = d,s,b)
   *   u Up-type fermion (1,2,3 = u,c,t)
   *   cs Returned (std::complex) value for scalar coupling
   *   cp Returned (std::complex) value for pseudoscalar coupling
   */
  void get_coupling_hdu(int h, int d, int u, std::complex<double> &cs,
                        std::complex<double> &cp) const;

  /**
   * @brief Couplings of Higgses to down-type fermions
   *
   * Calculates the coupling hdd between one physical Higgs state,
   * specified by h, and two down-type quarks f1 and f2.
   *
   * Includes QCD running of Yukawa couplings to the Higgs mass scale.
   * 
   * @params:
   *   h  Index of Higgs boson (1,2,3,4 = h_1, h_2, h_3, Hc)
   *   f1 First fermion (1,2,3 = d,s,b)
   *   f2 Second fermion (1,2,3 = d,s,b)
   *   cs Returned (std::complex) value for scalar coupling
   *   cp Returned (std::complex) value for pseudoscalar coupling
   */
  void get_coupling_hdd(int h, int f1, int f2, std::complex<double> &cs,
                        std::complex<double> &cp) const;

  /**
   * @brief Couplings of Higgses to up-type fermions
   *
   * Calculates the coupling  huu  between one physical Higgs state,
   * specified by h, and two up-type quarks f1 and f2.
   *
   * Includes QCD running of Yukawa couplings to the Higgs mass scale.
   * 
   * @params:
   *   h  Index of Higgs boson (1,2,3,4 = h_1,h_2,h_3,Hc)
   *   f1 First fermion (1,2,3 = u,c,t)
   *   f2 Second fermion (1,2,3 = u,c,t)
   *   cs Returned (std::complex) value for scalar coupling
   *   cp Returned (std::complex) value for pseudoscalar coupling
   */
  void get_coupling_huu(int h, int f1, int f2, std::complex<double> &cs,
                        std::complex<double> &cp) const;
  /**
   * @brief Couplings of Higgses to charged leptons
   *
   * Calculates the coupling  hll  between one physical Higgs state,
   * specified by h, and two leptons f1 and f2.
   *
   * @params:
   *   h  Index of Higgs boson (1,2,3,4 = h_1,h_2,h_3,H+)
   *   f1 First fermion (1,2,3 =  e,mu,tau )
   *   f2 Second fermion (1,2,3 =  e,mu,tau )
   *   cs Returned (std::complex) value for scalar coupling
   *   cp Returned (std::complex) value for pseudoscalar coupling
   */
  void get_coupling_hll(int h, int f1, int f2, std::complex<double> &cs,
                        std::complex<double> &cp) const;
  /**
   * @brief: Couplings of Higgses to mixed leptons
   *
   * Calculates the coupling hlnu_l between one physical Higgs state,
   * specified by h (only relevant one is charged Higgs), and two leptons
   * f1 and f2.
   *
   * @params:
   *   h  Index of Higgs boson (1,2,3,4 = h_1,h_2,h_3,Hc)
   *   l  Charged lepton (1,2,3 =  e,mu,tau)
   *   n  Neutrino (1,2,3 = nu_e,nu_mu,nu_tau)
   *   cs Returned (std::complex) value for scalar coupling
   *   cp Returned (std::complex) value for pseudoscalar coupling
   */
  void get_coupling_hln(int h, int l, int n, std::complex<double> &cs,
                        std::complex<double> &cp) const;

private:
  /* Sets the kappa matrices to be diagonal proportional to the fermion
       masses.*/
  void set_kF_from_fermion_masses();

  // Sets _rU, _rD and _rL to arguments.
  inline void set_yukawa_rho(const Eigen::Matrix3cd &rU,
                             const Eigen::Matrix3cd &rD,
                             const Eigen::Matrix3cd &rL);

  // Calculates VEV vectors from _beta and _xi.
  inline void calc_vev_tensors();

  /**
   * @brief: Updates the generic Yukawa sector (etaiF) from the Higgs Yukawa
   *         sector (_kF, _rF).
   * Full transformation:
   *  1.) Goes to fermion flavor basis using the CKM matrix,
   *      i.e. _kD-> VCKM _kD, _rD-> VCKM _rD.
   *  2.) Goes to generic basis by unitary transformation in Higgs flavor
   *      space.
   **/
  void calc_eta_from_kF_rF_ckm();

  /**
   * @brief: Updates the Higgs Yukawa sector, _kF & _rF, from from rhe eta
   *         matrices.
   *
   * Full transformation:
   *  1.) Goes to Higgs basis by unitary transformation of the eta matrices in
   *      Higgs flavor space.
   *  2.) Goes to fermion mass eigenbasis by performing a biunitary
   *      transformation on _kF and _rF. This diagonalizes _kF and updates the
   *      CKM matrix.
   */
  void calc_kF_rF_ckm_from_etaF();

  /**
   * @brief: Enforces the tree-lvl tadpole equations.
   *
   *  Fixes M112, M222 and the phase xi if beta is non-zero. TODO!
   *  If beta = 0, the generic basis is the Higgs basis and M112 and M12
   *  are fixed instead.
   *
   * @returns true if successful; false otherwise, if M12 is too small.
   */
  bool fix_treeLvl_tadpole_eqs();

  void init(); // Called at construction

  void print_z2Symmetry() const; // Prints Yukawa sectors Z2 symmetry

  // Functions that are called in RG evolution and produces data output to files
  void create_data_files();
  void write_to_data_files(const double &t); // t is renormalization scale

private:
  // @params: 2HDM parameters

  Z2symmetry _z2_symmetry; // Z2 sym of Yukawa sector
                           // (NO_SYMMETRY, TYPE_I, TYPE_II, TYPE_III, TYPE_IV)

  double _v2; // VEV of both Higgs doublets, v_1^2 + v_2^2.

  // Compact notation for the VEVs:
  std::complex<double> _vhat[2], _what[2], _V[2][2], _W[2][2];

  Base_generic _base_generic; // Scalar potential in generic basis.

  // Yukawa sector (etaF in generic basis, kF and rF are in Higgs basis)
  Eigen::Matrix3cd _eta1U, _eta2U, _eta1D, _eta2D, _eta1L, _eta2L;
  Eigen::Matrix3cd _kU, _rU, _kD, _rD, _kL, _rL;

  double _mh[3]; // Tree-lvl neutral Higgs masses
  double _mHc;   // Tree-lvl charged Higgs mass

  // @params: Standard model parameters
  Eigen::Matrix3cd _VCKM; // CKM matrix
  double _g1, _g2, _g3;   // gauge couplings ( U(1)_Y, SU(2)_W, SU(3)_c )
  double _mup[3], _mdn[3], _ml[3]; // Tree-lvl fermion masses

#ifdef SPHENO
  Spheno _spheno; // Class that handles SPheno calculations
#endif

  // FileSystem handles all the output to files.
  // Each filename is stored as a string for easy access to print to it.
  std::string _lambdaFile, _ZFile, _MijFile, _eta1UFile, _eta2UFile, _eta1DFile,
      _eta2DFile, _eta1LFile, _eta2LFile, _VEVFile, _gaugeFile, _massesFile,
      _kUFile, _kDFile, _kLFile, _rUFile, _rDFile, _rLFile, _z2Breaking;

  //------Static-variables------------------------------------------------------

  // Limit for machine precision
  constexpr static double _MACHINE_PRECISION = 1.E-12;
};

} // namespace THDME