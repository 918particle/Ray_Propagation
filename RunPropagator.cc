#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"

int main(int argc, char *argv[])
{
	std::pair<float,float> reflector_p(-75.0,0.07);
	std::pair<float,float> reflector_s(2,0.05);

	//Seed rand()
	srand(time(NULL));
	int nrays = 2000;
	#pragma omp parallel for
	for(int i=0;i<nrays;++i)
	{
		Propagator *m = new Propagator();
		m->InitializePropagator(0.0,-400.0,25.0);
		m->AddReflector(reflector_p,reflector_s);
		m->Propagate();
		std::stringstream ss;
		ss<<i;
		m->ReadoutPath("data/sp_propagation_path_"+ss.str()+".dat");
		delete m;
	}
	#pragma omp parallel for
	for(int i=0;i<nrays;++i)
	{
		Propagator *m = new Propagator();
		m->SetIceModelName("MB1");
		m->InitializePropagator(0.0,-400.0,25.0);
		m->AddReflector(reflector_p,reflector_s);
		m->Propagate();
		std::stringstream ss;
		ss<<i;
		m->ReadoutPath("data/mb_propagation_path_"+ss.str()+".dat");
		delete m;
	}
}