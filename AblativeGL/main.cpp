#include <stdlib.h>
#include <iostream>

#include "PhysXFluid.h"
#include "PlanetGenerator.h"

int main(int argc, char** argv) 
{
	Configuration config("config.yaml");

	//PhysXFluid sim;
	//sim.Run(config);

	PlanetGenerator sim;
	sim.Run(config);

	return 0;
}
