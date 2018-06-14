#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"
#include "Receiver.h"

int main(int argc, char *argv[])
{
	std::pair<float,float> reflector_p(-35.0,0.05);
	std::pair<float,float> reflector_s(2,0.05);

	//Seed rand()
	srand(time(NULL));
	int nrays = 100;
	float angle_i = -10.0;
	float angle_f = 10.0;
	float delta_angle = 1.0;
	Receiver *r = new Receiver();
	r->InitializeReceiver(543.0,-2.0,0.0);
	#pragma omp parallel for
	for(int i=0;i<nrays;++i)
	{
		for(float angle = angle_i;angle<=angle_f;angle+=delta_angle)
		{
			Propagator *m = new Propagator();
			m->InitializePropagator(0.0,-2.0,angle);
			m->AddReflector(reflector_p,reflector_s);
			m->Propagate();
			r->ReceiveRFRay(m);
			// std::stringstream ss1,ss2;
			// ss1<<i;
			// ss2<<int(angle);
			// m->ReadoutPath("data/sp_propagation_path_"+ss2.str()+"_"+ss1.str()+".dat");
			delete m;
		}
	}
	delete r;
}