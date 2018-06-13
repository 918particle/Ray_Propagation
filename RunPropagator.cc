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
	float angle_i = 20;
	float angle_f = 70;
	float delta_angle = 1.0;
	int nrays = 1;
	int count = 0;
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(0.0,-500.0);
	std::pair<bool,bool> preferences(true,true);
	std::vector<float> pol(3);
	pol[0] = 1.0;
	pol[1] = 0.0;
	pol[2] = 0.0;
	float globalTime = 10000.0;
	for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
	{
		#pragma omp parallel for
		for(int i=0;i<nrays;++i)
		{
			Propagator *p = new Propagator();
			p->InitializePropagator(globalTime,ice_d,preferences,"SPICE",emitter_p,angle,pol);
			// p->AddReflector(std::pair<float,float>(-75.0,1),std::pair<float,float>(1,0.1));
			p->Propagate();
			std::stringstream ss;
			ss<<count;
			p->ReadoutPath("data/propagation_path_"+ss.str()+".dat");
			delete p;
			++count;
		}
	}
}