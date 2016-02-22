#pragma once
#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

#include <string>
#include <fstream>
#include <stddef.h>

int set_nonblock( int fd );

void print_error_message_and_exit( const char* msg );

std::string get_path( const char* buff, ptrdiff_t size );

std::string get_file_text( std::ifstream& in );

#endif
