#include <thread>
#include <chrono>
#include <iostream>
#include "foo.h"

void hello_world()
{
	std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

	std::cout << "Hello, world!" << std::endl;
}

