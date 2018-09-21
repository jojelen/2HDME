// BaseModel.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Base class for models
 * @author: Joel Oredsson
 *============================================================================*/
#include "BaseModel.h"

#include <iostream>

namespace THDME
{

std::atomic<bool> BaseModel::_first_run(true);
const std::string BaseModel::_version = "0.9";

using namespace std;

BaseModel::BaseModel() : _modelName("unknown model"), _files("output/")
{
  // Loop that runs only once per execution of a program
  if (_first_run)
  {
    if (std::atomic_exchange(&_first_run, false))
    {
      // SPheno writes a bunch of temp files. This is a clean up action.
#ifdef SPheno
      int error = system("rm -r temp");
      if (error == -1)
        _console.error << "[ERROR]: Couldn't clean up temp directory.\n";
#endif

      print_2hdme_info();
    }
  }
}

BaseModel::~BaseModel() {}

void BaseModel::print_2hdme_info()
{
  auto color1 = [](std::string text) {
    text.insert(0, "\x1B[36m");
    text.append("\x1B[0m\n");
    cout << text;
  };

  auto color2 = [](std::string text) {
    text.insert(0, "\x1B[34m");
    text.append("\x1B[0m\n");
    cout << text;
  };

  cout << "\n";
  color1(
      "********************************************************************");
  color1("  ____  _   _ ____  __  __       _____            _");
  color1(
      " |___ \\| | | |  _ \\|  \\/  |     | ____|_   _____ | |_   _____ _ __");
  color1("   __) | |_| | | | | |\\/| |_____|  _| \\ \\ / / _ \\| \\ \\ / / _ "
         "\\ '__|");
  color1("  / __/|  _  | |_| | |  | |_____| |___ \\ V / (_) | |\\ V /  __/ |");
  color1(
      " |_____|_| |_|____/|_|  |_|     |_____| \\_/ \\___/|_| \\_/ \\___|_|");
  cout << "\n";

  std::string version = "version: " + _version;
  color2(string(34 - (int)(version.size() / 2), ' ') + version);

  std::string author = "author: Joel Oredsson, joel_ored@hotmail.com";
  color2(string(34 - (int)(author.size() / 2), ' ') + author);

  cout << "\n";
  color1(
      "********************************************************************");
  cout << "\n";
}

} // namespace THDME