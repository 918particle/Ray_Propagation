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
	int count = 0;
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(0.0,-800.0);
	std::pair<bool,bool> preferences(true,true);
	std::vector<float> pol(3);
	pol[0] = 0.0;
	pol[1] = 0.0;
	pol[2] = 1.0;
	float globalTime = 10000.0;

	Propagator p;
	p.AddReflector(std::pair<float,float>(-1.0,0.13),std::pair<int,float>(2,0.1));

	for(float t=40.0;t<=50.0;t+=0.5)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<100;++i)
		{
			p.InitializePropagator(globalTime,ice_d,preferences,"SPICE",emitter_p,t,pol);
			p.Propagate();
			std::stringstream ss;
			ss<<count;
			std::string title = "data/propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}
