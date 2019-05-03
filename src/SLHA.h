// SLHA.h is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Class for writing SLHA format files
 * @author: Joel Oredsson
 *
 * SLHA files are human readable text files containing parameters that are
 * organized in blocks. An example of such a block is:
 *
 *   BLOCK MIJ2 # Mij^2
 *     1   1.86914510e+05 # M112
 *     2   1.99660881e+04 # M222
 *
 * Each block has a name (MIJ2 here) and potentially a comment for every line
 * that denotes the block name or parameter (e.g. M112 and M222).
 * The parameters in each block is written in a line and are defined by a number
 * of indices (here, it is only one index). The last number, before any #, is
 * the value of the parameter.
 *
 * The class SLHA_FILE can be loaded with an external SLHA file. It saves all
 * the blocks in a vector of SLHA_BLOCK components. Each parameter is specified
 * as a SLHA_PARAMETER.
 *
 * Example usage:
 *   // Creates a block:
 *   SLHA_BLOCK block("MIJ2", "Mij^2");
 *   block.add_parameter(1, _base_generic.M112, "M112");
 *   block.add_parameter(2, _base_generic.M222, "M222");
 *   // Creates a file and gives it the block and saves it:
 *   SLHA_FILE file("Example created by 2HDME");
 *   file.add_block(block);
 *   file.save_to_file("fileName");
 *
 *============================================================================*/
#pragma once

#include <fstream>
#include <string>
#include <vector>

struct SLHA_PARAMETER // SLHA parameter building block
{
  std::vector<int> ID; // The indices that specifies the parameter
  double value;
  std::string comment;

  friend std::ostream &operator<<(std::ostream &oS,
                                  const SLHA_PARAMETER &parameter);
};

// Sets the parameter from a string, returns false if line contains less than
// 2 words (excluding comment).
bool retrieve_SLHA_PARAMETER(SLHA_PARAMETER &parameter,
                             const std::string &line);

//-----------------------------------------------------------------------------

class SLHA_BLOCK // A block that contains a number of SLHA_PARAMETERs
{
public:
  SLHA_BLOCK(const std::string &blockName, const std::string &comment = "");

  // Different functions that adds a parameter to the block.
  void add_parameter(const SLHA_PARAMETER parameter);
  void add_parameter(const int &id, const double &value,
                     const std::string &comment);
  void add_parameter(const int &id1, const int &id2, const double &value,
                     const std::string &comment);
  void add_parameter(const int &id1, const int &id2, const int &id3,
                     const double &value, const std::string &comment);
  void add_parameter(const std::vector<int> &idVec, const double &value,
                     const std::string &comment);

  bool get_param(const std::vector<int> ID, double &value) const;

  void print() const;             // Prints to console with printf
  void print(FILE *output) const; // Prints to file with fprintf

  std::string get_name() const;

private:
  std::string _name;
  std::string _comment;
  std::vector<SLHA_PARAMETER> _paramVec;
};

//-----------------------------------------------------------------------------

class SLHA_FILE // Main class that stores the SLHA file in SLHA_BLOCKs
{
public:
  SLHA_FILE(const std::string &title = "SLHA file created by 2HDME");
  ~SLHA_FILE();

  bool load_file(const std::string &fileName);
  bool save_to_file(const std::string &fileName) const;

  void print_to_console() const;

  void add_block(const SLHA_BLOCK &block);
  void add_blocks(const std::vector<SLHA_BLOCK> blockVec);

  // Retrieves a parameter
  double get_param(const std::string &block, const int &ID1) const;
  double get_param(const std::string &block, const int &ID1,
                   const int &ID2) const;
  double get_param(const std::string &block, const int &ID, const int &ID2,
                   const int &ID3) const;
  double get_param(const std::string &block,
                   const std::vector<int> &idVec) const;

private:
  void print_header(FILE *output) const;

  std::string _title;
  std::vector<SLHA_BLOCK> _blockVec;
};