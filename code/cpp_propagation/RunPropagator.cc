#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"
#include "Polarization.h"

int main(int argc, char *argv[])
{
	// Seed random number generator
	srand(time(NULL));

	// Initilize global variables
	Propagator p;
	p.Initialize(argv);

	// loop between range of input angles
	int count = 0;
	for(float t = p._angleI; t <= p._angleF; t+=p._dtheta)
	{
		std::cout << "Angle: " << t << std::endl;
		// loop through individual rays per angle
		for(int i = 0; i < p._nrays; ++i)
		{
			p.InitializePropagator(t,p._pol);
			p.Propagate(count);
			p.ReadoutPath(count);
			++count;
		}
		//p.ReadoutAngles();
	}
}