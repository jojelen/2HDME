// SPheno.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: SPheno wrapper class
 * @author: Joel Oredsson
 * 
 * SPheno is used to calculate loop corrected scalar masses as well as oblique 
 * parameters S,T,U.
 * 
 * Example usage:
 *   Spheno spheno;
 *   spheno.run( thdm, 2) // 2 refers to the loop level for the masses.
 *   spheno.print()       // Prints SPheno output to console.
 * 
 *============================================================================*/

#pragma once

#include <vector>
#include <mutex>

namespace THDME {

class THDM;

/**
 * @brief: Runs SPheno and stores the relevant output
 */
class Spheno{
  public:
    Spheno();
    ~Spheno();

    /**
     * @brief: Runs SPheno 
     * 
     * First it writes a SLHA text file of the THDM object. 
     * Then it runs SPheno as a system command and suppresses console output.
     * Finally, it reads the SPheno output and appends it to the SLHA file.
     * 
     * To make the function thread safe, it writes temporary SLHA input and 
     * output files in temp/... .
     * 
     * @returns true if successful
     */
    bool run( const THDM &thdm, const int massLoopLvl);

    /**
     * @brief: Prints some of the SPheno results to the console
     * 
     * All the SPheno observables are collected in SPheno.spc.THDM_GEN. This
     * function simply prints out a particular subset of them.
     */
    void print() const;

    /**
     * @brief: Returns the SPheno output
     */
    std::vector<double> get_output() const;

    /**
     * @brief: Checks if the calculated observables agrees with experiment
     * @returns true is they are allowed; false otherwise.
     */
    bool is_allowed() const;

    void set(const std::vector<double> &sphenoOutput);
    void set_mass_loop_lvl(int massLoopLvl);

  private:
    bool read_spheno_output(const std::string &outputFile);
    bool append_results_to_slha(const std::string &slhaFile) const;

  private:
    // Turns true after SPheno calculation
    bool _initialized;

    // Loop lvl to include in Higgs mass corrections
    int _massLoopLvl;

    // Higgs pole masses: mh, mH, mA, mHc
    double _mh[4];
    // Oblique parameters
    double _S, _T, _U;

    // Renormalization scale of calcualtion
    double _renormScale;
};

}