// SM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: SM class with RGE functionality
 * @author: Joel Oredsson
 *
 * Derived class:
 *  BaseModel -> SM
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

  bool evolve_to(const double mu);

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
  void fill_y(double y[], const std::string &blockType, std::ifstream &ifStream);
  /**
   * @brief: Does nothing, @returns true.
   */
  bool is_perturbative() const override;
  bool is_unitary() const override;
  bool is_stable() const override;

  /*--------------------------------------------------------------------------*/
private:
  // SM Lagrangian ms-bar parameters:

  // Gauge couplings:
  double _g1, _g2, _g3;

  // Potential:
  double _lambda, _v2;

  // Yukawa sector:
  Eigen::Matrix3cd _yU, _yD, _yL, _VCKM;
  double _mup[3], _mdn[3], _ml[3];
  void init_yukawa_sector();
  void set_ckm_from_pdg();

  void calc_fermionMasses_and_ckm();
};

} // namespace THDME