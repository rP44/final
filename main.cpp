#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "helper_functions.h"

int main( int argc, char** argv )
{
  auto args = check_program_arguments( argc, argv );
  std::string host      = std::move( std::get< HOST      >( args ) );
  std::string port      = std::move( std::get< PORT      >( args ) );
  std::string directory = std::move( std::get< DIRECTORY >( args ) );

  daemonization();

  std::ofstream daemon_log( "log.txt" );

  int master = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if ( master == -1 )
    log_and_exit( daemon_log, "socket");

  struct sockaddr_in addr;
  memset( &addr, 0, sizeof( addr ) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons( ( uint16_t )std::stoul( port ) );
  addr.sin_addr.s_addr = htonl( inet_addr( host.c_str() ) );

  if ( bind( master, ( sockaddr* )&addr, sizeof( addr ) ) == -1 )
    log_and_exit( daemon_log, "bind");

  int optval = 1;
  if ( setsockopt( master, SOL_SOCKET, SO_REUSEADDR, &optval,
    sizeof( optval ) ) == -1 )
    log_and_exit( daemon_log, "setsockopt");

  if ( listen( master, SOMAXCONN ) == -1 )
    log_and_exit( daemon_log, "listen");

  int slave = 0;

  while ( slave = accept( master, nullptr, 0) )
  {
    if ( slave == -1 )
      log_and_exit( daemon_log, "accept");

    std::thread worker{ &processing_request, slave, std::ref( directory ),
      std::ref( daemon_log ) };

    worker.detach();
  }

  return EXIT_SUCCESS;
}
