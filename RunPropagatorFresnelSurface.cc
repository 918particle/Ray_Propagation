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
	float angle_i = -10.0;
	float angle_f = 10.0;
	float delta_angle = 1;
	int nrays = 100;
	std::vector<float> pm;
	pm.push_back(1.0);
	pm.push_back(0.0);
	pm.push_back(0.0);
	float globalTime = 4000.0;
	float timeStep = 2;
	float propagator_z = -0.75;
	float propagator_y = 0.0;
	float layer_depth = -10.0;
	float reflection_coefficient = 0.05;
	float angular_spread = 0.02;
	float perturb_depth = -2.0;
	float perturb_strength = 0.03;
	float perturb_width = 2.0;

	std::pair<float,float> refl1(layer_depth,reflection_coefficient);
	std::pair<bool,float> refl2(true,angular_spread);

	for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
	{
		#pragma omp parallel for
		for(int i=0;i<nrays;++i)
		{
			Propagator *m = new Propagator();
			m->SetGlobalTimeAndStep(globalTime,timeStep);
			m->InitializePropagator(propagator_y,propagator_z,angle);
			m->SetPolarization(pm);
			m->AddReflector(refl1,refl2);
			m->AddGaussianPerturbation(perturb_depth,perturb_strength,perturb_width);
			m->Propagate();
			std::stringstream ss1,ss2;
			ss1<<angle;
			ss2<<i;
			m->ReadoutPath("data/sp_propagation_path_"+ss1.str()+"_"+ss2.str()+".dat");
			delete m;
		} //The ray loop
	}  //The angle loop
	return 0;
}