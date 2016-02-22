#pragma once
#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

#include <string>
#include <fstream>
#include <stddef.h>
#include <tuple>

enum { HOST, PORT, DIRECTORY };

extern const char* const SUCCESS_RESPONSE_FORMAT;
extern const char* const FAILURE_RESPONSE_FORMAT;

void log_and_exit( std::ostream& log, const char *msg );

std::string get_path( const char* buff, ptrdiff_t size );

std::string get_file_text( std::istream& in );

void processing_request( int slave, const std::string& directory,
  std::ostream& daemon_log );

std::tuple< std::string, std::string, std::string >
  check_program_arguments( int& argc, char**& argv );

void daemonization();

#endif
