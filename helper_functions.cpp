#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <mutex>
#include <iostream>
#include "helper_functions.h"

const char* const SUCCESS_RESPONSE_FORMAT =
  "HTTP/1.0 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "\r\n"
  "%s";

const char* const FAILURE_RESPONSE_FORMAT =
  "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";

std::mutex daemon_log_mutex;

static const size_t SLASH_POS = 4;

void log_and_exit( std::ostream& log, const char* msg )
{
  std::lock_guard< std::mutex > lock_guard( daemon_log_mutex );
  log << msg << ": " << strerror( errno ) << std::endl;
  exit( EXIT_FAILURE );
}

std::string get_path( const char* buff, ptrdiff_t size )
{
  if ( buff[ SLASH_POS ] != '/' )
    return { "/" };

  std::string tmp( buff, size );
  size_t end = tmp.find_first_of( " ?", SLASH_POS );
  std::string result( tmp.substr( SLASH_POS, end - SLASH_POS ) );

  if ( result[ result.size() - 1 ] == '/' )
    result += "index.html";

  return result;
}

std::string get_file_text( std::istream& in )
{
  std::string result;

  char ch = 0;

  while ( in.get( ch ) )
    result += ch;

  return result;
}

void processing_request( int slave, const std::string& directory,
  std::ostream& daemon_log )
{
  std::string path_to_file;
  const size_t BUF_SIZE = 1024;
  char buff[ BUF_SIZE ];
  ptrdiff_t recv_size = 0;
  memset( buff, 0, BUF_SIZE );

  if ( ( recv_size = recv( slave, buff, BUF_SIZE - 1, MSG_NOSIGNAL ) ) == -1 )
    log_and_exit( daemon_log, "recv" );

  path_to_file = get_path( buff, recv_size );

  if ( path_to_file == "/" )
    path_to_file += "index.html";

  std::ifstream in( directory + path_to_file );
  if ( in )
  {
    std::string text = get_file_text( in );
    snprintf( buff, BUF_SIZE, SUCCESS_RESPONSE_FORMAT, text.c_str() );

    if ( send( slave, buff, strlen( buff ), MSG_NOSIGNAL ) == -1 )
      log_and_exit( daemon_log, "send" );
  }
  else
  {
    snprintf( buff, BUF_SIZE, FAILURE_RESPONSE_FORMAT );

    if ( send( slave, buff, strlen( buff ), MSG_NOSIGNAL ) == -1 )
      log_and_exit( daemon_log, "send" );
  }

  shutdown( slave, SHUT_RDWR );
  close( slave );
}

std::tuple< std::string, std::string, std::string >
check_program_arguments( int& argc, char**& argv )
{
  int ch = 0;
  std::string host;
  std::string port;
  std::string directory;
  const char* const PROGRAM_NAME = argv[ 0 ];
  const char* const USAGE_FORMAT =
              "Usage: %s -h <ip> -p <port> -d <directory>\n";
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

  return std::make_tuple( host, port, directory );
}

void daemonization()
{
  int pid = fork();

  switch ( pid )
  {
  case 0:
    setsid();
    close( 0 );
    close( 1 );
    close( 2 );
    break;

  case -1:
    log_and_exit( std::cerr, "fork" );

  default:
    std::printf( "success: process %d went to background\n", pid );
    exit( EXIT_SUCCESS );
  }
}
