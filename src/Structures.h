// Structures.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Misc structures
 * @author: Joel Oredsson
 *============================================================================*/
#pragma once

#include "HelpFunctions.h"

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

/*----------------------------------------------------------------------------*/


// /**
//  * @Brief Gradient information
//  *
//  * The gradient of the breakdown energy of a THDM as a function of the
//  * generic parameters can be saved in Gradient, which stores information about
//  * the calculation.
//  * */
// struct Gradient
// {
//   int parameters;
//   double ef_start;
//   double DELTA; // Sets the stepsize when calculating derivatives
//   std::vector<double> derivative;
//   std::vector<double> energy;

//   // Constructor
//   Gradient() : parameters(12), ef_start(0.), DELTA(0.)
//   {
//     derivative.resize(parameters);
//     energy.resize(parameters);
//   }

//   // Function that returns max derivative
//   int maxDerivative() const
//   {
//     double max = 0.;
//     int maxIndex = 0;

//     for (int i = 0; i < parameters; i++)
//     {
//       if (std::abs(derivative[i]) > max)
//       {
//         maxIndex = i;
//         max = std::abs(derivative[i]);
//       }
//     }
//     return maxIndex;
//   }

//   // Normalizes the gradient vector
//   void normalize()
//   {
//     double sum = 0.;

//     for (int i = 0; i < parameters; i++)
//     {
//       sum += derivative[i] * derivative[i];
//     }
//     if (sum != 0.)
//     {
//       double sumSquaredInv = 1.0 / sqrt(sum);
//       for (int i = 0; i < parameters; i++)
//       {
//         derivative[i] *= sumSquaredInv;
//       }
//     }
//     else
//       std::cout << "[WARNING]: couldn't normalize gradient. It has length 0!\n";
//   }

//   // Print function
//   void print() const
//   {
//     std::cout << "Gradient at point with breakdown energy " << ef_start
//               << std::endl;
//     Table tab(3);
//     tab.add_row(
//         std::vector<std::string>{"Parameter", "def/dParameter", "Delta ef"},
//         true);
//     tab.add_row(std::vector<std::string>{"M222", stringAuto(derivative[0]),
//                                          stringAuto(energy[0] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Lambda1", stringAuto(derivative[1]),
//                                          stringAuto(energy[1] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Lambda2", stringAuto(derivative[2]),
//                                          stringAuto(energy[2] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Lambda3", stringAuto(derivative[3]),
//                                          stringAuto(energy[3] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Lambda4", stringAuto(derivative[4]),
//                                          stringAuto(energy[4] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Re(Lambda5)",
//                                          stringAuto(derivative[5]),
//                                          stringAuto(energy[5] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Im(Lambda5)",
//                                          stringAuto(derivative[6]),
//                                          stringAuto(energy[6] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Re(Lambda6)",
//                                          stringAuto(derivative[7]),
//                                          stringAuto(energy[7] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Im(Lambda6)",
//                                          stringAuto(derivative[8]),
//                                          stringAuto(energy[8] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Re(Lambda7)",
//                                          stringAuto(derivative[9]),
//                                          stringAuto(energy[9] - ef_start)});
//     tab.add_row(std::vector<std::string>{"Im(Lambda7)",
//                                          stringAuto(derivative[10]),
//                                          stringAuto(energy[10] - ef_start)});
//     tab.print();
//     std::cout << "\n";
//   }
// };

} // namespace THDME