// HelpFunctions.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Miscellaneous functions
 * @author: Joel Oredsson
 *============================================================================*/
#include "HelpFunctions.h"

#include "unistd.h"
#include <Eigen/Dense>
#include <complex>
#include <fstream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_rng.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <time.h>

namespace THDME
{

double readLine(std::ifstream &ifStream, unsigned int wordID)
{
  std::string line, temp;
  std::getline(ifStream, line);
  std::istringstream iss(line);

  for (unsigned int i = 1; i < wordID; ++i)
  {
    iss >> temp;
  }
  double number;
  iss >> number;
  return number;
}

bool file_exists(const std::string &file)
{
  struct stat buf;
  return (stat(file.c_str(), &buf) == 0);
}

std::string extract_folder_path(const std::string &path)
{
  int slashPosition = 0;
  std::string newFolder;
  std::string find = "/";
  for (int i = path.length() - 1; i > -1; i--)
  {
    if (path.at(i) == find.at(0))
    {
      slashPosition = i;
      break;
    }
  }
  newFolder.assign(path, 0, slashPosition);
  return newFolder;
}

bool create_folder_path(const std::string &path)
{
  std::string newFolder = "mkdir -p " + path;

  char *cstr = new char[newFolder.length() + 1];
  std::strcpy(cstr, newFolder.c_str());
  const int dir_err = system(cstr);
  if (-1 == dir_err)
  {
    std::cout << "Error creating directory!\n";
    return false;
  }
  return true;
}

bool change_directory(const std::string &cdCommand)
{
  if (chdir(cdCommand.c_str()) == -1)
    return false;
  return true;
}

//------------------------------------------------------------------------------

Eigen::Matrix3cd randomMatrix3cd(gsl_rng *rng)
{
  Eigen::Matrix3cd randomMatrix;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      randomMatrix(i, j) = gsl_rng_uniform(rng) *
                           std::polar(1., gsl_rng_uniform(rng) * 2. * M_PI);
    }
  }
  return randomMatrix;
}

//------------------------------------------------------------------------------

void print_prog_bar(int percent)
{
  std::string bar;
  static int barLength = 10;
  static int barPercent = 100 / barLength;

  for (int i = 0; i < barLength; i++)
  {
    if (i < (percent / barPercent))
    {
      bar.replace(i, 1, "=");
    }
    else if (i == (percent / barPercent))
    {
      bar.replace(i, 1, ">");
    }
    else
    {
      bar.replace(i, 1, " ");
    }
  }

  std::cout << "\r"
               "["
            << bar << "] ";
  std::cout.width(3);
  std::cout << percent << "%";
}

void print_prog_number(const std::string message, const double num)
{
  std::cout.width(20);
  std::cout << std::flush;

  std::cout << "\r" << message << ": ";
  std::cout.width(12);
  std::cout << num << std::flush;
}

//------------------------------------------------------------------------------

// Converts double to a string with scientific notation for big numbers
std::string stringAuto(const double &number)
{
  // Create an output string stream
  std::ostringstream streamObj;
  // Add double to stream
  streamObj << number;
  // Get string from output string stream
  return streamObj.str();
}

std::string stringAuto(const std::complex<double> &cNumber)
{
  // Create an output string stream
  std::ostringstream streamObj;
  // Add double to stream
  streamObj << cNumber;
  // Get string from output string stream
  return streamObj.str();
}

//------------------------------------------------------------------------------

void programTime(bool exact)
{
  // seconds of CPU time the program has occupied.
  // the actual time is sec/(number of threads).
  float sec = ((float)clock()) / CLOCKS_PER_SEC;
  static time_t timeZero = time(NULL);

  // Conversions
  int min = (int)floor(sec / 60);
  int hours = (int)floor(min / 60);
  min %= 60;
  sec = sec - 60 * 60 * hours - 60 * min;
  if (exact)
    printf(" CPU time = %i h %i m %f s.\n", hours, min, sec);
  else
    printf(" CPU time = %i h %i m %.f s.\n", hours, min, sec);

  float seconds = difftime(time(NULL), timeZero);
  min = (int)floor(seconds / 60);
  hours = (int)floor(min / 60);
  min %= 60;
  seconds = seconds - 60 * 60 * hours - 60 * min;
  printf("Real time = %i h %i m %.f s.\n", hours, min, seconds);
}

Timer::Timer() : _message("Timer:")
{
  _start = std::chrono::high_resolution_clock::now();
}

Timer::Timer(const std::string &message) : _message(message)
{
  _start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
  _end = std::chrono::high_resolution_clock::now();
  _duration = _end - _start;
  printDuration();
}

void Timer::printDuration() const
{
  float seconds = _duration.count();
  int min = (int)floor(seconds / 60);
  int hours = (int)floor(min / 60);
  min %= 60;
  seconds = seconds - 60 * 60 * hours - 60 * min;
  printf(_message.c_str());
  printf(" %i h %i m %.3f s.\n", hours, min, seconds);
}

std::string date()
{
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  char buf[100] = {0};
  std::strftime(buf, sizeof(buf), "%d %h %Y at %H:%M", std::localtime(&now));
  std::string dateString(buf);

  return dateString;
}

//------------------------------------------------------------------------------

int jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
  return GSL_SUCCESS;
}

//------------------------------------------------------------------------------

void BiUnitary(Eigen::Matrix3cd &kU, Eigen::Matrix3cd &kD, Eigen::Matrix3cd &kL,
               Eigen::Matrix3cd &rU, Eigen::Matrix3cd &rD, Eigen::Matrix3cd &rL,
               Eigen::Matrix3cd &VCKM, double mup[], double mdn[], double ml[],
               double &v2)
{
  using namespace std;
  using namespace Eigen;

  // Temporary matrices of kappa and rho matrices. These should be the
  // potentially non-diagonal matrices in the fermion flavor eigenbasis.
  Matrix3cd kU_temp = kU;
  Matrix3cd kD_temp = kD;
  Matrix3cd kL_temp = kL;
  Matrix3cd rU_temp = rU;
  Matrix3cd rD_temp = rD;
  Matrix3cd rL_temp = rL;

  // Find biunitary transformations

  // Leptons
  ComplexEigenSolver<Matrix3cd> kLHkL;
  kL.setZero();
  Matrix3cd kL_inverse;
  kL_inverse.setZero();
  kLHkL.compute(kL_temp.adjoint() * kL_temp);
  for (unsigned int i = 0; i < 3; i++)
  {
    kL(i, i) = sqrt(real(kLHkL.eigenvalues()[i]));
    kL_inverse(i, i) = 1. / kL(i, i);
    ml[i] = real(kL(i, i)) * sqrt(v2 / 2.);
  }
  ComplexEigenSolver<Matrix3cd> kLkLH;
  kLkLH.compute(kL_temp * kL_temp.adjoint());
  Matrix3cd VL_L = kLkLH.eigenvectors().adjoint();
  Matrix3cd VL_R = kL_inverse * VL_L * kL_temp;
  rL = VL_L * rL_temp * VL_R.adjoint();

  // Up-Quarks
  ComplexEigenSolver<Matrix3cd> kUHkU;
  kU.setZero();
  Matrix3cd kU_inverse;
  kU_inverse.setZero();
  kUHkU.compute(kU_temp.adjoint() * kU_temp);
  for (unsigned int i = 0; i < 3; i++)
  {
    kU(i, i) = sqrt(real(kUHkU.eigenvalues()[i]));
    kU_inverse(i, i) = 1. / kU(i, i);
    mup[i] = real(kU(i, i)) * sqrt(v2 / 2.);
  }
  ComplexEigenSolver<Matrix3cd> kUkUH;
  kUkUH.compute(kU_temp * kU_temp.adjoint());
  Matrix3cd VU_L = kUkUH.eigenvectors().adjoint();
  Matrix3cd VU_R = kU_inverse * VU_L * kU_temp;

  // Down-Quarks
  ComplexEigenSolver<Matrix3cd> kDHkD;
  kD.setZero();
  Matrix3cd kD_inverse;
  kD_inverse.setZero();
  kDHkD.compute(kD_temp.adjoint() * kD_temp);
  for (unsigned int i = 0; i < 3; i++)
  {
    kD(i, i) = sqrt(real(kDHkD.eigenvalues()[i]));
    kD_inverse(i, i) = 1. / kD(i, i);
    mdn[i] = real(kD(i, i)) * sqrt(v2 / 2.);
  }
  ComplexEigenSolver<Matrix3cd> kDkDH;
  kDkDH.compute(kD_temp * kD_temp.adjoint());
  Matrix3cd VD_L = kDkDH.eigenvectors().adjoint();
  Matrix3cd VD_R = kD_inverse * VD_L * kD_temp;

  // Calculate new temporary CKM matrix
  Matrix3cd VCKM_temp = VU_L * VD_L.adjoint();

  /*  We will rephase the quarks to get the CKM matrix to
    the PDG phase convention. This is done with two diagonal
    unitary matrices that transforms the left and right
    handed quarks.
  */
  Matrix3cd P_U, P_D;
  P_U.setZero();
  P_D.setZero();

  // std::cout << "VCKM_temp:\n" << VCKM_temp << std::endl; // DEBUG

  // We need to derive the phases of these matrices, P_U, P_D.
  double s13 = std::abs(VCKM_temp(0, 2));
  double c13 = sqrt(1. - s13 * s13);
  double s23 = std::abs(VCKM_temp(1, 2)) / c13;
  double c23 = sqrt(1. - s23 * s23);
  double s12 = std::abs(VCKM_temp(0, 1)) / c13;
  double c12 = sqrt(1. - s12 * s12);
  double cosdelta = (s12 * s12 * s23 * s23 + c12 * c12 * c23 * c23 * s13 * s13 -
                     std::abs(VCKM_temp(2, 0)) * std::abs(VCKM_temp(2, 0))) /
                    (2. * s12 * s23 * c12 * c23 * s13);
  double sindelta = sqrt(abs(1. - cosdelta * cosdelta));
  complex<double> eIdelta = complex<double>(cosdelta, sindelta);

  P_D(0, 0) = VCKM_temp(0, 0) / std::abs(VCKM_temp(0, 0));
  P_D(1, 1) = VCKM_temp(0, 1) / std::abs(VCKM_temp(0, 1));
  P_D(2, 2) = eIdelta * VCKM_temp(0, 2) / std::abs(VCKM_temp(0, 2));

  P_U(0, 0) = complex<double>(1., 0.);
  P_U(1, 1) = conj(VCKM_temp(1, 2)) * P_D(2, 2) / std::abs(VCKM_temp(1, 2));
  P_U(2, 2) = conj(VCKM_temp(2, 2)) * P_D(2, 2) / std::abs(VCKM_temp(2, 2));

  // The CKM matrix transforms during quark rephasing as
  VCKM = P_U * VCKM_temp * P_D.adjoint();

  // The rho matrices are also transformed.
  rU = P_U * VU_L * rU_temp * VU_R.adjoint() * P_U.adjoint();
  rD = P_D * VD_L * rD_temp * VD_R.adjoint() * P_D.adjoint();
}

//--Table-class-----------------------------------------------------------------

bool Table::_colorMode = true;

Table::Table(const int columns)
    : _borderChar("||"), _lineChar("="), _frameColor(RED), _columns(columns),
      _rows(0), _titleBool(false), _titleString("")
{
  _columnSize.resize(columns, 0);
  _content.resize(columns);
}

Table::~Table() {}

void Table::add_row(const std::vector<std::string> &stringVec,
                    bool boldAndunderLine)
{
  if (stringVec.size() > _columns)
  {
    std::cout << "[ERROR]: Couldn't add row to table.\n";
    return;
  }

  if (boldAndunderLine)
    _boldAndunderLine.push_back(true);
  else
    _boldAndunderLine.push_back(false);

  if (stringVec.size() != _columns)
    std::cout
        << "[WARNING]: number of strings does not match number of columns!\n";

  for (unsigned int i = 0; i < stringVec.size(); i++)
  {
    _content[i].push_back(stringVec[i]);

    // Updates the column size if it should be increased.
    if (stringVec[i].length() > _columnSize[i])
      _columnSize[i] = stringVec[i].length();
  }

  ++_rows;
}

void Table::add_row(const std::vector<double> &doubleVec,
                    bool boldAndunderLine)
{
  if (doubleVec.size() > _columns)
  {
    std::cout << "[ERROR]: Couldn't add row to table.\n";
    return;
  }

  if (boldAndunderLine)
    _boldAndunderLine.push_back(true);
  else
    _boldAndunderLine.push_back(false);

  if (doubleVec.size() != _columns)
    std::cout
        << "[WARNING]: number of doubles does not match number of columns!\n";

  for (unsigned int i = 0; i < doubleVec.size(); i++)
  {
    _content[i].push_back(stringAuto(doubleVec[i]));

    // Updates the column size if it should be increased.
    if (_content[i].back().length() > _columnSize[i])
      _columnSize[i] = _content[i].back().length();
  }
  _rows++;
}

// Print the Table
void Table::print() const
{
  std::string temp;
  int spaces = 0;
  int tableLength = 2 * _borderChar.length() + 1 + _columns;

  // Calculate table length
  for (unsigned int i = 0; i < _columns; i++)
  {
    tableLength += _columnSize[i];
  }

  if (_titleBool)
    drawTitle(tableLength);

  drawLine(tableLength, _lineChar);

  // Draw all rows
  for (unsigned int i = 0; i < _rows; i++)
  {
    drawColor(_borderChar + " ");

    for (unsigned int j = 0; j < _columns; j++)
    {
      /* First: calculate the number of spaces in the cell.
        The content is centered, so half spaces is printed first
        and half after the content. */
      spaces = 1 + _columnSize[j] - _content[j][i].length();
      for (int k = 0; k < spaces / 2; k++)
      {
        std::cout << " ";
      }

      // Incase the row should be boldfaced and with an underline
      if (_boldAndunderLine[i] && _colorMode)
      {
        temp = "\x1B[4m\x1B[1m" + _content[j][i] + "\x1B[0m";
        std::cout << temp;
      }
      else // print in normal format
        std::cout << _content[j][i];

      for (int k = 0; k < spaces / 2; k++)
      {
        std::cout << " ";
      } // Spaces after content

      // An extra space for uneven lengths
      if (spaces % 2 != 0)
        std::cout << " ";
    }

    drawColor(_borderChar + "\n");
  }

  drawLine(tableLength, _lineChar);
}

void Table::set_frame_color(const FrameColor &color) { _frameColor = color; }

void Table::set_frame_style(const std::string &line,
                            const std::string &border)
{
  _lineChar = line;
  _borderChar = border;
}

void Table::set_title(const std::string &title)
{
  _titleBool = true;
  _titleString = title;
}

void Table::drawTitle(int length) const
{
  int spaces = 1 + length - _titleString.length(); // Number of spaces in title
  for (int k = 0; k < spaces / 2; k++)
  {
    std::cout << " ";
  } // Spaces before title
  std::string temp = _titleString;

  if (_colorMode)
  {
    temp.insert(0, "\x1B[4m\x1B[1m");
    temp.append("\x1B[0m");
  }
  std::cout << temp;

  for (int k = 0; k < spaces / 2; k++)
  {
    std::cout << " ";
  } // Spaces after title

  if (spaces % 2 != 0)
    std::cout << " "; // An extra space for uneven lengths

  std::cout << "\n";
}
void Table::drawColor(std::string text) const
{
  if (_colorMode)
  {
    switch (_frameColor)
    {
    case RED:
      text.insert(0, "\x1B[31m");
      break;
    case BLUE:
      text.insert(0, "\x1B[34m");
      break;
    case GREEN:
      text.insert(0, "\x1B[32m");
      break;
    case YELLOW:
      text.insert(0, "\x1B[33m");
      break;
    default:
      break;
    }
    text.append("\x1B[0m");
  }
  std::cout << text;
}

void Table::change_colorMode(const bool mode) { Table::_colorMode = mode; }

void Table::drawLine(int length, const std::string &lineChar) const
{
  for (int i = 0; i < length; i++)
  {
    drawColor(lineChar);
  }
  std::cout << std::endl;
}

//------------------------------------------------------------------------------
} // namespace THDME