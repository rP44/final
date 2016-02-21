#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include "foo.h"

int main( int argc, char** argv )
{
	int ch = 0;
  std::string host;
  std::string port;
  std::string directory;
  const char* const PROGRAMM_NAME = argv[ 0 ];
  const char* const USAGE_FORMAT  = "Usage: %s -h <ip> -p <port> -d <directory>\n";

  if ( argc < 7 )
  {
    std::fprintf( stderr, USAGE_FORMAT, PROGRAMM_NAME );
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
      std::fprintf( stderr, USAGE_FORMAT, PROGRAMM_NAME );
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

	return EXIT_SUCCESS;
}


