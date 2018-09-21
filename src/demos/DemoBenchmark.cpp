// DemoBenchmark.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Demo for debugging
 * @author: Joel Oredsson
 *============================================================================*/
#include "HelpFunctions.h"
#include "ParameterScan.h"
#include "SM.h"
#include "THDM.h"

#include <gsl/gsl_rng.h>
#include <iostream>

// Create random number generator
// gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);

using namespace std;
using namespace THDME;

int main(int argc, char *argv[])
{

  // Initializes time zero
  programTime(true);

  Base_hybrid hyb;
  hyb.mh = 80.;
  hyb.mH = 125.;
  hyb.cba = 1.;
  hyb.Z4 = -5.;
  hyb.Z5 = -5.;
  hyb.Z7 = 0.;
  hyb.tanb = 1.5;

  Base_higgs higgs = hyb.convert_to_higgs(  246*246);

  std::cout << "Higgs basis: \n" << higgs;

  SM sm;
  THDM thdm(sm);

  thdm.set_param_hybrid(hyb);
  thdm.set_yukawa_type(TYPE_I);
  thdm.print_all();
  
  Base_hybrid hyb2 = thdm.get_param_hybrid();
  std::cout << hyb2 << std::endl;


  Base_invariant inv = thdm.get_param_invariant();
  std::cout << inv << std::endl;
  // Finished!
  printf("Benchmark complete!\n");
  programTime(true); // Prints out the CPU time of the program (not actual time
                     // when using multiple threads.)
  printf("\n");
}
