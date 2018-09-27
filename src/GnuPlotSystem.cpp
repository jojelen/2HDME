// GnuPlotSystem.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: GNUPLOT class
 * @author: Joel Oredsson
 *============================================================================*/
#include "GnuPlotSystem.h"
#include "HelpFunctions.h"

#include <fstream>

namespace THDME
{

GnuPlotSystem::GnuPlotSystem(const std::string &outputDirectory)
    : _outputDir(outputDirectory) {}
GnuPlotSystem::~GnuPlotSystem() {}

void GnuPlotSystem::export_pdf()
{
  // create directory where to store the plots
  create_folder_path(_outputDir + "/plots");
  std::string sysCommand = "gnuplot " + _outputDir + "exportEps.gnu";
  int errorSys = system(sysCommand.c_str());
  if (errorSys == -1)
    std::cout << "[ERROR]: Couldn't execute GnuPlot.\n";
}

void GnuPlotSystem::set_z2_symmetry(const Z2symmetry &sym)
{
  _z2Symmetry = sym;
}

void GnuPlotSystem::create_pdf_plots(const RgeConfig &rgeConfig,
                                     const RgeResults &rgeResults)
{
  create_folder_path(_outputDir);
  create_folder_path(_outputDir + "/gnuScripts");

  // GNU scripts:
  create_common_options(rgeConfig, rgeResults);
  create_lambda_script();
  create_Z_script();
  create_angles_script();
  create_Mij_script();
  create_vev_script();
  create_higgs_masses_script();
  create_yukawa_script();

  create_export_script();
  export_pdf();
}

void GnuPlotSystem::create_common_options(const RgeConfig &rgeConfig,
                                          const RgeResults &rgeResults)
{
  // Some commands that are applicable to many plots
  _loopOrder = rgeConfig.twoloop ? "twoloop" : "oneloop";
  _setBorder = "set border 31 lw 3\n";
  _xRange = "set xrange [" + stringAuto(rgeResults.e0) + ":" +
            stringAuto(rgeResults.ef) + "]\n";
  _lastTic = rgeResults.ef < 1000
                 ? "set xtics add (\"" + std::to_string((int)rgeResults.ef) +
                       "\" " + std::to_string(rgeResults.ef) + ")\n"
                 : "";
  _lineWidth = "lineWidth = 4\n";
}

void GnuPlotSystem::create_lambda_script() const
{
  std::ofstream *file =
      new std::ofstream(_outputDir + "/gnuScripts/lambda.gnu", std::ios::out);
  std::string text =
      _setBorder +
      "set size ratio 1\n"
      "set xlabel 'Energy [GeV]'\n"
      "set ylabel ''\n"
      "unset logscale\n"
      "set logscale x\n"
      // "set yrange [-1:15]\n"
      "set autoscale\n" +
      _xRange + _lastTic + _lineWidth +
      "set key left top\n"
      "plot   '" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:2 w l lw lineWidth  title \"{/Symbol l}_1\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:3 w l lw lineWidth  title \"{/Symbol l}_2\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:4 w l lw lineWidth  title \"{/Symbol l}_3\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:5 w l lw lineWidth  title \"{/Symbol l}_4\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:6 w l lw lineWidth  title \"{/Symbol l}_5\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:8 w l lw lineWidth title \"{/Symbol l}_6\",\\\n"
      "'" +
      _outputDir + "data/lambda_" + _loopOrder + "_generic" +
      ".dat' using 1:10 w l lw lineWidth  title \"{/Symbol l}_7\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_Z_script() const
{
  std::ofstream *file =
      new std::ofstream(_outputDir + "/gnuScripts/Z.gnu", std::ios::out);
  std::string text = _setBorder +
                     "set size ratio 1\n"
                     "set xlabel 'Energy [GeV]'\n"
                     "set ylabel ''\n"
                     "unset logscale\n"
                     "set logscale x\n"
                     // "set yrange [-1:15]\n"
                     "set autoscale\n" +
                     _xRange + _lastTic + _lineWidth +
                     "set key left top\n"
                     "plot   '" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:2 w l lw lineWidth  title \"Z_1\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:3 w l lw lineWidth  title \"Z_2\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:4 w l lw lineWidth  title \"Z_3\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:5 w l lw lineWidth  title \"Z_4\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:6 w l lw lineWidth  title \"Z_5\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:8 w l lw lineWidth title \"Z_6\",\\\n"
                     "'" +
                     _outputDir + "data/Z_" + _loopOrder + "_higgs" +
                     ".dat' using 1:10 w l lw lineWidth  title \"Z_7\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_angles_script() const
{
  std::ofstream *file =
      new std::ofstream(_outputDir + "/gnuScripts/angles.gnu", std::ios::out);
  std::string text =
      _setBorder +
      "set size ratio 1\n"
      "set xlabel 'Energy [GeV]'\n"
      "set ylabel ''\n"
      "unset logscale\n"
      "set logscale x\n"
      "set autoscale y\n" +
      _xRange + _lastTic + _lineWidth +
      "set key right bottom\n"
      "plot   '" +
      _outputDir + "data/VevAndMixing_" + _loopOrder +
      ".dat' using 1:4 w l lw lineWidth title \"tan({/Symbol b})\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_Mij_script() const
{
  std::ofstream *file =
      new std::ofstream(_outputDir + "/gnuScripts/Mij.gnu", std::ios::out);
  std::string text = _setBorder +
                     "set size ratio 1\n"
                     "set xlabel 'Energy [GeV]'\n"
                     "set ylabel '[GeV^2]'\n"
                     "unset logscale\n"
                     "set logscale x\n"
                     "set autoscale y\n"
                     "set logscale y\n" +
                     _xRange + _lastTic + _lineWidth +
                     "set key left top\n"
                     "plot   '" +
                     _outputDir + "data/Mij_" + _loopOrder + "_generic" +
                     ".dat' using 1:2 w l lw lineWidth title \"M_{11}^2\",\\\n"
                     "'" +
                     _outputDir + "data/Mij_" + _loopOrder + "_generic" +
                     ".dat' using 1:3 w l lw lineWidth title \"M_{22}^2\",\\\n"
                     "'" +
                     _outputDir + "data/Mij_" + _loopOrder + "_generic" +
                     ".dat' using 1:4 w l lw lineWidth title \"M_{12}^2\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_vev_script() const
{
  std::ofstream *file =
      new std::ofstream(_outputDir + "/gnuScripts/vev.gnu", std::ios::out);
  std::string text = _setBorder +
                     "set size ratio 1\n"
                     "set xlabel 'Energy [GeV]'\n"
                     "set ylabel '[GeV]'\n"
                     "unset logscale\n"
                     "set logscale x\n"
                     "set autoscale y\n"
                     "set logscale y\n" +
                     _xRange + _lastTic + _lineWidth +
                     "set key left top\n"
                     "plot   '" +
                     _outputDir + "data/VevAndMixing_" + _loopOrder +
                     ".dat' using 1:2 w l lw lineWidth title \"v\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_higgs_masses_script() const
{
  std::ofstream *file = new std::ofstream(
      _outputDir + "/gnuScripts/higgsMasses.gnu", std::ios::out);
  std::string text = _setBorder +
                     "set size ratio 1\n"
                     "set xlabel 'Energy [GeV]'\n"
                     "set ylabel '[GeV]'\n"
                     "unset logscale\n"
                     "set logscale x\n"
                     //  "set logscale y\n"
                     "set autoscale y\n" +
                     _xRange + _lastTic + _lineWidth +
                     "set key left top\n"
                     "plot   '" +
                     _outputDir + "data/HiggsMasses_" + _loopOrder +
                     ".dat' using 1:2 w l lw lineWidth title \"m_{h_1}\",\\\n"
                     "'" +
                     _outputDir + "data/HiggsMasses_" + _loopOrder +
                     ".dat' using 1:3 w l lw lineWidth title \"m_{h_2}\",\\\n"
                     "'" +
                     _outputDir + "data/HiggsMasses_" + _loopOrder +
                     ".dat' using 1:4 w l lw lineWidth title \"m_{h_3}\",\\\n"
                     "'" +
                     _outputDir + "data/HiggsMasses_" + _loopOrder +
                     ".dat' using 1:5 w l lw lineWidth title \"m_{H^\pm}\"";
  *file << text;
  file->close();
  delete file;
}

void GnuPlotSystem::create_yukawa_script() const
{
  std::vector<std::string> matrixNames = {"eta1U", "eta1D", "eta1L", "eta2U",
                                          "eta2D", "eta2L", "kU", "kD",
                                          "kL", "rU", "rD", "rL"};
  std::vector<std::string> matrixNamesTitle = {
      "{{/Symbol h}_1^U}", "{{/Symbol h}_1^D}", "{{/Symbol h}_1^L}",
      "{{/Symbol h}_2^U}", "{{/Symbol h}_2^D}", "{{/Symbol h}_2^L}",
      "{{/Symbol k}^U}", "{{/Symbol k}^D}", "{{/Symbol k}^L}",
      "{{/Symbol r}^U}", "{{/Symbol r}^D}", "{{/Symbol r}^L}"};
  for (unsigned int i = 0; i < matrixNames.size(); i++)
  {
    std::ofstream *file = new std::ofstream(
        _outputDir + "/gnuScripts/" + matrixNames[i] + ".gnu", std::ios::out);
    std::string text =
        _setBorder +
        "set size ratio 1\n"
        "set xlabel 'Energy [GeV]'\n"
        "set ylabel ''\n"
        "set logscale x\n"
        "set logscale y\n"
        // "set yrange [-1:15]\n"
        "set autoscale\n" +
        _xRange + _lastTic + _lineWidth +
        "set key outside right\n"
        "set key spacing 4\n"
        "plot   '" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:2 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{11}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:3 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{12}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:4 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{13}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:5 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{21}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:6 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{22}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:7 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{23}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:8 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{31}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:9 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{32}|\",\\\n"
        "'" +
        _outputDir + "data/" + matrixNames[i] + "_" + _loopOrder +
        ".dat' using 1:10 w l lw lineWidth  title \"|" + matrixNamesTitle[i] +
        "_{33}|\"";
    *file << text;
    file->close();
    delete file;
  }
}

void GnuPlotSystem::create_export_script() const
{
  std::vector<std::string> matrixNames = {"kU", "kD", "kL", "rU", "rD", "rL"};

  // In case of a Yukawa Z2 symmetry, some of the etaF are zero, i.e. no need
  // to plot them (otherwise gnuplot will complain).
  switch (_z2Symmetry)
  {
  case NO_SYMMETRY:
    matrixNames.emplace_back("eta1U");
    matrixNames.emplace_back("eta1D");
    matrixNames.emplace_back("eta1L");
    matrixNames.emplace_back("eta2U");
    matrixNames.emplace_back("eta2D");
    matrixNames.emplace_back("eta2L");
    break;
  case TYPE_I:
    matrixNames.emplace_back("eta2U");
    matrixNames.emplace_back("eta2D");
    matrixNames.emplace_back("eta2L");
    break;
  case TYPE_II:
    matrixNames.emplace_back("eta2U");
    matrixNames.emplace_back("eta1D");
    matrixNames.emplace_back("eta1L");
    break;
  case TYPE_III:
    matrixNames.emplace_back("eta2U");
    matrixNames.emplace_back("eta1D");
    matrixNames.emplace_back("eta2L");
    break;
  case TYPE_IV:
    matrixNames.emplace_back("eta2U");
    matrixNames.emplace_back("eta2D");
    matrixNames.emplace_back("eta1L");
    break;
  default:
    break;
  }

  std::ofstream *file =
      new std::ofstream(_outputDir + "exportEps.gnu", std::ios::out);
  std::string text = "set terminal postscript enhanced color eps\n"
                     "set output '" +
                     _outputDir + "plots/lambdas_" + _loopOrder + "_generic" +
                     ".ps' \n"
                     "load   '" +
                     _outputDir +
                     "/gnuScripts/lambda.gnu' \n"
                     "set output '" +
                     _outputDir + "plots/Z_" + _loopOrder + "_higgs" +
                     ".ps' \n"
                     "load   '" +
                     _outputDir +
                     "/gnuScripts/Z.gnu' \n"
                     "set output '" +
                     _outputDir + "plots/vev_" + _loopOrder +
                     ".ps' \n"
                     "load   '" +
                     _outputDir +
                     "/gnuScripts/vev.gnu' \n"
                     "set output '" +
                     _outputDir + "plots/angles_" + _loopOrder +
                     ".ps' \n"
                     "load   '" +
                     _outputDir +
                     "/gnuScripts/angles.gnu' \n"
                     "set output '" +
                     _outputDir + "plots/Mij_" + _loopOrder +
                     ".ps' \n"
                     "load   '" +
                     _outputDir +
                     "/gnuScripts/Mij.gnu' \n"
                     "set output '" +
                     _outputDir + "plots/higgsMasses_" + _loopOrder +
                     ".ps' \n"
                     "load   '" +
                     _outputDir + "/gnuScripts/higgsMasses.gnu' \n";
  for (unsigned int i = 0; i < matrixNames.size(); i++)
  {
    text.append("set output '" + _outputDir + "plots/" + matrixNames[i] + "_" +
                _loopOrder + ".ps' \n");
    text.append("load   '" + _outputDir + "/gnuScripts/" + matrixNames[i] +
                ".gnu' \n");
  }
  text.append("quit");
  *file << text;
  file->close();
  delete file;
}
} // namespace THDME