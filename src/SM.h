// SM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: SM class with RGE functionality
 * @author: Joel Oredsson
 *
 * Derived class:
 *  BaseModel <- RgeModel <- SM
 * 
 * This class represents the Standard Model. All parameters are initialized to
 * some default values (contained in the constructor), which are defined at the
 * top mass renormalization scale. To change these, use for example 
 * "set_params(...)" below.
 * 
 * This class can be used to set the SM parameters of a THDM object, i.e. the
 * gauge couplings, VEV, fermion masses and CKM matrix.
 * 
 * A SM class object can be evolved in renormalization energy just like a THDM
 * object.
 *
 * Example usage: See DemoSM.cpp
 *
 *============================================================================*/
#pragma once

#include "RgeModel.h"

#include <Eigen/Dense>

namespace THDME
{

class SM : public RgeModel
{
public:
  SM();
  ~SM();

  /**
   * @brief: Setting the SM parameters
   * 
   * This function can be used to manually set all the SM member variables.
   */
  void set_params(const double &renormScale, const double &lambda,
                  const double &v2, const std::vector<double> &g,
                  const std::vector<double> &mup,
                  const std::vector<double> &mdn, const std::vector<double> &ml,
                  Eigen::Matrix3cd vCkm);

  void set_higgs(const double &v2, const double &lambda);
  void set_gauge(const double &g1, const double &g2, const double &g3);

  // These sets the corresponing fermion masses/CKM matrix and recalculates
  // the Yukawa matrices
  void set_mup(const double &mu, const double &mc, const double &mt);
  void set_mdn(const double &md, const double &ms, const double &mb);
  void set_ml(const double &me, const double &mmu, const double &mtau);
  void set_ckm(const Eigen::Matrix3cd &vCkm);

  double get_v2() const;
  std::vector<double> get_gauge_couplings() const;
  std::vector<double> get_mup() const;
  std::vector<double> get_mdn() const;
  std::vector<double> get_ml() const;
  Eigen::Matrix3cd get_vCkm() const;
  double get_lambda() const;

  /*-----Override-BaseModel-functions-----------------------------------------*/

  void print_all() const override;

  /*-----Override-RgeModel-functions------------------------------------------*/

  /**
   * @brief: Updates the parameters during RG evolution.
   * @returns false if RG evolution should stop; true otherwise.
   */
  bool rge_update(const double y[], const double ti) override;
  void rge_finished() override;

  /**
   * @brief: Fills the y[] array with the parameters of the derived model,
   *         or the opposite for set_model_from_y.
   */
  void set_y(double y[]) const override;
  void set_model_from_y(const double y[]) override;

  bool reset(const double y[], const double &renormScale) override;
  bool reset_to_saved_state() override;

  /**
   * @brief: Writes a SLHA file
   *
   * Writes all the information of the SM to a SLHA file.
   */
  void write_slha_file(const std::string &file = "LesHouches.SM") const;
  bool set_from_slha_file(const std::string &file);
  void fill_y(double y[], const std::string &blockType,
              std::ifstream &ifStream);

  /**
   * @brief: Does nothing, @returns true.
   */
  bool is_perturbative() const override;
  bool is_unitary() const override;
  bool is_stable() const override;

  /*--------------------------------------------------------------------------*/

private:
  // Calculates _yU, _yD and _yL from the given fermion masses and CKM matrix
  // Also sets the masses and CKM member variables.
  void calc_yukawa_matrices(const std::vector<double> &mup,
                            const std::vector<double> &mdn,
                            const std::vector<double> &ml,
                            const Eigen::Matrix3cd &vCkm);

  // Calculates the fermion masses, as well as the CKM matrix, from the
  // Yukawa matrices.
  void calc_fermionMasses_and_ckm();

private:
  // SM Lagrangian ms-bar parameters:
  double _g1, _g2, _g3;            // Gauge couplings
  double _lambda, _v2;             // Potential
  Eigen::Matrix3cd _yU, _yD, _yL;  // Yukawa matrices
  Eigen::Matrix3cd _VCKM;          // CKM matrix
  double _mup[3], _mdn[3], _ml[3]; // Fermion masses
};

} // namespace THDME