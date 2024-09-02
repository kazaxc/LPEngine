#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "TestingApp.h"

int main(int argc, char** argv)
{
	// Must initialize logger before anything else
	LPEngine::Logger::Init();

	TestingApp app;

	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}