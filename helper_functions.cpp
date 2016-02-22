#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "helper_functions.h"

int set_nonblock( int fd )
{
  int flags;
#if defined( O_NONBLOCK )
  if ( -1 == ( flags = fcntl( fd, F_GETFL, 0 ) ) )
    flags = 0;
  return fcntl( fd, F_SETFL, flags | O_NONBLOCK );
#else
  flags = 1;
  return ioctl( fd, FIOBIO, &flags );
#endif
}

void print_error_message_and_exit( const char* msg )
{
  perror( msg );
  exit( EXIT_FAILURE );
}


