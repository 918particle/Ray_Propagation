#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"
#include "Polarization.h"

int main(int argc, char *argv[])
{
	//Seed rand()
	srand(time(NULL));
	float angle_1 = 20.0;
	float angle_2 = 25.0;
	float dtheta = 0.5;
	int count = 0;
	int nrays = 10;
	int globalCount = 0;
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(0.0,-200.0);
	std::pair<bool,bool> preferences(true,true);
	std::vector<float> pol(3);
	pol[0] = 0.0;
	pol[1] = 0.0;
	pol[2] = 1.0;
	float amplitude = 1.0;
	float globalTime = 10000.0;

	Propagator p;
	p._ReflectionMethod = 1; // atof(argv[2]);   <--- For python script
	if(p._ReflectionMethod == 1)  // Reflections only at hardcoded locations
	{
		p.AddReflector(std::pair<float,float>(-14.0,0.13),0.1);    //atof(argv[1]));   <---When using python script
	}    
	if(p._ReflectionMethod == 2)  // Every point in propogation is treated as possibe reflector
	{
		p.ReflectionSmoothness(0.1); // atof(argv[1]);  <--- python script, Gaussian STD;
	}

	for(float t=angle_1;t<=angle_2;t+=dtheta)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<nrays;++i)
		{
			p.InitializePropagator(globalTime,ice_d,preferences,"SPICE",emitter_p,t,pol);
			p.Propagate(globalCount);
			std::stringstream ss;
			ss<<count;
			std::string title = "data/propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
			++globalCount;
		}
	}
}
