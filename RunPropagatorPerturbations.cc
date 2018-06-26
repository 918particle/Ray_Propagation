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
	float angle_i = -30.0;
	float angle_f = 30.0;
	float delta_angle = 1;
	int nrays = 1;
	std::vector<float> pr;
	pr.push_back(0.0);
	pr.push_back(0.0);
	pr.push_back(1.0);
	std::vector<float> pm;
	pm.push_back(0.0);
	pm.push_back(0.0);
	pm.push_back(1.0);
	float perturbation_depth = -50.0;
	float globalTime = 4000.0;
	float timeStep = 0.5;
	float z_0 = 40.0;
	float propagator_z = perturbation_depth-z_0;
	float middle_y = 2000.0;
	float y_0 = 250.0;
	float receiver_range = 1.0;
	float perturbation_size = 0.1;
	float perturbation_width = 20.0;

	for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
	{
		float receiver_y = middle_y+y_0;
		float propagator_y = middle_y-y_0;
		Receiver *r = new Receiver();
		r->InitializeReceiver(receiver_y,propagator_z,angle);
		r->SetReceiverRange(receiver_range);
		r->SetPolarization(pr);
		for(int i=0;i<nrays;++i)
		{
			Propagator *m = new Propagator();
			m->SetGlobalTimeAndStep(globalTime,timeStep);
			m->InitializePropagator(propagator_y,propagator_z,angle);
			m->SetPolarization(pm);
			m->AddGaussianPerturbation(perturbation_depth,perturbation_size,perturbation_width);
			m->Propagate();
			r->ReceiveRFRay(m);
			std::stringstream ss1,ss2;
			ss1<<angle;
			ss2<<i;
			m->ReadoutPath("data/sp_propagation_path_"+ss1.str()+"_"+ss2.str()+".dat");
			delete m;
		} //The ray loop
		delete r;
	}  //The angle loop
}