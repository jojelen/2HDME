// BaseModel.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/*==============================================================================
 * @brief: Base class for models
 * @author: Joel Oredsson
 *
 * This is an abstract class, where many features should be implemented in
 * derived classes.
 * All virtual functions must be overridden in derived classes.
 *
 * BaseModel only cover general features that are useful for derived models;
 * such as printing to consoles, resource managing related to I/O to files etc.
 *
 * Example of derived classes:
 *  BaseModel <- RgeModel <- THDM
 *
 *============================================================================*/
#pragma once

#include "Structures.h"
#include "LoggingSystem.h"
#include "FileSystem.h"

#include <gsl/gsl_odeiv2.h>
#include <string>
#include <atomic>

namespace THDME {



class FileSystem;

class BaseModel
{
public:
  BaseModel(); // Initializes the home directory of FileSystem to "output/"

  virtual ~BaseModel();

  virtual void print_all() const = 0; // Prints model details to console.

  // logLevel determines what level of information is printed to the console.
  // Possible choices: LOG_INFO, LOG_ERRORS, LOG_WARNINGS, LOG_DEBUG.
  inline void set_logLevel(LogLevel lvl) { _console.set_logLevel(lvl); }

  inline std::string get_name() const { return _modelName; }

  inline LogLevel get_logLevel() const { return _console.get_logLevel(); }

protected:
  std::string _modelName;

  Logger _console;
  FileSystem _files;

private:
  void print_2hdme_info(); // Prints logo to console

  static std::atomic<bool> _first_run; // turns false after first class object is created.
  const static std::string _version; // version number of software.
};
}