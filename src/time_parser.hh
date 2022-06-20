// General utils for mta stuff
#ifndef MTA_TIME_PARSER
#define MTA_TIME_PARSER

#include <stdlib.h>

#include <chrono>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

// Copied from the interwebz (how is this not standardized is beyond me)
std::string get_time(time_t unix_timestamp);

#endif // MTA_TIME_PARSER header guard
