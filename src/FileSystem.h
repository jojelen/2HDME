// FileSystem.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: FileSystem class
 * @author: Joel Oredsson
 *
 * Handles input/output to data files.
 * 
 * Usage:
 *   FileSystem fileSystem("output/example");
 *   fileSystem.create_file("hej.dat", true);
 *   fileSystem.add_line("hej.dat", "hej");
 *   fileSystem.close_streams();
 *
 *============================================================================*/
#pragma once

#include "Structures.h"

#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace THDME {

class RgeModel;

// FileSystem handles I/O data to files.
class FileSystem {
public:
  /**
   * @brief: Simple constructor; does nothing
   */
  FileSystem();

  /**
   * @brief: Constructor that sets the directory, where files are stored.
   */
  FileSystem(const std::string &dir_in);

  /**
   * @brief: Destructor
   *
   * Deletes and clears _fileStreams.
   */
  ~FileSystem();

  /**
   * @brief: Sets _directory
   *
   * The _directory determines where to store output files.
   */
  void set_directory(const std::string &dir_in);
  std::string get_directory() const;

  /**
   * @brief: Creates a text file
   *
   * Set append=true to NOT overwrite files.
   *
   * @returns true if the file was created successfully; false otherwise.
   */
  bool create_multiple_files(const std::vector<std::string> &vec_in);
  bool create_file(const std::string &dir_in, bool append = false);
  bool open_file(const std::string &dir_in, bool append = true);

  
  // Creates a textfile
  bool create_single_file(const std::string &fileName,
                          const std::string &text) const;

  /**
   * @brief: Adds lines to text files.
   *
   * Functions that add a line to the file specified by name_in.
   * The ones with vector arguments separate each entry with a tab.
   *
   * @returns false if it couldn't add line to file; true otherwise.
   */
  template <typename T>
  bool add_line(const std::string &name_in, const T &string_in) {
    auto search = _fileMap.find(name_in);
    if (search != _fileMap.end()) {
      *_fileStreams[search->second] << string_in << "\n";
      return true;
    }
    std::cout << "\n[ERROR]: could not add new line to " << name_in.c_str()
              << std::endl;
    return false;
  }

  template <typename T>
  bool add_line(const std::string &name_in, const std::vector<T> &vector_in) {
    // searches the _fileMap for the right stream
    auto search = _fileMap.find(name_in);
    if (search != _fileMap.end()) {
      *_fileStreams[search->second] << vector_in[0];
      for (unsigned int j = 1; j < vector_in.size(); j++) {
        *_fileStreams[search->second] << "\t" << vector_in[j];
      }
      *_fileStreams[search->second] << "\n";
      return true;
    }
    // if name_in is not in _fileMap
    std::cout << "\n[ERROR]: could not add new line to " << name_in.c_str()
              << std::endl;
    return false;
  }

  // Special function for matrices
  bool add_line(const std::string &name_in, const double &t_in,
                const Eigen::Matrix3cd &matrix_in);

  void close_streams();
  void flush_streams();

private:
  unsigned int _created;

  std::string _directory; // specifies output

  // Map from filename to _fileStreams index.
  std::map<std::string, int> _fileMap;
  std::vector<std::ofstream *> _fileStreams;
};
}