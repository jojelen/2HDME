// DemoSM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Demo of RG evolution of the SM
 * @author: Joel Oredsson
 *
 * Simple example of running the standard model up to the Planck scale, 1e18 
 * GeV. Stability should break down at around 10^11 GeV using the 2-loop
 * RGEs (set by default).
 * 
 *============================================================================*/
#include "SM.h"
#include "HelpFunctions.h"

#include <iostream>

using namespace std;
using namespace THDME;

int main(int argc, char *argv[])
{
  Timer timer; // Prints running time at the end

  // Creates a SM at the top mass scale.
  SM sm;
  sm.print_all();

  // One can save the current state of a RgeModel with
  sm.save_current_state();

  sm.evolve_to(1e18);
  sm.print_rgeResults();
  sm.print_all();

  // One can also save the SM as a SLHA file.
  // So first reset to the saved state and then print model file.
  sm.reset_to_saved_state();
  sm.write_slha_file("DemoSM_SLHA");

  std::cout << "DemoSM complete!\n\n";
}
