// GnuPlotSystem.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: GNUPLOT class
 * @author: Joel Oredsson
 *
 * Creates GNUPLOT scripts for simple plotting of running parameters of 2HDM.
 * Should be given the directory where there is a "data" folder containing the
 * evolution data files.
 *
 * Example usage:
 *   GnuPlotSystem gnuSys( "testRun");
 *   gnuSys.set_z2_symmetry( TYPE_I); // Needed to NOT plot "zero" matrices.
 *   gnuSys.create_pdf_plots( rgeConfig, rgeResults);
 *
 *============================================================================*/
#pragma once

#include "Structures.h"
#include "THDM_bases.h"

#include <string>

namespace THDME
{
class GnuPlotSystem
{
public:
  GnuPlotSystem(const std::string &outputDirectory);
  ~GnuPlotSystem();

  void set_z2_symmetry(const Z2symmetry &sym);

  void create_pdf_plots(const RgeConfig &rgeConfig,
                        const RgeResults &rgeResults);

private:
  std::string _outputDir;
  Z2symmetry _z2Symmetry;

  // Common options used for many plots
  std::string _loopOrder, _setBorder, _xRange, _lastTic, _lineWidth;

  void export_pdf();

  void create_common_options(const RgeConfig &rgeConfig,
                             const RgeResults &rgeResults);
  void create_lambda_script() const;
  void create_Z_script() const;
  void create_angles_script() const;
  void create_higgs_masses_script() const;
  void create_Mij_script() const;
  void create_vev_script() const;
  void create_yukawa_script() const;

  void create_export_script() const;
};
} // namespace THDME