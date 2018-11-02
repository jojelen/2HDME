// DemoRGE.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Demo of RG evolution of a 2HDM
 * @author: Joel Oredsson
 *
 * Evolves a CP conserving type II 2HDM up to the Planck scale.
 *
 * This demonstrates the basic functionality of specifying a THDM at the top
 * mass scale and running it up to the Planck scale.
 *
 *============================================================================*/
#include "SM.h"
#include "THDM.h"

#include <complex>
#include <iostream>

using namespace THDME;

int main(int argc, char *argv[])
{
  Timer timer;

  // The Standard Model is by default constructed with its parameters defined at
  // the top mass scale.
  SM sm;
  sm.print_all();

  // The sm object is used to set the boundary condition for a THDM object.
  // The THDM will be defined at the same renormalization scale and with the
  // same fermion masses, gauge coupling, CKM matrix as the given sm.
  THDM thdm(sm);

  // The potential of the THDM can be set with any of the bases specified in
  // THDM_bases. Here we specify a Z2 symmetric CP conserving generic basis.
  Base_generic gen;

  gen.beta = 1.46713; // tanb =  9.61166
  gen.M12 = std::complex<double>(3132.85, 0.);
  gen.Lambda1 = 0.413702;
  gen.Lambda2 = 0.263926;
  gen.Lambda3 = 0.13313;
  gen.Lambda4 = -0.0444794;
  gen.Lambda5 = std::complex<double>(0.29586, 0.);
  gen.Lambda6 = std::complex<double>(0., 0.);
  gen.Lambda7 = std::complex<double>(0., 0.);

  thdm.set_param_gen(gen);

  // The Yukawa sector is set to be type-I symmetric. A potential needs to be
  // set before this, since tanb must be specified.
  thdm.set_yukawa_type(TYPE_I);

  // Calculates 1-loop masses if SPheno is enabled:
  #ifdef SPHENO
  thdm.run_spheno(1); // Argument is loop lvl (0, 1 or 2).
  #endif

  thdm.print_all();

  // One can also write a SLHA file of the THDM and SM
  sm.write_slha_file();
  int sphenoLoopOrder = 0; // Loop order for mass calculations if SPheno is used
  thdm.write_slha_file(sphenoLoopOrder);

  // The options for evolving a THDM object can configured by specifying a
  // RgeConfig struct:
  RgeConfig options;
  options.dataOutput = true;         // Writes parameters to file in each RGE step.
  options.consoleOutput = true;      // Writes info to console
  options.evolutionName = "DemoRGE"; // Output directory
  options.twoloop = true;            // Uses 2-loop RGEs
  options.perturbativity = true;     // Stops at perturbativity violation
  options.stability = false;         // Continues at stability violation
  options.unitarity = false;         // Continues at unitarity violation
  options.finalEnergyScale = 1e18;
  options.steps = 100; // Number of steps in evolution where checks are being
                       // made and parameters are saved to files.

  options.print(); // Prints the options to the console

  thdm.set_rgeConfig(options);

  thdm.evolve();

  std::cout << "Parameters after evolution: \n";
  thdm.print_all();

  // There are plenty of parameters one can get from the thdm. See THDM.h for
  // a complete list of the implemented ones.
  // For example, to retrieve the potential parameters one can use
  Base_generic genFinal = thdm.get_param_gen();

  std::cout << "THDM API example:\n";
  std::cout << "\nLambda1 increased by "
            << std::abs(100 * genFinal.Lambda1 / gen.Lambda1) << " %\n";

  // We can save the evolved model in another file
  thdm.write_slha_file(sphenoLoopOrder, "DemoRGE_evolvedThdm");
  
  // To evolve downwards in energy, all one have to do is to change the 
  // finalEnergyScale of RgeConfig.
  options.finalEnergyScale = 1e3;
  options.dataOutput = false; // To prevent overwriting the first plots.
  thdm.set_rgeConfig( options);
  thdm.evolve();

  std::cout << "\nDemoRGE complete!\n\n";
}
