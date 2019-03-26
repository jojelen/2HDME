// RgeModel.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Base class for models with possibility for RG evolution
 * @author: Joel Oredsson
 * 
 * This is an abstract class, where many features should be implemented in
 * derived classes.
 * All virtual functions must be overridden in derived classes.
 * 
 * RgeModel covers the functionality that models with RGE possibilities should 
 * inherit. It inherits basic model 
 * independent features from its base class BaseModel.
 * 
 * Example of derived classes:
 *  BaseModel <- RgeModel <- THDM
 * 
 *============================================================================*/
#pragma once

#include "BaseModel.h"
#include "Structures.h"
#include "LoggingSystem.h"
#include "FileSystem.h"

#include <gsl/gsl_odeiv2.h>
#include <string>

namespace THDME {

/*------------------------------------------------------------------------------
 *  Main usage:
 *   1.) Each derived model must implement all the virtual functions according
 *    to specifications.
 *   2.) The init_rge_system function must be called before any RG evolution.
 *    Probably best to do at initialization of
 *    derived model. As an argument, it takes a function that calculated
 *    dy/dt from y[] and t. Examples are collected in RGE.cpp.
 *   3.) The evolve function can be used to perform the RG evolution. There are
 *    different options one can use that are set with set_rgeConfig function.
 *----------------------------------------------------------------------------*/
class RgeModel : public BaseModel
{
public:
  RgeModel();

  virtual ~RgeModel();

  /*-----Virtual-functions----------------------------------------------------*/

  /**
   * @brief: Updates the model during RG evolution.
   * 
   * After each step in the RG evolution, this function is called. It should 
   * set the models parameters from the y[] array and may perform additional 
   * checks. 
   * 
   * @returns false if RG evolution should stop; true otherwise.
   */
  virtual bool rge_update(const double y[], const double ti) = 0;

  /** 
   * @brief: Called at the end of RG evolution
   * 
   * The functionality is not restricted and can be made to suit the derived 
   * models needs.
   */
  virtual void rge_finished() = 0;
  

  /**
   * @brief: Fills the y[] array with the parameters of the derived model.
   */
  virtual void set_y(double y[]) const = 0;

  /**
   * @brief: Sets the model's parameters from the y[] array.
   */
  virtual void set_model_from_y(const double y[]) = 0;

  /** 
   * @brief: Checks perturbativity
   * 
   * @returns false if the perturbativity constraint is violated, true otherwise
   */
  virtual bool is_perturbative() const = 0;

  /** 
   * @brief: Checks unitarity
   * 
   * @returns false if the unitarity constraint is violated, true otherwise
   */
  virtual bool is_unitary() const = 0;

  /** 
   * @brief: Checks stability
   * 
   * @returns false if the stability is violated, true otherwise
   */
  virtual bool is_stable() const = 0;

  /**
   * @brief: Resets the model
   *
   * Resets the model to a state either from an array with "reset" or to a 
   * saved state (previously saved with save_current_state() ) with
   * "reset_to_saved_state".
   *
   * @returns false if it fails setting the model.
   */
  virtual bool reset(const double y[], const double &renormScale) = 0;
  virtual bool reset_to_saved_state() = 0;

  //----------------------------------------------------------------------------

  /**
   * @brief: Evolves models parameters in energy
   * 
   * Performs RG evolution according to the RGEs specified by the function used 
   * in init_rge_system. The details of the running performed is stored in 
   * _options; which is set by set_rgeConfig.
   * 
   * It performs checks of stability, unitarity and perturbativity along the 
   * RG running.
   * 
   * evolve_to(mu) simply sets the _rgeConfig.finalEnergyScale to mu before 
   * evolving.
   */
  bool evolve();
  bool evolve_to(const double mu);

  /**
   * @brief: Saves the current state of the model
   * 
   * Saves the models parameters in an array, _ySaved. The model can be 
   * initialized to this state at a later time with the function 
   * "reset_to_saved_state".
   */
  void save_current_state();

  /**
   * @brief: Sets/gets the specifics of the RGE running.
   */
  void set_rgeConfig(const RgeConfig &rgeConfig);
  RgeConfig get_rgeConfig() const;

  
  // @brief: Sets final evolution scale
  void set_final_energy_scale(const double energy);

  /**
   * @brief: Sets/gets the Evolution of the model (contains results from RG
   *         evolution)
   * 
   * The print function, prints to the console.
   */
  void set_rgeResults(const RgeResults &rgeResults);
  RgeResults get_rgeResults() const;
  void print_rgeResults() const;

  /**
   * @brief: Returns the current renormalization scale $\mu$ of the model
   */
  void set_renormalization_scale(const double &renormScale);
  double get_renormalization_scale() const;

  /**
   * @brief: Returns the number of parameters in y[], i.e. _numParams.
   */
  unsigned int get_numParams() const;

protected:
  /**
   * @brief: Set RGE ode systems
   */
  bool init_rge_systems(int rgeFunc_1loop(double t, const double y[], 
                                                    double dydt[],
                                void *params),
                        int rgeFunc_2loop(double t, const double y[], double dydt[],
                                void *params),
                        const int numParams);

  double* _ySaved; // Stored parameters from save_current_state.
  double _savedRenormScale; // Energy scale where _ySaved is defined.

  std::size_t _numParams; // Number of running parameters in the y[] array.
  unsigned int _rgeLoopOrder;
  double _renormScale; // Renormalization scale.
  RgeResults _rgeResults;
  RgeConfig _rgeConfig;  // Options used for any RGE evolution.
  

private:
  /**
   * @brief: Initializes _odeSystem and _odeDriver
   * 
   * @returns true if successful; false otherwise
   */
  bool init_rge_driver();

  /**
   * @brief: Evolves parameters y[] from t to ti
   * 
   * @returns false if the gsl ODE solver runs into problems (for example 
   *          Landau pole) or if there is a NaN value in y[].
   */
  bool take_ode_step(double &t, const double &ti, double y[]);

  gsl_odeiv2_system *_odeSystem_1loop;
  gsl_odeiv2_system *_odeSystem_2loop;
  gsl_odeiv2_driver *_odeDriver;

  // step size and params used for gsl's ode solver
  static double _ODE_STEP;
  static int _ODE_PARAMS;
};
}