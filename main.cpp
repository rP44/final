#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <netinet/in.h>
//#include <ev.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "helper_functions.h"

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
    std::fprintf( stderr, USAGE_FORMAT, PROGRAM_NAME );
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
      std::fprintf( stderr, USAGE_FORMAT, PROGRAM_NAME );
      std::exit( EXIT_FAILURE );
    }
  }

  argc -= optind;
  argv += optind;

  /*if ( !host.empty() )
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

    close( 0 );
    close( 1 );
    close( 2 );
    break;

  case -1:
    std::printf( "Error: unable to fork\n" );
    break;

  default:
    std::printf( "Success: process %d went to background\n", pid );
    exit( EXIT_SUCCESS );
  }*/

  int master = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if ( master == -1 )
    print_error_message_and_exit( "socket" );

  struct sockaddr_in addr;
  memset( &addr, 0, sizeof( addr ) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons( ( uint16_t )std::stoul( port ) );
  addr.sin_addr.s_addr = htonl( INADDR_ANY );
  //addr.sin_addr.s_addr = htonl( inet_addr( host.c_str() ) );

  if ( bind( master, ( sockaddr* )&addr, sizeof( addr ) ) == -1 )
    print_error_message_and_exit( "bind" );

  int optval = 1;
  if ( setsockopt( master, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof( optval ) ) == -1 )
    print_error_message_and_exit( "setsockopt" );

  /*if ( setsockopt( master, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof( optval ) ) == -1 )
    print_error_message_and_exit( "setsockopt" );*/

  if ( listen( master, SOMAXCONN ) == -1 )
    print_error_message_and_exit( "listen" );

  const size_t BUF_SIZE = 1024;
  char buff[ BUF_SIZE ];

  int slave = 0;

  while ( slave = accept( master, nullptr, 0) )
  {
    if ( slave == -1 )
      print_error_message_and_exit( "accept" );

    //set_nonblock( slave );

    memset( buff, 0, BUF_SIZE );

    if ( recv( slave, buff, BUF_SIZE - 1, MSG_NOSIGNAL ) == -1 )
      print_error_message_and_exit( "recv" );

    shutdown( slave, SHUT_RDWR );
    close( slave );

    printf( "%s\n", buff );
  }

	return EXIT_SUCCESS;
}

