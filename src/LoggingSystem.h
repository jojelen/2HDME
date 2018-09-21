// LoggingSystem.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Log class for printing to console
 * @author: Joel Oredsson
 *
 * Is used for choosing what to print to the console. There are five levels set
 * by LogLevel.
 *
 * Example usage:
 *   Logger console;
 *   console.warning << "[WARNING]!\n";
 *   console.info << "Hello world!\n";
 *   console.error << "[ERROR]!\n";
 *
 * The default LogLevel for a Logger object is LOG_INFO.
 *
 *============================================================================*/
#pragma once

#include <iostream>

namespace THDME {

//  The different log levels
enum LogLevel
{
  LOG_NONE = -1,
  LOG_ERRORS = 0,
  LOG_WARNINGS = 1,
  LOG_INFO = 2,
  LOG_DEBUG = 3
};

// This class containts a garbage ostream, which is used when suppressing
// output.
class StreamClass
{
public:
  StreamClass() : _lvl(LOG_INFO) {}
  void set_logLevel(const LogLevel &lvl);

protected:
  static std::ostream *_garbage;
  LogLevel _lvl;
};

/* Information, Warning, Error and Debug are overloading the << operator so
that one can print with << if the LogLevel is high enough for each type. */
class Information : public StreamClass
{
public:
  template <typename T>
  friend std::ostream &operator<<(const Information &info, const T &in)
  {
    if (info._lvl > LOG_INFO - 1)
      return std::cout << in;
    else
      return *_garbage;
  }
};

class Warning : public StreamClass
{
public:
  template <typename T>
  friend std::ostream &operator<<(const Warning &info, const T &in)
  {
    if (info._lvl > LOG_WARNINGS - 1)
      return std::cout << in;
    else
      return *_garbage;
  }
};

class Error : public StreamClass
{
public:
  template <typename T>
  friend std::ostream &operator<<(const Error &info, const T &in)
  {
    if (info._lvl > LOG_ERRORS - 1)
      return std::cout << in;
    else
      return *_garbage;
  }
};

class Debug : public StreamClass
{
public:
  template <typename T>
  friend std::ostream &operator<<(const Debug &info, const T &in)
  {
    if (info._lvl > LOG_DEBUG - 1)
      return std::cout << in;
    else
      return *_garbage;
  }
};

/* Logger is a class that contains public Information, Warning, Error and Debug
objects. Where everyone have the same current LogLevel state. */
class Logger
{
public:
  Logger();

  // Sets/gets the LogLevel of all the member objects. 
  void set_logLevel(const LogLevel &lvl);
  LogLevel get_logLevel() const;

  Information info;
  Warning warning;
  Error error;
  Debug debug;

private:
  LogLevel _lvl; // The LogLevel deciding what is being printed to the console.
};

}