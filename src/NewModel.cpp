// NewModel.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Example of a simple model
 * @author: Joel Oredsson
 *
 * See header file for details.
 *
 *============================================================================*/
#include "NewModel.h"
#include "Globals.h"

namespace THDME {

NewModel::NewModel() : _g1(0.35830) { init(); }
NewModel::NewModel(const NewModel &model) { set_from_model(model); }
NewModel &NewModel::operator=(const NewModel &model) {
  set_from_model(model);
  return *this;
}
void NewModel::set_from_model(const NewModel &model) {
  _console.error << "[ERROR]: No copy functions implemented.\n";
}
NewModel::~NewModel() {}
bool NewModel::rge_update(const double y[], const double ti) {
  _rgeResults.ef = exp(ti);
  _renormScale = _rgeResults.ef;
  set_model_from_y(y);
  return true;
}
void NewModel::rge_finished() {}
bool NewModel::is_perturbative() const {
  if (_g1 < 4. * M_PI)
    return true;
  else
    return false;
}
bool NewModel::is_unitary() const { return true; }
bool NewModel::is_stable() const { return true; }
void NewModel::set_y(double y[]) const { y[0] = _g1; }
void NewModel::set_model_from_y(const double y[]) { _g1 = y[0]; }
bool NewModel::reset(const double y[], const double &renormScale) {
  set_model_from_y(y);
  _renormScale = renormScale;
  return true;
}
bool NewModel::reset_to_saved_state() {
  _rgeResults.reset();

  if (_ySaved != nullptr) {
    reset(_ySaved, _savedRenormScale);
    return true;
  } else {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
}
void NewModel::print_all() const {
  _console.info << "\n----------------------------------------\n";
  _console.info << "QED (NewModel):\n";
  _console.info << "Renormalization scale = " << _renormScale << " GeV\n";
  _console.info << "g1 = " << _g1 << std::endl;
  _console.info << "----------------------------------------\n";
}

void NewModel::init() {
  _modelName = "NewModel";

  // Points to RGE functions.
  // NUM_PARAMS is the total number of parameters in RGE system. This must be
  // modified when adding additional RGEs.
  constexpr static int NUM_PARAMS = 1;
  init_rge_systems(rgeFuncNewModel_1loop, rgeFuncNewModel_2loop, NUM_PARAMS);

  _renormScale = Global::mt;
}

//------------------------------------------------------------------------------

int rgeFuncNewModel_1loop(double t, const double y[], double dydt[],
                          void *params) {
  double Dg1_1;

  // Initial conditions
  double g1 = y[0];

  /*---------Calculating-RGEs-------------------------------------------------*/

  Dg1_1 = (3. * pow(g1, 2.)) / 4.;
  // fill derivatives in dydt

  static double pif = 1. / (16. * M_PI * M_PI);

  dydt[0] = pif * Dg1_1;

  return GSL_SUCCESS;
}

int rgeFuncNewModel_2loop(double t, const double y[], double dydt[],
                          void *params) {
  double Dg1_1, Dg1_2;

  // Initial conditions
  double g1 = y[0];

  /*---------Calculating-RGEs-------------------------------------------------*/

  Dg1_1 = (3. * pow(g1, 2.)) / 4.;
  // Twoloop terms
  Dg1_2 = -431. * pow(g1, 4.) / 96.;

  // fill derivatives in dydt
  static double pif = 1. / (16. * M_PI * M_PI);

  dydt[0] = pif * Dg1_1 + pif * pif * Dg1_2;

  return GSL_SUCCESS;
}
} // namespace THDME