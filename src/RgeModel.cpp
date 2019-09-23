// RgeModel.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Base class for models with possibility for RG evolution
 * @author: Joel Oredsson
 *============================================================================*/
#include "RgeModel.h"
#include "HelpFunctions.h"

namespace THDME {

double RgeModel::_ODE_STEP = 1e-6;
int RgeModel::_ODE_PARAMS = 10;

RgeModel::RgeModel()
{
  _odeSystem_1loop = new gsl_odeiv2_system;
 
  _odeSystem_2loop = new gsl_odeiv2_system;
  
  _odeDriver = nullptr;

  _ySaved = nullptr;
}
 
RgeModel::~RgeModel() 
{
  if (_odeDriver != nullptr)
    gsl_odeiv2_driver_free(_odeDriver);
  
  if (_odeSystem_1loop != nullptr) 
    delete _odeSystem_1loop;
    
  if (_odeSystem_2loop != nullptr)
    delete _odeSystem_2loop;

  if (_ySaved != nullptr) 
    delete[] _ySaved;
}

bool RgeModel::init_rge_systems(int rgeFunc_1loop(double t, const double y[],
                                                  double dydt[], void *params),
                                int rgeFunc_2loop(double t, const double y[],
                                                  double dydt[], void *params),
                                const int numParams)
{
    _numParams = numParams;
  *_odeSystem_1loop = {rgeFunc_1loop, jac, _numParams, &_ODE_PARAMS};
  *_odeSystem_2loop = {rgeFunc_2loop, jac, _numParams, &_ODE_PARAMS};

  if (_odeSystem_1loop == nullptr || _odeSystem_2loop == nullptr){
    _console.error << "[ERROR]: Couldn't set rge systems.\n";
    return false;
  }
  else
    return init_rge_driver();
}

bool RgeModel::init_rge_driver()
{
  if (_odeDriver != nullptr)
    gsl_odeiv2_driver_free(_odeDriver);

  /**
   * gsl_odeiv2_driver_alloc_y_new returns a pointer to a newly allocated
   * instance of a driver object.The function automatically allocate and
   * initialise the evolve, control and stepper objects for ODE system "sys"
   * using stepper type "gsl_odeiv2_step_rkf45". This pointer must be deleted
   * with gsl_odeiv2_driver_free(driver) to prevent memory leaks!
   */
  if (_rgeConfig.twoloop)
  {
    _odeDriver = gsl_odeiv2_driver_alloc_y_new(
        _odeSystem_2loop, gsl_odeiv2_step_rkf45, _ODE_STEP, 1e-6, 0.);
  }
  else{
    _odeDriver = gsl_odeiv2_driver_alloc_y_new(
        _odeSystem_1loop, gsl_odeiv2_step_rkf45, _ODE_STEP, 1e-6, 0.);
    }

  if (_odeDriver == nullptr)
  {
    _console.error << "[ERROR]: Couldn't allocate gsl_odeiv2_driver.\n";
    return false;
  }
  return true;
}

bool RgeModel::evolve()
{
  // Timer timer("RgeModel evolve():"); // DEBUG TIMER

  if (_odeDriver != nullptr)
  {
    // Initialize start and final energy scale
    const double t0 = log(_renormScale);
    const double t1 = log(_rgeConfig.finalEnergyScale);

    // Change step direction for backwards evolution
    if (t1 < t0)
    {
      int changeDirectionStatus =
          gsl_odeiv2_driver_reset_hstart(_odeDriver, -_ODE_STEP);
      if (changeDirectionStatus != 0)
      {
        _console.error << "[ERROR]: Couldn't change direction of evolution.\n";
        return false;
      }
    }

    // Set start and final energies
    _rgeResults.reset();
    _rgeResults.e0 = _renormScale;
    _rgeResults.ef = _rgeConfig.finalEnergyScale;
    _rgeResults.evolved =
        false; // Means the model has not been evolved in energy

    // Array of parameters that is given to ODE solver.
    double y[_numParams];
    set_y(y);

    // Running parameter, t = log(mu). Initialized to start energy scale
    double t = t0;

    /* Bool that wether to check for violations of specific features. Changed to
    false if it is violated. */
    bool Check_perturbativity = true;
    bool Check_unitarity = true;
    bool Check_stability = true;

    bool failed = false; // If the RG evolution failed

    // Evolve RGE from starting to stop scale
    _console.info
        << "************************************************************\n";
    _console.info << "Evolving " << _modelName << " from " << exp(t0)
                  << " GeV to " << exp(t1) << " GeV\n";
    if (_rgeConfig.twoloop)
      _console.info << "Using 2-loop RGEs\n";
    else
      _console.info << "Using 1-loop RGEs\n";
    _console.info
        << "************************************************************\n";
    _console.info << "\n";
    for (int i = 0; i <= _rgeConfig.steps; i++)
    {
      // Calculate new energy ti and evolve y[t] to y[ti]
      double ti = t0 + i * (t1 - t0) / ((double)_rgeConfig.steps);
      if (!take_ode_step(t, ti, y))
      {
        failed = true;
        _rgeResults.ef = exp(ti);
        break;
      }

      // Print progress status.
      if (_console.get_logLevel() > LOG_INFO - 1)
      {
        print_prog_bar((int)(((double)i / ((double)_rgeConfig.steps)) * 100));
        std::cout << " Current energy: " << exp(ti) << " GeV " << std::flush;
      }

      // Updates the parameters of the model.
      if (!rge_update(y, ti))
        break;

      if (Check_perturbativity)
        if (!is_perturbative())
        {
          _console.warning << "[WARNING]: Perturbativity breakdown!\n";
          Check_perturbativity = false;
          _rgeResults.perturbativityViolation = true;
          _rgeResults.ef_pert = exp(ti);
          if (_rgeConfig.perturbativity)
            break;
        }

      if (Check_unitarity)
        if (!is_unitary())
        {
          _console.warning << "[WARNING]: Unitarity breakdown!\n";
          Check_unitarity = false;
          _rgeResults.unitarityViolation = true;
          _rgeResults.ef_unit = exp(ti);
          if (_rgeConfig.unitarity)
            break;

        }

      if (Check_stability)
        if (!is_stable())
        {
          _console.warning << "[WARNING]: Stability breakdown!\n";
          Check_stability = false;
          _rgeResults.stabilityViolation = true;
          _rgeResults.ef_stab = exp(ti);
          if (_rgeConfig.stability)
            break;
        }
    }

    _console.info
        << "          \n\n"
        << "************************************************************\n";

    // Change back step direction to default forward direction
    if (t1 < t0)
    {
      int changeDirectionStatus =
          gsl_odeiv2_driver_reset_hstart(_odeDriver, _ODE_STEP);
      if (changeDirectionStatus != 0)
      {
        _console.error << "[ERROR]: Couldn't change direction of evolution.\n";
        return false;
      }
    }

    rge_finished();
    _rgeResults.evolved = true;

    if (_console.get_logLevel() > LOG_INFO - 1)
      _rgeResults.print();

    if (failed)
      return false;

    return true;
  }
  else
  {
    _console.error << "[ERROR]: Couldn't evolve RgeModel. Need to initialize "
                      "_rgeDriver first.\n";
    return false;
  }
}

bool RgeModel::evolve_to(const double mu)
{
  _rgeConfig.finalEnergyScale = mu;
  return evolve();
}

bool RgeModel::take_ode_step(double &t, const double &ti, double y[])
{
  int status = gsl_odeiv2_driver_apply(_odeDriver, &t, ti, y);

  // Check for failure of gsl ode solver.
  if (status != 0)
  {
    _console.warning << "[WARNING]: Error in gsl's ODE solver.\n";
    _console.warning << "Probably due to Landau pole at scale " << exp(ti)
                     << std::endl;
    _console.warning << "return value = " << status << std::endl;
    _rgeResults.LandauViolation = true;
    return false;
  }

  // Check y array for NaN values.
  for (unsigned int i = 0; i < _numParams; i++)
    if (std::isnan(y[i]))
    {
      _console.error << "[ERROR]: NaN value detected in y[], element " << i
                     << std::endl;
      _console.info << "return value = " << status << std::endl;
      _console.info << "scale = " << exp(ti) << std::endl;
      _console.info << "step = " << i << std::endl;
      return false;
    }

  return true;
}

void RgeModel::set_final_energy_scale(const double energy)
{
  _rgeConfig.finalEnergyScale = energy;
}

void RgeModel::set_rgeConfig(const RgeConfig &rgeConfig)
{
  if (!rgeConfig.consoleOutput)
    _console.set_logLevel(LOG_ERRORS);

  if (rgeConfig.twoloop != _rgeConfig.twoloop)
  {
    _rgeConfig = rgeConfig;
    init_rge_driver();
  }
  else
    _rgeConfig = rgeConfig;
}

RgeConfig RgeModel::get_rgeConfig() const { return _rgeConfig; }

void RgeModel::set_rgeResults(const RgeResults &rgeResults)
{
  _rgeResults = rgeResults;
} 

RgeResults RgeModel::get_rgeResults() const { return _rgeResults; }

void RgeModel::set_renormalization_scale(const double &renormScale)  
{ 
  _renormScale = renormScale;
}

double RgeModel::get_renormalization_scale() const { return _renormScale; }

unsigned int RgeModel::get_numParams() const { return _numParams; }

void RgeModel::print_rgeResults() const { _rgeResults.print(); }

void RgeModel::save_current_state() 
{
  _ySaved = new double[_numParams];
  _savedRenormScale = _renormScale;

  set_y(_ySaved);
}
}