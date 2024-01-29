#include <iostream>
#include <string>
#include "LPEngine.h"

int main(int argc, char** argv)
{
	LPEngine::Logger::Init();
	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Starting Engine!");

	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Creating Window!");

	LPEngine::Window window("Sandbox", 800, 600);

	LPEngine::Logger::Log(LPEngine::LogLevel::INFO, "Terminating Engine!");
	return 0;
}