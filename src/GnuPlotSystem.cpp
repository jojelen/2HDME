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

namespace THDME {

GnuPlotSystem::GnuPlotSystem(const std::string &outputDirectory)
    : _outputDir(outputDirectory) {}
GnuPlotSystem::~GnuPlotSystem() {}

void GnuPlotSystem::export_pdf() {
    // create directory where to store the plots
    create_folder_path(_outputDir + "/plots");
    std::string sysCommand = "gnuplot " + _outputDir + "exportEps.gnu";
    int errorSys = system(sysCommand.c_str());
    if (errorSys == -1) std::cout << "[ERROR]: Couldn't execute GnuPlot.\n";
}

void GnuPlotSystem::set_z2_symmetry(const Z2symmetry &sym) {
    _z2Symmetry = sym;
}

void GnuPlotSystem::create_pdf_plots(const RgeConfig &rgeConfig,
                                     const RgeResults &rgeResults) {
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

    create_export_script();
    export_pdf();
}

void GnuPlotSystem::create_common_options(const RgeConfig &rgeConfig,
                                          const RgeResults &rgeResults) {
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

void GnuPlotSystem::create_lambda_script() const {
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
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:9 w l lw lineWidth  title \"{/Symbol l}_1\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:10 w l lw lineWidth  title \"{/Symbol l}_2\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:11 w l lw lineWidth  title \"{/Symbol l}_3\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:12 w l lw lineWidth  title \"{/Symbol l}_4\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:13 w l lw lineWidth  title \"re{/Symbol l}_5\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:15 w l lw lineWidth title \"re{/Symbol l}_6\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:17 w l lw lineWidth  title \"re{/Symbol l}_7\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_Z_script() const {
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
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:23 w l lw lineWidth  title \"Z_1\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:24 w l lw lineWidth  title \"Z_2\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:25 w l lw lineWidth  title \"Z_3\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:26 w l lw lineWidth  title \"Z_4\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:27 w l lw lineWidth  title \"reZ_5\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:29 w l lw lineWidth title \"reZ_6\",\\\n"
                       "'" +
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:31 w l lw lineWidth  title \"reZ_7\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_angles_script() const {
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
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:4 w l lw lineWidth title \"tan({/Symbol b})\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_Mij_script() const {
    std::ofstream *file =
        new std::ofstream(_outputDir + "/gnuScripts/Mij.gnu", std::ios::out);
    std::string text =
        _setBorder +
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
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:5 w l lw lineWidth title \"M_{11}^2\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:6 w l lw lineWidth title \"M_{22}^2\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:7 w l lw lineWidth title \"reM_{12}^2\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_vev_script() const {
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
                       _outputDir + "data/evolution_" + _loopOrder +
                       ".csv' using 1:22 w l lw lineWidth title \"v\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_higgs_masses_script() const {
    std::ofstream *file = new std::ofstream(
        _outputDir + "/gnuScripts/higgsMasses.gnu", std::ios::out);
    std::string text =
        _setBorder +
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
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:33 w l lw lineWidth title \"m_{h_1}\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:34 w l lw lineWidth title \"m_{h_2}\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:35 w l lw lineWidth title \"m_{h_3}\",\\\n"
        "'" +
        _outputDir + "data/evolution_" + _loopOrder +
        ".csv' using 1:36 w l lw lineWidth title \"m_{Hc}\"";
    *file << text;
    file->close();
    delete file;
}

void GnuPlotSystem::create_export_script() const {
    std::ofstream *file =
        new std::ofstream(_outputDir + "exportEps.gnu", std::ios::out);
    std::string text =
        "set terminal postscript enhanced color eps\n"
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
    text.append("quit");
    *file << text;
    file->close();
    delete file;
}
}  // namespace THDME
