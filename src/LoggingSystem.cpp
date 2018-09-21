// LoggingSystem.cpp is a part of 2HDME.
// Copyright (C) 2018 Joel Oredsson.
// 2HDME is licenced under the GNU GPL v3 or later, see LICENSE for details.
/**=============================================================================
 * @brief: Log class for printing to console
 * @author: Joel Oredsson
 *============================================================================*/
#include "LoggingSystem.h"

namespace THDME {

/* The _garbage points nowhere. Thus writing to it produces no
  output in the console. */
std::ostream* StreamClass::_garbage = new std::ostream(nullptr);

void StreamClass::set_logLevel(const LogLevel& lvl)
{
	_lvl = lvl;
}

Logger::Logger()
{
	set_logLevel(LOG_INFO);
}

void Logger::set_logLevel(const LogLevel& lvl) {
	_lvl = lvl;
	info.set_logLevel(lvl);
	warning.set_logLevel(lvl);
	error.set_logLevel(lvl);
	debug.set_logLevel(lvl);
}

LogLevel Logger::get_logLevel() const
{
	return _lvl;
}

}