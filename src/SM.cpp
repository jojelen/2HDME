// SM.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: SM class with RGE functionality
 * @author: Joel Oredsson
 *============================================================================*/
#include "SM.h"
#include "HelpFunctions.h"
#include "Globals.h"
#include "RGE.h"
#include <complex>
#include <string>
#include <vector>

using namespace std;

namespace THDME
{

/**
 *   The standard model is created with the ms-bar parameters at the scale
 * MT = 173.34 GeV.
 *
 *   Fermion masses are taken from arXiv:0712.1419.
 *
 *   The Higgs coupling is taken from arXiv:1307.3536v4. 
 *
 *   Used the tree-lvl relation v ~ 246 GeV.
 *
 *   Gauge couplings are also from arXiv:1307.3536v4. 
 */
SM::SM()
    : _g1(0.35830), _g2(0.64779), _g3(1.1666), _lambda(0.12604),
      _v2(246.21971 * 246.21971)
{
  _modelName = "SM";
  _renormScale = 173.34;

  _rgeConfig.dataOutput = false;
  _rgeConfig.consoleOutput = false;
  _rgeConfig.evolutionName = "SM";
  _rgeConfig.twoloop = true;
  _rgeConfig.perturbativity = true;
  _rgeConfig.stability = false;
  _rgeConfig.unitarity = false;
  _rgeConfig.finalEnergyScale = 500.;
  _rgeConfig.steps = 10;
  _console.set_logLevel(LOG_WARNINGS);

  // The RGE running of the SM is performed with full 2-loop RGEs with 6 quark
  // flavors. 59 is the number of params in y[].
  init_rge_systems(rgeFuncSm_1loop, rgeFuncSm_2loop, 59);

  // Using the MS-bar fermion masses at scale Mt from arXiv:0712.1419.
  static const std::vector<double> mup = {0.00122, 0.590, 162.2};
  static const std::vector<double> mdn = {0.00276, 0.052, 2.75};
  static const std::vector<double> ml = {0.000485289396, 0.1024673155, 1.74215};
  // Pole masses // DEBUG
  // static const std::vector<double> mup = {Global::mu, Global::mc, Global::mt};
  // static const std::vector<double> mdn = {Global::md, Global::ms, Global::mb};
  // static const std::vector<double> ml = {Global::me, Global::mmuon, Global::mtau};

  _VCKM = get_pdg_ckm_matrix();
  calc_yukawa_matrices(mup, mdn, ml, _VCKM);
}

SM::~SM()
{
  // std::cout << "Destroying SM!\n"; // DEBUG
}

void SM::set_params(const double &renormScale, const double &lambda,
                    const double &v2, const std::vector<double> &g,
                    const std::vector<double> &mup,
                    const std::vector<double> &mdn,
                    const std::vector<double> &ml, Eigen::Matrix3cd vCkm)
{
  _renormScale = renormScale;
  _lambda = lambda;
  _v2 = v2;
  _g1 = g[0];
  _g2 = g[1];
  _g3 = g[2];

  calc_yukawa_matrices(mup, mdn, ml, vCkm);
}

void SM::set_higgs(const double &v2, const double &lambda)
{
  _v2 = v2;
  _lambda = lambda;
}

void SM::set_gauge(const double &g1, const double &g2, const double &g3)
{
  _g1 = g1;
  _g2 = g2;
  _g3 = g3;
}

void SM::set_mup(const double &mu, const double &mc, const double &mt)
{
  std::vector<double> mup = {mu, mc, mt};
  calc_yukawa_matrices(mup, get_mdn(), get_ml(), _VCKM);
}
void SM::set_mdn(const double &md, const double &ms, const double &mb)
{
  std::vector<double> mdn = {md, ms, mb};
  calc_yukawa_matrices(get_mup(), mdn, get_ml(), _VCKM);
}
void SM::set_ml(const double &me, const double &mmu, const double &mtau)
{
  std::vector<double> ml = {me, mmu, mtau};
  calc_yukawa_matrices(get_mup(), get_mdn(), ml, _VCKM);
}
void SM::set_ckm(const Eigen::Matrix3cd &vCkm)
{
  calc_yukawa_matrices(get_mup(), get_mdn(), get_ml(), vCkm);
}

bool SM::reset(const double y[], const double &renormScale)
{
  set_model_from_y(y);
  calc_fermionMasses_and_ckm();
  _renormScale = renormScale;
  return true;
}

bool SM::reset_to_saved_state()
{
  _rgeResults.reset();

  if (_ySaved != nullptr)
  {
    reset(_ySaved, _savedRenormScale); 
    return true;
  }
  else
  {
    _console.error << "[ERROR]: No saved state. Cannot reset.\n";
    return false;
  }
}

void SM::print_all() const
{
  std::cout << "\nSM parameters:\n";
  std::cout << "Renormalization scale: " << _renormScale << " GeV:\n";

  double thetaW = atan(_g1 / _g2);
  double cW = cos(thetaW);
  double alpha = (_g1 * cW) * (_g1 * cW) / (4. * M_PI);
  double aS = _g3 * _g3 / (4. * M_PI);
  Table tab1(4);
  tab1.set_frame_style("-", "|");
  tab1.set_frame_color(FrameColor::YELLOW);
  tab1.set_title("Gauge couplings");
  tab1.add_row(vector<string>{"g1:", stringAuto(_g1),
                              "alpha^-1:", stringAuto(1. / alpha)});
  tab1.add_row(vector<string>{"g2:", stringAuto(_g2), "cosW:", stringAuto(cW)});
  tab1.add_row(
      vector<string>{"g3:", stringAuto(_g3), "alpha_S:", stringAuto(aS)});
  tab1.print();

  double v = sqrt(_v2);
  double mh = v * sqrt(2. * _lambda);
  double mu = mh / sqrt(2.);

  Table tab3(3);
  tab3.set_frame_style("-", "|");
  tab3.set_frame_color(FrameColor::GREEN);
  tab3.set_title("Higgs");
  tab3.add_row(vector<string>{"mh:", stringAuto(mh), "GeV"});
  tab3.add_row(vector<string>{"v:", stringAuto(v), "GeV"});
  tab3.add_row(vector<string>{"mu:", stringAuto(mu), "GeV"});
  tab3.add_row(vector<string>{"lambda:", stringAuto(_lambda), ""});
  tab3.print();

  Table tab2(2);
  tab2.set_frame_style("-", "|");
  tab3.set_title("Fermions");
  tab2.add_row(vector<string>{"Masses", "[GeV]"}, true);
  tab2.add_row(vector<string>{"mT:", stringAuto(_mup[2])});
  tab2.add_row(vector<string>{"mC:", stringAuto(_mup[1])});
  tab2.add_row(vector<string>{"mU:", stringAuto(_mup[0])});
  tab2.add_row(vector<string>{"mB:", stringAuto(_mdn[2])});
  tab2.add_row(vector<string>{"mS:", stringAuto(_mdn[1])});
  tab2.add_row(vector<string>{"mD:", stringAuto(_mdn[0])});
  tab2.add_row(vector<string>{"mTau:", stringAuto(_ml[2])});
  tab2.add_row(vector<string>{"mMu:", stringAuto(_ml[1])});
  tab2.add_row(vector<string>{"mE:", stringAuto(_ml[0])});
  tab2.print();

  std::cout << "VCKM:\n"
            << _VCKM << std::endl;
  std::cout << "\n";
}

bool SM::rge_update(const double y[], const double ti)
{
  _rgeResults.ef = exp(ti);
  _renormScale = _rgeResults.ef;
  set_model_from_y(y);
  return true;
}

void SM::rge_finished() { calc_fermionMasses_and_ckm(); }

void SM::calc_fermionMasses_and_ckm()
{
  // Using biunitary transformation from 2HDM case, where there are 6 Yukawa
  // matrices. Same procedure for SM but three of them are zero.
  Eigen::Matrix3cd tempMatrix;
  tempMatrix.setZero();

  BiUnitary(_yU, _yD, _yL, tempMatrix, tempMatrix, tempMatrix, _VCKM, _mup,
            _mdn, _ml, _v2);
  // Set the Yukawa matrices in the flavor basis again.
  _yD = _VCKM * _yD;
}

void SM::set_y(double y[]) const
{
  y[0] = _g1;
  y[1] = _g2;
  y[2] = _g3;
  y[3] = sqrt(_v2);
  y[4] = 2. * _lambda; // Different factor of 2 convention compared to 2HDM

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      y[5 + i * 3 + j] = real(_yU(i, j));
      y[14 + i * 3 + j] = imag(_yU(i, j));
      y[23 + i * 3 + j] = real(_yD(i, j));
      y[32 + i * 3 + j] = imag(_yD(i, j));
      y[41 + i * 3 + j] = real(_yL(i, j));
      y[50 + i * 3 + j] = imag(_yL(i, j));
    }
}

void SM::set_model_from_y(const double y[])
{
  _g1 = y[0];
  _g2 = y[1];
  _g3 = y[2];
  _v2 = y[3] * y[3];
  _lambda = y[4] / 2.; // Different factor of 2 convention compared to 2HDM

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      _yU(i, j) = std::complex<double>(y[5 + i * 3 + j], y[14 + i * 3 + j]);
      _yD(i, j) = std::complex<double>(y[23 + i * 3 + j], y[32 + i * 3 + j]);
      _yL(i, j) = std::complex<double>(y[41 + i * 3 + j], y[50 + i * 3 + j]);
    }
}

bool SM::is_perturbative() const { return true; }

bool SM::is_unitary() const { return true; }

bool SM::is_stable() const
{
  if (_lambda < 0)
    return false;

  return true;
}

void SM::calc_yukawa_matrices(const std::vector<double> &mup,
                              const std::vector<double> &mdn,
                              const std::vector<double> &ml,
                              const Eigen::Matrix3cd &vCkm)
{
  _VCKM = vCkm;
  _yU.setZero();
  _yD.setZero();
  _yL.setZero();

  for (int i = 0; i < 3; i++)
  {
    _mup[i] = mup[i];
    _mdn[i] = mdn[i];
    _ml[i] = ml[i];
    _yU(i, i) = std::complex<double>(mup[i] * sqrt(2. / _v2), 0.);
    _yD(i, i) = std::complex<double>(mdn[i] * sqrt(2. / _v2), 0.);
    _yL(i, i) = std::complex<double>(ml[i] * sqrt(2. / _v2), 0.);
  }

  // Going to the flavor eigenbasis
  _yD = _VCKM * _yD;
}

double SM::get_v2() const { return _v2; }

vector<double> SM::get_gauge_couplings() const
{
  return vector<double>{_g1, _g2, _g3};
}

vector<double> SM::get_mup() const
{
  return vector<double>{_mup[0], _mup[1], _mup[2]};
}

vector<double> SM::get_mdn() const
{
  return vector<double>{_mdn[0], _mdn[1], _mdn[2]};
}

vector<double> SM::get_ml() const
{
  return vector<double>{_ml[0], _ml[1], _ml[2]};
}

Eigen::Matrix3cd SM::get_vCkm() const { return _VCKM; }

double SM::get_lambda() const { return _lambda; }

void SM::write_slha_file(const std::string &file) const
{
  FILE *output;
  output = fopen(file.c_str(), "w");

  if (output == NULL)
  {
    printf("Cannot open file \"%s\" for writing\n", file.c_str());
    return;
  }
  std::string info = "#  Created: " + date() + "\n";
  fprintf(
      output,
      "##################################################################\n");
  fprintf(output, "#\n");
  fprintf(output, "#  SM SLHA file                                 \n");
  fprintf(output, "%s", info.c_str());
  fprintf(output, "#\n");
  fprintf(
      output,
      "##################################################################\n");
  fprintf(output, "Block THDME   # Features at current scale\n");
  fprintf(output, " 0   % 16.8e   # Renormalization scale\n",
          get_renormalization_scale());
  fprintf(output, " 5   %i   # Stable, tree-lvl  (0=false, 1=true)\n",
          is_stable() ? 1 : 0);
  // If the model has undergone RG evolution
  if (_rgeResults.evolved)
  {
    fprintf(output, "Block RGE   # Results of RGE evolution\n");
    fprintf(output, " 0   % 16.8e   # Start scale\n", _rgeResults.e0);
    fprintf(output,
            " 1   % 16.8e   # Finish scale (same as current renorm scale)\n",
            _rgeResults.ef);
    fprintf(
        output,
        " 2   % 16.8e   # Perturbativity breakdown scale (-1 = no violation)\n",
        _rgeResults.ef_pert);
    fprintf(output,
            " 3   % 16.8e   # Unitarity breakdown scale (-1 = no violation)\n",
            _rgeResults.ef_unit);
    fprintf(output,
            " 4   % 16.8e   # Stability breakdown scale (-1 = no violation)\n",
            _rgeResults.ef_stab);
  }

  // Scalar potential
  fprintf(output, "Block MINPAR      # Potential\n");
  fprintf(output, " 1   % 16.8e   # lambda\n", _lambda);
  // Yukawa sector
  fprintf(output, "Block YUIN    # YU Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # YU(%i,%i)\n", i + 1, j + 1,
              real(_yU(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block YDIN    # YD Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # YD(%i,%i)\n", i + 1, j + 1,
              real(_yD(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block YLIN    # YL Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # YL(%i,%i)\n", i + 1, j + 1,
              real(_yL(i, j)), i + 1, j + 1);
    }

  fprintf(output, "Block IMYUIN    # YU Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # im(YU(%i,%i))\n", i + 1, j + 1,
              imag(_yU(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMYDIN    # YD Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # im(YD(%i,%i))\n", i + 1, j + 1,
              imag(_yD(i, j)), i + 1, j + 1);
    }
  fprintf(output, "Block IMYLIN    # YL Yukawa matrix \n");
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      fprintf(output, " %i %i   % 16.8e   # im(YL(%i,%i))\n", i + 1, j + 1,
              imag(_yL(i, j)), i + 1, j + 1);
    }

  fprintf(output, "Block HMIXIN       #  \n");
  fprintf(output, " 102   % 16.8e   # v\n", std::sqrt(_v2));
  fprintf(output, "Block GAUGEIN       #  \n");
  fprintf(output, " 1   % 16.8e   # g1\n", _g1);
  fprintf(output, " 2   % 16.8e   # g2\n", _g2);
  fprintf(output, " 3   % 16.8e   # g3\n", _g3);

  fclose(output);

  _console.info << "SLHA output file, " << file << ", created!\n";
}

bool SM::set_from_slha_file(const std::string &file)
{
  // Loading the output
  std::ifstream modelFileStream(file, std::ios_base::in);

  if (!modelFileStream)
  {
    _console.warning << "[ERROR]: Couldn't open " << file << std::endl;
    return false;
  }

  double y[_numParams];

  std::string line, firstWord, blockType;
  while (std::getline(modelFileStream, line))
  {
    // Retrieving first word
    std::istringstream iss(line);
    if (!(iss >> firstWord))
    {
      break;
    }

    // If it is the start of a block
    if (firstWord == "Block")
    {
      iss >> blockType;
      // Fills the y array with info from that specific block.
      fill_y(y, blockType, modelFileStream);
    }
  }

  set_model_from_y(y);
  calc_fermionMasses_and_ckm();

  return true;
}

void SM::fill_y(double y[], const string &blockType, std::ifstream &ifStream)
{
  if (blockType == "THDME")
  {
    _renormScale = readLine(ifStream, 2);
  }
  if (blockType == "MINPAR")
  {
    y[4] = readLine(ifStream, 2);
  }
  // Real parts of Yukawa matrices
  else if (blockType == "YUIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[5 + i * 3 + j] = readLine(ifStream, 3); // real(yU(i,j))
      }
  }
  else if (blockType == "YDIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[23 + i * 3 + j] = readLine(ifStream, 3); // real(yD(i,j))
      }
  }
  else if (blockType == "YLIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[41 + i * 3 + j] = readLine(ifStream, 3); // real(yL(i,j))
      }
  }
  // Imag parts of Yukawa matrices
  else if (blockType == "IMYUIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[14 + i * 3 + j] = readLine(ifStream, 3); // imag(yU(i,j))
      }
  }
  else if (blockType == "IMYDIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[32 + i * 3 + j] = readLine(ifStream, 3); // imag(yD(i,j))
      }
  }
  else if (blockType == "IMYLIN")
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        y[50 + i * 3 + j] = readLine(ifStream, 3); // imag(yL(i,j))
      }
  }
  else if (blockType == "HMIXIN")
  {
    y[3] = readLine(ifStream, 2); // v
  }
  else if (blockType == "GAUGEIN")
  {
    y[0] = readLine(ifStream, 2); // g1
    y[1] = readLine(ifStream, 2); // g2
    y[2] = readLine(ifStream, 2); // g3
  }
}

} // namespace THDME