// SLHA.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: Class for writing SLHA format files
 * @author: Joel Oredsson
 *
 * See header file for more details.
 *
 *============================================================================*/
#include "SLHA.h"
#include "HelpFunctions.h" // For date() function

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

std::ostream &operator<<(std::ostream &oS, const SLHA_PARAMETER &parameter)
{
  for (auto &i : parameter.ID)
    printf(" %i ", i);
  printf("%16.8e ", parameter.value);
  printf("# %s\n", parameter.comment.c_str());
  return oS;
}

bool retrieve_SLHA_PARAMETER(SLHA_PARAMETER &parameter,
                             const std::string &line)
{
  // std::cout << "Retrieving parameter from " << line << std::endl; //DEBUG
  // Creates a vector of words of line, except comment content(after #).
  string word, comment;
  vector<string> wordVec;
  for (std::istringstream iss(line); iss >> word;)
  {
    if (word == "#")
    {
      while (iss >> word)
      {
        comment += word;
        comment += " ";
      }
      comment.pop_back();
    }
    else
      wordVec.push_back(word);
  }

  // Each parameter must have at least one ID and one value.
  if (wordVec.size() < 2)
    return false;

  // Setting parameter
  try
  {
    for (auto it = wordVec.begin(); it != wordVec.end() - 1; ++it)
      parameter.ID.push_back(stoi(*it));
    parameter.value = std::stod(wordVec.back());
    parameter.comment = comment;
  }
  catch (const std::exception &e)
  {
    std::cout << "Exception when setting SLHA param. Message: "
              << e.what() << std::endl;
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------

SLHA_BLOCK::SLHA_BLOCK(const string &blockName, const string &comment)
    : _name(blockName), _comment(comment) {}

void SLHA_BLOCK::add_parameter(const SLHA_PARAMETER parameter)
{
  _paramVec.push_back(parameter);
}

void SLHA_BLOCK::add_parameter(const int &id, const double &value,
                               const std::string &comment)
{
  SLHA_PARAMETER parameter;
  parameter.ID.push_back(id);
  parameter.value = value;
  parameter.comment = comment;
  _paramVec.push_back(parameter);
}

void SLHA_BLOCK::add_parameter(const int &id1, const int &id2,
                               const double &value,
                               const std::string &comment)
{
  SLHA_PARAMETER parameter;
  parameter.ID.push_back(id1);
  parameter.ID.push_back(id2);
  parameter.value = value;
  parameter.comment = comment;
  _paramVec.push_back(parameter);
}

void SLHA_BLOCK::add_parameter(const int &id1, const int &id2, const int &id3,
                               const double &value,
                               const std::string &comment)
{
  SLHA_PARAMETER parameter;
  parameter.ID.push_back(id1);
  parameter.ID.push_back(id2);
  parameter.ID.push_back(id3);
  parameter.value = value;
  parameter.comment = comment;
  _paramVec.push_back(parameter);
}

void SLHA_BLOCK::add_parameter(const std::vector<int> &idVec,
                               const double &value,
                               const std::string &comment)
{
  SLHA_PARAMETER parameter;
  parameter.ID = idVec;
  parameter.value = value;
  parameter.comment = comment;
  _paramVec.push_back(parameter);
}

void SLHA_BLOCK::print() const
{
  printf("Block %s ", _name.c_str());
  printf("# %s\n", _comment.c_str());
  for (auto &param : _paramVec)
  {
    for (auto &i : param.ID)
      printf(" %i", i);
    if (param.value == (int)param.value)
      printf("   %i", (int)param.value);
    else
      printf(" %16.8e", param.value);
    printf(" # %s\n", param.comment.c_str());
  }
}

void SLHA_BLOCK::print(FILE *output) const
{
  fprintf(output, "Block %s ", _name.c_str());
  fprintf(output, "# %s\n", _comment.c_str());
  for (auto &param : _paramVec)
  {
    for (auto &i : param.ID)
      fprintf(output, " %i", i);
    if (param.value == (int)param.value)
      fprintf(output, "   %i", (int)param.value);
    else
      fprintf(output, " %16.8e", param.value);
    fprintf(output, " # %s\n", param.comment.c_str());
  }
}

bool SLHA_BLOCK::get_param(const std::vector<int> ID, double &value) const
{
  for (auto &param : _paramVec)
  {
    if ( ID == param.ID)
    {
      value = param.value;
      return true;
    }
  }
  return false;
}

std::string SLHA_BLOCK::get_name() const
{
  return _name;
}
//-----------------------------------------------------------------------------

SLHA_FILE::SLHA_FILE(const string &title) : _title(title) {}

SLHA_FILE::~SLHA_FILE() {}

bool SLHA_FILE::load_file(const string &fileName)
{
  printf("Loading file: %s\n", fileName.c_str());

  std::ifstream slhaFileStream(fileName, std::ios_base::in);

  if (!slhaFileStream)
  {
    cout << "[ERROR]: Couldn't open " << fileName << endl;
    return false;
  }

  string line, firstWord;
  while (std::getline(slhaFileStream, line))
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
      string blockType;
      iss >> blockType;

      // Looking for block comment
      string blockComment = "";
      iss >> firstWord;
      if (firstWord == "#")
      {
        while (iss >> firstWord)
          blockComment += firstWord + " ";
        if (blockComment.back() == ' ')
          blockComment.pop_back();
      }

      _blockVec.emplace_back(blockType, blockComment);
      continue;
    }
    else if (firstWord.front() == '#')
      continue;

    SLHA_PARAMETER parameter;
    if (retrieve_SLHA_PARAMETER(parameter, line))
    {
      _blockVec.back().add_parameter(parameter);
    }
  }

  printf("Loaded SLHA file with %lu blocks!\n", _blockVec.size());
  return true;
}

bool SLHA_FILE::save_to_file(const std::string &fileName) const
{
  FILE *output;
  output = fopen(fileName.c_str(), "w");

  if (output == NULL)
  {
    printf("Cannot open file \"%s\" for writing\n", fileName.c_str());
    return false;
  }

  print_header(output);
  for (auto &block : _blockVec)
    block.print(output);
  fclose(output);
  return true;
}

void SLHA_FILE::print_header(FILE *output) const
{
  string info = "#  Created: " + THDME::date() + "\n";
  fprintf(
      output,
      "##################################################################\n");
  fprintf(output, "#\n");
  fprintf(output, "#  %s\n", _title.c_str());
  fprintf(output, "%s", info.c_str());
  fprintf(output, "#\n");
  fprintf(
      output,
      "##################################################################\n");
}

void SLHA_FILE::add_block(const SLHA_BLOCK &block)
{
  _blockVec.push_back(block);
}

void SLHA_FILE::add_blocks(const std::vector<SLHA_BLOCK> blockVec)
{
  _blockVec.insert(std::end(_blockVec), std::begin(blockVec), std::end(blockVec));
}

void SLHA_FILE::print_to_console() const
{
  for (auto &block : _blockVec)
    block.print();
}

double SLHA_FILE::get_param(const std::string &blockName, const int &ID1) const
{
  std::vector<int> idVec;
  idVec.push_back(ID1);

  return get_param(blockName, idVec);
}

double SLHA_FILE::get_param(const std::string &blockName, const int &ID1, const int &ID2) const
{
  std::vector<int> idVec;
  idVec.push_back(ID1);
  idVec.push_back(ID2);

  return get_param(blockName, idVec);
}

double SLHA_FILE::get_param(const std::string &blockName, const int &ID1, const int &ID2, const int &ID3) const
{
  std::vector<int> idVec;
  idVec.push_back(ID1);
  idVec.push_back(ID2);
  idVec.push_back(ID3);

  return get_param(blockName, idVec);
}

double SLHA_FILE::get_param(const std::string &blockName, const std::vector<int> &idVec) const
{
  double value = 0.;

  for (auto &block : _blockVec)
    if (block.get_name() == blockName)
    {
      if (block.get_param(idVec, value))
      {
        return value;
      }
    }

  std::cout << "[ERROR]: Couldn't get parameter in block " << blockName << std::endl;

  return value;
}

