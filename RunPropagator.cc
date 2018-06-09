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
	float angle = 35.0;
	int nrays = 2000;
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(0.0,-800.0);
	std::pair<bool,bool> preferences(true,true);
	std::vector<float> pol(3);
	pol[0] = 0.0;
	pol[1] = 0.0;
	pol[2] = 1.0;
	float globalTime = 10000.0;
	#pragma omp parallel for
	for(int i=0;i<nrays;++i)
	{
		Propagator *p = new Propagator();
		p->InitializePropagator(globalTime,ice_d,preferences,"SPICE",emitter_p,angle,pol);\
		p->AddReflector(std::pair<float,float>(-200.0,0.1),std::pair<float,float>(1,0.01));
		p->Propagate();
		std::stringstream ss;
		ss<<i;
		p->ReadoutPath("data/propagation_path_"+ss.str()+".dat");
		delete p;
	}
}