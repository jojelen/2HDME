// NewModel.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Example of a simple model
 * @author: Joel Oredsson
 *
 * This is ań example of a simple model that uses the RGE functionality of
 * 2HDME. It represents the theory of QED and has only one parameter, the gauge
 * coupling g1.
 *
 * Study this class to implement your own class that inherits from RgeModel.
 *
 * The beta functions are stored in rgeFuncNewModel_1loop (and _2loop); which
 * need to be modified when extending the model.
 *
 * Derived class:
 *  BaseModel <- RgeModel <- NewModel
 *
 *============================================================================*/
#pragma once

#include "RgeModel.h"

namespace THDME {

// Beta functions for the models parameters.
int rgeFuncNewModel_1loop(double t, const double y[], double dydt[],
                          void *params);
int rgeFuncNewModel_2loop(double t, const double y[], double dydt[],
                          void *params);

//-----------------------------------------------------------------------------

class NewModel : public RgeModel {
   public:
    /**
     * @brief: Constructor
     */
    NewModel();

    /**
     * @brief: Copy and assignment constructor
     *
     * Calls set_from_thdm() that copies a thdm.
     */
    NewModel(const NewModel &model);
    NewModel &operator=(const NewModel &model);
    void set_from_model(const NewModel &model);

    ~NewModel();

    //---Override-functions-------------------------------------------------------

    /**
     * @brief: Functions used by RGE_system when performing RG evolution.
     *
     * rge_update: Updates the parameters of the model.
     *             @returns true if the update didn't result in any NaN values.
     * rge_finished: Can do something at the end of the run.
     */
    bool rge_update(const double y[], const double ti) override;
    void rge_finished() override;

    /**
     * @brief: Checks perturbativity
     *
     * @returns false if the perturbativity constraint is violated;
     * true otherwise.
     */
    bool is_perturbative() const override;

    /**
     * @brief: Checks unitary
     *
     * Implement unitarity check here.
     */
    bool is_unitary() const override;

    /**
     * @brief: Checks stability
     *
     * Implement stability check here.
     */
    bool is_stable() const override;

    /**
     * @brief: Sets parameters of the NewModel to and from an array.
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
     * Sets parameters to zero.
     * If one supplies a pointer to a double array y[], it resets all the
     * parameters to those as well.
     *
     * Should return false if it cannot reset with the given array.
     * */
    bool reset(const double y[], const double &renormScale) override;
    bool reset_to_saved_state() override;

    /**
     * @brief: Prints info about the model to the console
     */
    void print_all() const override;

    /*------------------------------------------------------------------------*/

   private:
    void init();  // Called at construction

   private:
    double _g1;  // Electromagnetic gauge coupling.
};

}  // namespace NewModelE
