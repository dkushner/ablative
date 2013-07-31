#include <stdlib.h>
#include <iostream>

#include "PhysXFluid.h"

int main(int argc, char** argv) 
{
	Configuration config;
	config.WindowWidth = 800;
	config.WindowHeight = 600;
	config.WindowTitle = "AblativeGL";

	PhysXFluid sim;
	sim.Run(config);

	return 0;
}
