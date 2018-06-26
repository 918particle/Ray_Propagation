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
	float angle_i = 0.0;
	float angle_f = 180.0;
	float delta_angle = 1;
	std::vector<float> pm;
	pm.push_back(0.0);
	pm.push_back(0.0);
	pm.push_back(1.0);
	float globalTime = 200.0;
	float timeStep = 0.1;
	float propagator_z = -10.0;
	float propagator_y = 2000.0;

	for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
	{
		Propagator *m = new Propagator();
		m->SetIceModelName("Flat");
		m->SetGlobalTimeAndStep(globalTime,timeStep);
		m->InitializePropagator(propagator_y,propagator_z,angle);
		m->SetPolarization(pm);
		m->Propagate();
		std::stringstream ss1;
		ss1<<angle;
		m->ReadoutPath("data/sp_propagation_path_"+ss1.str()+".dat");
		delete m;
	}
}