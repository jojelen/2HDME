// HelpFunctions.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Miscellaneous functions
 * @author: Joel Oredsson
 *
 * Collection of helpful functions related to measuring time, diagonalizing
 * matrices, writing tables to the console etc...
 *
 *============================================================================*/
#pragma once

#include <Eigen/Dense>
#include <chrono>
#include <complex>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_rng.h>
#include <stdarg.h>
#include <stdio.h>
#include <thread>

namespace THDME {

/**
 * @brief: Reads number from ifstream
 * 
 * Reads a line from ifStream, 
 * @returns the wordID'th word as a double;
 */
double readLine(std::ifstream &ifStream, unsigned int wordID);

// Checks if a file exists, returns true if it does.
bool file_exists(const std::string &file);

// Returns the folder path of a path to a file.
std::string extract_folder_path(const std::string &path);

bool create_folder_path(const std::string &path);

bool change_directory(const std::string &cdCommand);

//------------------------------------------------------------------------------

/**
 * @brief Sign function
 *
 * @returns 1 (-1) for positive (negative) argument
 */
inline int sign(double x) {
  if (x >= 0)
    return 1;
  if (x < 0)
    return -1;

  return 0;
}

/**
 * @brief: Simple progress bar for console
 */
void print_prog_bar(const int percent);

/**
 * @brief: Simple progress number for console
 */
void print_prog_number(const std::string message, const double num);



//----Time-functions------------------------------------------------------------

/**
 * @brief Prints out the time the program has been running
 *
 * CPU time is the time the CPU has been occupied with the program.
 * CPU time can exceed the real time when using multiple threads.
 *
 * Also prints out the real time since it was called the first time.
 */
void programTime(bool exact = false);

/**
 * @brief Timer
 *
 * Prints the time since construction at time of destruction.
 * */
struct Timer {
  Timer();
  Timer(const std::string &message); 
  ~Timer();

  std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;
  std::chrono::duration<float> _duration;

  std::string _message;

  void printDuration() const;
};

/** @brief Gives the date at runtime.
 *
 * @returns date in the form %Y-%m-%d
 */
std::string date();

//------------------------------------------------------------------------------

// Returns random Matrix3cd, with abs(elements) < 1
Eigen::Matrix3cd randomMatrix3cd(gsl_rng *rng);

// Converts double to a string with scientific notation for big numbers
std::string stringAuto(const double &number);
std::string stringAuto(const std::complex<double> &cNumber);

int jac(double t, const double y[], double *dfdy, double dfdt[], void *params);

/**
 * @brief Biunitary transformation of the Yukawa matrices in the Higgs basis.
 * This diagonalizes the kF matrices. It also calculates the CKM matrix,
 * transforms the rF matrices. Also calculates fermion masses.
 */
void BiUnitary(Eigen::Matrix3cd &kU, Eigen::Matrix3cd &kD, Eigen::Matrix3cd &kL,
               Eigen::Matrix3cd &rU, Eigen::Matrix3cd &rD, Eigen::Matrix3cd &rL,
               Eigen::Matrix3cd &VCKM, double mup[], double mdn[], double ml[],
               double &v2);

//------------------------------------------------------------------------------

/**
 * @brief: Console table
 *
 * A Class that can write a table of strings or doubles to the console.
 * It's by default set to have a border "|" in red color and no title.
 */

enum FrameColor { RED, BLUE, GREEN, YELLOW };

class Table {
private:
  std::string _borderChar, _lineChar; // Characters for frame borders.
  FrameColor _frameColor;

  unsigned int _columns, _rows;

  static bool _colorMode;

  // Vector containing the size of each column (decided by largest content).
  std::vector<unsigned int> _columnSize;

  // True if the row should be boldfaced with underline.
  std::vector<bool> _boldAndunderLine;

  // Holds the cells of the table, _content[column, row].
  std::vector<std::vector<std::string>> _content;

  bool _titleBool; // Sets to true if one sets a title.
  std::string _titleString;

  // Draws text in _frameColor
  void drawColor(std::string text) const;

  // Draws a line in _frameColor
  void drawLine(int length, const std::string &lineChar) const;

  // Draws a centered boldface underlined title on top of the table
  void drawTitle(int length) const;

public:
  Table(const int columns);
  ~Table();

  void set_frame_color(const FrameColor &color);
  void set_frame_style(const std::string &lineChar,
                       const std::string &borderChar);
  void set_title(const std::string &title);

  void add_row(const std::vector<std::string> &stringVec,
               bool boldAndunderLine = false);
  void add_row(const std::vector<double> &doubleVec,
               bool boldAndunderLine = false);

  void print() const;

  // All color can be dsiabled (for printing to text files).
  static void change_colorMode(const bool mode);
};

}