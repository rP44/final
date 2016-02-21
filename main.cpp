#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include "foo.h"

int main( int argc, char** argv )
{
  int ch = 0;
  std::string host;
  std::string port;
  std::string directory;
  const char* const PROGRAM_NAME = argv[ 0 ];
  const char* const USAGE_FORMAT = "Usage: %s -h <ip> -p <port> -d <directory>\n";
  const size_t MIN_PROGRAM_ARGUMENTS = 7;

  if ( argc < MIN_PROGRAM_ARGUMENTS )
  {
    std::fprintf(stderr, USAGE_FORMAT, PROGRAM_NAME);
    std::exit( EXIT_FAILURE );
  }

  while ( ( ch = getopt( argc, argv, "h:p:d:" ) ) != EOF )
  {
    switch ( ch )
    {
    case 'h' :
      host = optarg;
      break;

    case 'p' :
      port = optarg;
      break;

    case 'd' :
      directory = optarg;
      break;

    default:
      std::fprintf(stderr, USAGE_FORMAT, PROGRAM_NAME);
      std::exit( EXIT_FAILURE );
    }
  }

  argc -= optind;
  argv += optind;

  if ( !host.empty() )
    std::cout << "host: " << host << std::endl;

  if ( !port.empty() )
    std::cout << "port: " << port << std::endl;

  if ( !directory.empty() )
    std::cout << "directory: " << directory << std::endl;

  int pid = fork();

  switch ( pid )
  {
  case 0:
    setsid();
    //chdir("/");

    close(0);
    close(1);
    close(2);
    break;

  case -1:
    std::printf("Error: unable to fork\n");
    break;

  default:
    std::printf( "Success: process %d went to background\n", pid );
    exit( EXIT_SUCCESS );
  }

  std::this_thread::sleep_for( std::chrono::minutes( 1 ) );
  std::ofstream out( "out" );
  out << "Hello, world from daemon!" << std::endl;

	return EXIT_SUCCESS;
}

