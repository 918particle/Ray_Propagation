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
	float angle_i = -5.0;
	float angle_f = 5.0;
	float delta_angle = 0.1;
	int nrays = 1;
	Receiver *r = new Receiver();
	r->InitializeReceiver(2500.0,-200.0,0.0);
	r->SetReceiverRange(1.0);
	std::vector<float> pr;
	pr.push_back(1.0);
	pr.push_back(0.0);
	pr.push_back(0.0);
	std::vector<float> pm;
	pm.push_back(0.0);
	pm.push_back(0.0);
	pm.push_back(1.0);
	r->SetPolarization(pr);
	for(int i=0;i<nrays;++i)
	{
		for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
		{
			Propagator *m = new Propagator();
			m->SetGlobalTimeAndStep(5000,0.5);
			m->InitializePropagator(2000,-50.0,angle);
			m->SetPolarization(pm);
			m->Propagate();
			r->ReceiveRFRay(m);
			delete m;
		}
	}
	std::cout<<r->GetReceivedRFRays()<<std::endl;
	std::cout<<std::endl;
	for(int i=0;i<r->GetReceivedRFRays();++i)
	{
		std::cout<<r->GetReceivedDotProduct(i)<<std::endl;
	}
	delete r;
}