// SPheno.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: SPheno wrapper class
 * @author: Joel Oredsson
 *============================================================================*/

#include "SPheno.h"
#include "HelpFunctions.h"
#include "THDM.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace THDME {

const std::string SPHENO_PATH = "SPheno-4.0.3";

//------------------------------------------------------------------------------

Spheno::Spheno()
    : _initialized(false), _massLoopLvl(1), _S(0.), _T(0.), _U(0.), _renormScale(173.34)
{
  _mh[0] = 0.;
  _mh[1] = 0.;
  _mh[2] = 0.;
  _mh[3] = 0.;
}

Spheno::~Spheno() {}

bool Spheno::run(const THDM &thdm, const int massLoopLvl)
{
  // Timer timer("SPheno run():"); // DEBUG
  set_mass_loop_lvl(massLoopLvl);
  // std::unique_lock<std::mutex> lck(_runningSpheno);
  // std::cout << "Running thread on id = " << std::this_thread::get_id() <<
  // std::endl;

  // Retrieving thread ID as a string, creating a temporary path
  std::ostringstream ss;
  ss << std::this_thread::get_id();
  std::string tempPath = "temp/" + ss.str();

  if (!create_folder_path(tempPath))
  {
    std::cout << "[ERROR]: SPheno couldn't create folder path.\n";
    return false;
  }
  // change_directory("temp/" + threadIdString);

  thdm.write_slha_file(_massLoopLvl, tempPath + "/LesHouches.in.THDM_GEN");

  // Running SPheno
  std::string sysCommand = "./" + SPHENO_PATH + "/bin/SPhenoTHDM_GEN  " +
                           tempPath + "/LesHouches.in.THDM_GEN " + tempPath +
                           "/SphenoOutput.THDM_GEN > /dev/null";
  if ( system(sysCommand.c_str()) == -1){
    std::cout << "[ERROR]: Couldn'r run SPheno.\n";
    return false;
  }

  
  if (!read_spheno_output(tempPath + "/SphenoOutput.THDM_GEN"))
  {
    std::cout << "[ERROR]: Couldn't read SPheno output!\n";
    return false;
  }
  append_results_to_slha(tempPath + "/LesHouches.in.THDM_GEN");
  _initialized = true;

  _renormScale = thdm.get_renormalization_scale();

  return true;
  // change_directory("../../");
}

bool Spheno::read_spheno_output(const std::string &outputFile)
{
  // Loading the output
  std::ifstream modelFileStream(outputFile, std::ios_base::in);

  if (!modelFileStream)
  {
    std::cout << "[ERROR]: Couldn't open SPheno.spc.THDM_GEN " << std::endl;
    return false;
  }

  std::stringstream sStream;
  sStream << modelFileStream.rdbuf();

  std::string blockStart = "particle";
  std::string SPhenoLowEnergy = "SPhenoLowEnergy";
  double tD;

  while (sStream)
  {
    std::string temp;
    sStream >> temp;

    if (temp == blockStart)
    {
      for (int i = 0; i < 4; ++i)
      {
        sStream >> temp;
        sStream >> tD;
        _mh[i] = tD;
        sStream >> temp;
        sStream >> temp;
      }
    }
    if (temp == SPhenoLowEnergy)
    {
      for (int i = 0; i < 5; ++i)
        sStream >> temp;
      sStream >> tD;
      _T = tD;
      for (int i = 0; i < 5; ++i)
        sStream >> temp;
      sStream >> tD;
      _S = tD;
      for (int i = 0; i < 5; ++i)
        sStream >> temp;
      sStream >> tD;
      _U = tD;
    }
  }

  return true;
}

bool Spheno::append_results_to_slha(const std::string &slhaFile) const
{
  FILE *output;
  output = fopen(slhaFile.c_str(), "a");

  if (output == NULL)
  {
    printf("Cannot open file \"%s\" for writing\n", slhaFile.c_str());
    return false;
  }

  fprintf(output, "Block SPhenoLowEnergy # low energy observables \n");
  fprintf(output, " 1   % 16.8e   # T-parameter (1-loop BSM)\n", _T);
  fprintf(output, " 2   % 16.8e   # S-parameter (1-loop BSM)\n", _S);
  fprintf(output, " 3   % 16.8e   # U-parameter (1-loop BSM)\n", _U);

  fprintf(output,
          "Block MASS # Pole masses calculated with SPheno (loop-lvl: %i)\n",
          _massLoopLvl);
  fprintf(output, "#   PDG code      mass          particle\n");
  fprintf(output, " 25   % 16.8e   # hh_1\n", _mh[0]);
  fprintf(output, " 35   % 16.8e   # hh_2\n", _mh[1]);
  fprintf(output, " 36   % 16.8e   # Ah_1\n", _mh[2]);
  fprintf(output, " 37   % 16.8e   # Hm_2\n", _mh[3]);

  fclose(output);

  return true;
}

std::vector<double> Spheno::get_output() const
{
  return std::vector<double>{_mh[0], _mh[1], _mh[2], _mh[3], _S, _T, _U, _renormScale};
}

void Spheno::set(const std::vector<double> &sphenoOutput)
{
  _initialized = true;
  _mh[0] = sphenoOutput[0];
  _mh[1] = sphenoOutput[1];
  _mh[2] = sphenoOutput[2];
  _mh[3] = sphenoOutput[3];
  _S = sphenoOutput[4];
  _T = sphenoOutput[5];
  _U = sphenoOutput[6];
  _renormScale = sphenoOutput[7];
}

void Spheno::set_mass_loop_lvl(int massLoopLvl) { _massLoopLvl = massLoopLvl; }

bool Spheno::is_allowed() const
{
  // std:cout << "SPheno calculated _mh[0] = " << _mh[0] << std::endl; // DEBUG
  if (!_initialized)
  {
    return false;
  }

  if (_mh[0] > 130. || _mh[0] < 120.)
  {
    // std::cout << "Higgs mass outside limits, mh[0] = " << _mh[0] << "
    // GeV.\n";
    return false;
  }

  // if ( _S > 0.12 || _S < -0.08){
  //   std::cout << "S outside limits, S = " << _S << std::endl;
  //   return false;
  // }
  // if ( _T > 0.19 || _T < -0.05){
  //   std::cout << "T outside limits, T = " << _T << std::endl;
  //   return false;
  // }
  // if ( _U > 0.09 || _U < -0.09){
  //   std::cout << "U outside limits, U = " << _U << std::endl;
  //   return false;
  // }

  return true;
}

void Spheno::print() const
{
  if (_initialized)
  {
    Table tab(2);
    tab.set_frame_style("-", "|");
    tab.set_title("SPheno output");
    tab.set_frame_color(YELLOW);
    tab.add_row(std::vector<std::string>{"mu:", stringAuto(_renormScale) + " GeV"});
    tab.add_row(std::vector<std::string>{"Masses", "[GeV]"}, true);
    tab.add_row(std::vector<std::string>{"m_h:", stringAuto(_mh[0])});
    tab.add_row(std::vector<std::string>{"m_H:", stringAuto(_mh[1])});
    tab.add_row(std::vector<std::string>{"m_A:", stringAuto(_mh[2])});
    tab.add_row(std::vector<std::string>{"m_Hc:", stringAuto(_mh[3])});
    tab.add_row(
        std::vector<std::string>{"LoopLvl:", std::to_string(_massLoopLvl)});
    tab.add_row(std::vector<std::string>{"Oblique", "#"});
    tab.add_row(std::vector<std::string>{"S:", stringAuto(_S)});
    tab.add_row(std::vector<std::string>{"T:", stringAuto(_T)});
    tab.add_row(std::vector<std::string>{"U:", stringAuto(_U)});
    tab.print();
    std::cout << "\n";
  }
  else
    std::cout << "SPheno: \x1B[33munitialized.\x1B[0m\n";
}

}