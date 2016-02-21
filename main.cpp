#include <thread>
#include <chrono>
#include <iostream>
#include "foo.h"

int main()
{
	std::thread thread{ []
	{
		std::cout
			<< "Hello, world from thread "
			<< std::this_thread::get_id()
			<< std::endl;
	} };

	std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	hello_world();
	
	thread.join();
	
	return 0;
}

