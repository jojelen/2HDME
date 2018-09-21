// Structures.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Misc structures
 * @author: Joel Oredsson
 *============================================================================*/
#include "Structures.h"

namespace THDME
{

RgeConfig::RgeConfig()
    : twoloop(false), perturbativity(true), stability(true), unitarity(true),
      consoleOutput(true), dataOutput(false), evolutionName("evolution"),
      finalEnergyScale(1e18), steps(100) {}

void RgeConfig::print() const
{
  std::cout << "RGE options:\n";

  std::cout << "Evolution name: " << evolutionName << std::endl;

  auto check = [](const std::string &message, const bool boolean) {
    std::cout << message;
    if (boolean)
      std::cout << "true\n";
    else
      std::cout << "false\n";
  };
  check("Data output: ", dataOutput);
  check("Console output: ", consoleOutput);
  check("2-loop: ", twoloop);
  check("Break at perturbativity violation: ", perturbativity);
  check("Break at unitarity violation: ", unitarity);
  check("Break at stability violation: ", stability);

  std::cout << "End energy: " << finalEnergyScale << std::endl;
  std::cout << "ODE steps: " << steps << std::endl;
}

RgeResults::RgeResults()
    : e0(0), ef(0), ef_pert(-1.0), ef_unit(-1.0), ef_stab(-1.0), evolved(false),
      LandauViolation(false), perturbativityViolation(false),
      unitarityViolation(false), stabilityViolation(false) {}

// Copy constructor
RgeResults::RgeResults(const RgeResults &rge)
{
  e0 = rge.e0;
  ef = rge.ef;
  ef_pert = rge.ef_pert;
  ef_unit = rge.ef_unit;
  ef_stab = rge.ef_stab;
  evolved = rge.evolved;
  LandauViolation = rge.LandauViolation;
  perturbativityViolation = rge.perturbativityViolation;
  unitarityViolation = rge.unitarityViolation;
  stabilityViolation = rge.stabilityViolation;
}

void RgeResults::reset()
{
  e0 = 0.;
  ef = 0.;
  ef_pert = -1.0;
  ef_unit = -1.0;
  ef_stab = -1.0;
  evolved = false;
  LandauViolation = false;
  perturbativityViolation = false;
  unitarityViolation = false;
  stabilityViolation = false;
}

// Print to console
void RgeResults::print(const bool fancyStyle) const
{
  if (!evolved)
  {
    std::cout << "RGE: \x1B[33munevolved.\x1B[0m\n";
    return;
  }
  if (fancyStyle)
  {
    // \x1B[31m = bold red text color
    // \x0B[32m = green text color
    // \x1B[0m restores it.
    std::cout << "\n\x1B[4mRGE Results\x1B[0m" << std::endl;
    std::cout << "Evolved from " << e0 << " GeV to " << ef
              << " GeV: " << std::endl;
    if (LandauViolation)
      std::cout << "\x1B[1;31mLandau pole: " << ef << " GeV\x1B[0m"
                << std::endl;
    else
      std::cout << "\x1B[0;32mNo Landau pole!\x1B[0m" << std::endl;
    if (perturbativityViolation)
      std::cout << "\x1B[1;31mPerturbativity breakdown: " << ef_pert
                << " GeV\x1B[0m" << std::endl;
    else
      std::cout << "\x1B[0;32mPerturbativity satisfied!\x1B[0m" << std::endl;
    if (unitarityViolation)
      std::cout << "\x1B[1;31mTree-lvl unitarity breakdown: " << ef_unit
                << " GeV\x1B[0m" << std::endl;
    else
      std::cout << "\x1B[0;32mUnitarity satisfied!\x1B[0m" << std::endl;
    if (stabilityViolation)
      std::cout << "\x1B[1;31mStability breakdown: " << ef_stab << " GeV\x1B[0m"
                << std::endl;
    else
      std::cout << "\x1B[0;32mStability satisfied!\x1B[0m" << std::endl;

    std::cout << "\n";
  }
  else
  {
    std::cout << "\nResults" << std::endl;
    std::cout << "Evolved from " << e0 << " GeV to " << ef
              << " GeV: " << std::endl;
    if (LandauViolation)
      std::cout << "Landau pole: " << ef << " GeV" << std::endl;
    else
      std::cout << "No Landau pole!" << std::endl;
    if (perturbativityViolation)
      std::cout << "Perturbativity breakdown: " << ef_pert << " GeV"
                << std::endl;
    else
      std::cout << "Perturbativity satisfied!" << std::endl;
    if (unitarityViolation)
      std::cout << "Tree-lvl unitarity breakdown: " << ef_unit << " GeV"
                << std::endl;
    else
      std::cout << "Unitarity satisfied!" << std::endl;
    if (stabilityViolation)
      std::cout << "Stability breakdown: " << ef_stab << " GeV" << std::endl;
    else
      std::cout << "Stability satisfied!" << std::endl;
    std::cout << "\n";
  }
}

double_range::double_range() : fixed(false) {}

double_range::double_range(double min_in, double max_in)
    : fixed(false), min(min_in), max(max_in) {}

double_range::double_range(double value_in)
    : fixed(true), fixedValue(value_in) {}

double double_range::draw_random(const gsl_rng *rng) const
{
  if (fixed)
    return fixedValue;
  else
    return min + (max - min) * gsl_rng_uniform(rng);
  ;
}

// Overloading the << operator
std::ofstream &operator<<(std::ofstream &of, const double_range &range)
{
  if (range.fixed)
    of << range.fixedValue;
  else
    of << "( " << range.min << ", " << range.max << ")";
  return of;
}
// Overloading the << operator
std::ostream &operator<<(std::ostream &of, const double_range &range)
{
  if (range.fixed)
    of << range.fixedValue;
  else
    of << "( " << range.min << ", " << range.max << ")";
  return of;
}

std::string double_range::toString()
{

  std::string rangeString;

  if (fixed)
    rangeString = std::to_string(fixedValue);
  else
    rangeString =
        "( " + std::to_string(min) + ", " + std::to_string(max) + " )";

  return rangeString;
}

} // namespace THDME