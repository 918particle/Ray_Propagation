#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"
#include "Receiver.h"

int main(int argc, char *argv[])
{
	//Seed rand()
	srand(time(NULL));
	float angle_i = 180.0;
	float angle_f = 200.0;
	float delta_angle = 1.0;
	int nrays = 1;
	for(int i=0;i<nrays;++i)
	{
		for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
		{
			Propagator *m = new Propagator();
			m->SetGlobalTimeAndStep(5000,0.5);
			m->InitializePropagator(2000,-50.0,angle);
			m->Propagate();
			std::stringstream ss1,ss2;
			ss1<<i;
			ss2<<int(angle);
			m->ReadoutPath("data/sp_propagation_path_"+ss2.str()+"_"+ss1.str()+".dat");
			delete m;
		}
	}
}