// FileSystem.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief:: FileSystem class
 * @author: Joel Oredsson
 *============================================================================*/
#include "FileSystem.h"
#include "HelpFunctions.h"
#include "RgeModel.h"
#include "THDM.h"

#include <Eigen/Dense>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace THDME {

FileSystem::FileSystem() : _created(0) {}

FileSystem::FileSystem(const std::string &dir_in)
    : _created(0), _directory(dir_in + "/") {}

FileSystem::~FileSystem() {
    for (unsigned int i = 0; i < _fileStreams.size(); i++)
        delete _fileStreams[i];

    _fileStreams.clear();
}

void FileSystem::set_directory(const std::string &dir_in) {
    _directory = dir_in + "/";
}

std::string FileSystem::get_directory() const { return _directory; }
bool FileSystem::create_multiple_files(const std::vector<std::string> &vec_in) {
    for (unsigned int i = 0; i < vec_in.size(); i++) {
        if (create_file(vec_in[i]) == false) return false;
    }
    return true;
}

bool FileSystem::create_file(const std::string &dir_in, bool append) {
    // Create an ofstream
    _fileStreams.push_back(new std::ofstream);

    // Maps the directory of the file to an index of the _fileStreams
    _fileMap[dir_in] = _created;

    // If the file does not exist -> create it
    if (file_exists(_directory + dir_in) == false) {
        create_folder_path(extract_folder_path(_directory + dir_in));
    }

    // Open file in append or overwrite mode
    if (append)
        _fileStreams[_created]->open(_directory + dir_in, std::fstream::app);
    else
        _fileStreams[_created]->open(_directory + dir_in, std::ios::out);

    _created++;
    return true;
}
bool FileSystem::open_file(const std::string &dir_in, bool append) {
    /* Maps the directory of the file to an index of the _fileStreams
       (the index is search->second). */
    auto search = _fileMap.find(dir_in);
    // Open file
    if (search != _fileMap.end()) {
        if (append)
            _fileStreams[search->second]->open(_directory + dir_in,
                                               std::fstream::app);
        else
            _fileStreams[search->second]->open(_directory + dir_in,
                                               std::ios::out);
        return true;
    }
    std::cout << "\n[ERROR]: could not open file " << dir_in.c_str()
              << std::endl;
    return false;
}

bool FileSystem::create_single_file(const std::string &fileName,
                                    const std::string &text) const {
    std::ofstream streamTemp;  // Temporary ofstream
    // Tries to open the ofstream
    streamTemp.open(_directory + fileName, std::ios::out);
    streamTemp << "";

    // If the file does not exist -> create it
    if (!file_exists(_directory + fileName)) {
        if (!create_folder_path(extract_folder_path(_directory + fileName)))
            return false;
        streamTemp.open(_directory + fileName);
    }
    streamTemp << text;  // Writes the text_in to file
    streamTemp.close();
    return true;
}

bool FileSystem::add_line(const std::string &name_in, const double &t_in,
                          const Eigen::Matrix3cd &matrix_in) {
    auto search = _fileMap.find(name_in);
    if (search != _fileMap.end()) {
        *_fileStreams[search->second] << exp(t_in);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                *_fileStreams[search->second] << "\t" << abs(matrix_in(i, j));
            }
        }
        *_fileStreams[search->second] << "\n";
        return true;
    }
    std::cout << "\n[ERROR]: could not add new line to " << name_in.c_str()
              << std::endl;
    return false;
}

void FileSystem::close_streams() {
    for (unsigned int i = 0; i < _created; i++) {
        _fileStreams[i]->close();
    }
}
void FileSystem::flush_streams() {
    for (unsigned int i = 0; i < _created; i++) {
        _fileStreams[i]->flush();
    }
}
}
