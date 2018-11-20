// Structures.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Misc structures
 * @author: Joel Oredsson
 *============================================================================*/
#pragma once

#include "HelpFunctions.h"
#include "THDM_bases.h"

#include <Eigen/Dense>
#include <complex>
#include <fstream>
#include <gsl/gsl_rng.h>
#include <iostream>
#include <mutex>
#include <string>

namespace THDME
{

/**
 * @brief Options for RG evolutions
 *
 * The RGE evolution is configured with a RGE_config struct.
 * The Results of the RGE evolution is saved in a RgeResults struct.
 */
struct RgeConfig
{
  bool twoloop; // set to true to use two-loop RGEs

  bool perturbativity; // if these = true, enforces
  bool stability;      // that the RGE evolution breaks when violating them.
  bool unitarity;

  bool consoleOutput;        // If false, hides output to console
  bool dataOutput;           // prints evolution data to files
  std::string evolutionName; // data is printed to "output/evolutionName"
  double finalEnergyScale;   // Final energy scale for RG evolution.
  int steps;                 // steps for RGE evolution (distributed
                             // logarithmically)

  RgeConfig();

  void print() const;
};

//------------------------------------------------------------------------------

/**
 * @brief: Results of RG evolution
 *
 * Struct that contains all information of a models
 * RG evolution.
 **/
struct RgeResults
{
  // Start and finish energy for RGE evolution
  double e0, ef;

  // The breakdown energy for perturbativity etc.
  // They are initialized to -1 to mark that they have not been violated.
  double ef_pert;
  double ef_unit;
  double ef_stab;

  bool evolved; // true if the THDM has been performed RGE_evolve

  // These are true if the model has
  // broken them during RGE evolion
  bool LandauViolation;
  bool perturbativityViolation;
  bool unitarityViolation;
  bool stabilityViolation;

  // Constructor
  RgeResults();

  // Copy constructor
  RgeResults(const RgeResults &rge);

  void reset();

  // Print to console
  void print(const bool fancyStyle = true) const;

};

/*----------------------------------------------------------------------------*/

// A structure that can be used to set a range for a double parameter.
struct double_range
{
  bool fixed;
  double fixedValue;
  double min, max;

  double_range();
  double_range(double min_in, double max_in);
  double_range(double value_in);

  // Returns random value between min and max or the fixedValue if fixed = true
  double draw_random(const gsl_rng *rng) const;

  // Overloading the << operator
  friend std::ofstream &operator<<(std::ofstream &of,
                                   const double_range &range);
  // Overloading the << operator
  friend std::ostream &operator<<(std::ostream &of, const double_range &range);

  std::string toString();
};

//------------------------------------------------------------------------------

/*******************************************************************************
 * @brief: Collection of parameter ranges for hybrid basis
 */
struct Range_hybrid
{
  double_range mh, mH, cba, tanb, Z4, Z5, Z7;
  std::string name;
  Z2symmetry yukawaType;

  Range_hybrid();

  void print() const;

  Base_hybrid get_random_point(const gsl_rng *rng) const;
};

} // namespace THDME