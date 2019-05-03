// DemoNewModel.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Demo of the simple NewModel class
 * @author: Joel Oredsson
 *
 * The NewModel represents the theory of QED and has only a single parameter, 
 * the gauge coupling g1.
 * At 1-loop, it breaks perturbativity at around 1e252.
 * 
 *============================================================================*/
#include "NewModel.h"
#include "HelpFunctions.h" // For Timer

#include <iostream>

using namespace std;
using namespace THDME;

int main(int argc, char *argv[])
{
  Timer timer; // Prints running time at the end

  NewModel model;
  model.print_all();

  model.evolve_to(1e18);
  model.print_all();
  
  model.evolve_to(1e300);
  model.print_all();

  std::cout << "DemoNewModel complete!\n\n";
}
