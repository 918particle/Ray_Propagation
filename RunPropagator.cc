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
	float angle_f = 90.0;
	float delta_angle = 2;
	int nrays = 100;
	std::vector<float> pr;
	pr.push_back(0.0);
	pr.push_back(0.0);
	pr.push_back(1.0);
	std::vector<float> pm_p;
	pm_p.push_back(0.0);
	pm_p.push_back(0.0);
	pm_p.push_back(1.0);
	std::vector<float> pm_s;
	pm_s.push_back(1.0);
	pm_s.push_back(0.0);
	pm_s.push_back(0.0);
	float layer_depth = -800.0;
	float reflection_coefficient = 0.1;
	float globalTime = 5000.0;
	float timeStep = 0.5;
	float z_0 = 20.0;
	float propagator_z = layer_depth-z_0;
	float middle_y = 2000.0;
	float receiver_range = 1.0;
	bool diffuse_reflection = false;
	float delta_angle_reflection = 0.01;
	float reflector_range = timeStep*0.3/1.78*1.5;

	std::pair<float,float> refl1(layer_depth,reflection_coefficient);
	std::pair<bool,float> refl2(diffuse_reflection,delta_angle_reflection);

	for(float angle=angle_i;angle<angle_f;angle+=delta_angle)
	{
		float receiver_y = middle_y+z_0/tan(angle*3.14159/180.0);
		float propagator_y = middle_y-z_0/tan(angle*3.14159/180.0);
		Receiver *r_s = new Receiver();
		Receiver *r_p = new Receiver();
		r_s->InitializeReceiver(receiver_y,propagator_z,angle);
		r_s->SetReceiverRange(receiver_range);
		r_s->SetPolarization(pr);
		r_p->InitializeReceiver(receiver_y,propagator_z,angle);
		r_p->SetReceiverRange(receiver_range);
		r_p->SetPolarization(pr);
		for(int i=0;i<nrays;++i)
		{
			Propagator *m_s = new Propagator();
			m_s->SetGlobalTimeAndStep(globalTime,timeStep);
			m_s->InitializePropagator(propagator_y,propagator_z,angle);
			m_s->SetPolarization(pm_s);
			m_s->AddReflector(refl1,refl2);
			m_s->SetReflectorRange(reflector_range);
			m_s->Propagate();
			r_s->ReceiveRFRay(m_s);
			delete m_s;
			Propagator *m_p = new Propagator();
			m_p->SetGlobalTimeAndStep(globalTime,timeStep);
			m_p->InitializePropagator(propagator_y,propagator_z,angle);
			m_p->SetPolarization(pm_p);
			m_p->AddReflector(refl1,refl2);
			m_s->SetReflectorRange(reflector_range);
			m_p->Propagate();
			r_p->ReceiveRFRay(m_p);
			delete m_p;
		} //The ray loop

		std::cout<<angle<<" "<<r_s->GetReceivedRFRays()<<" "<<std::sqrt(r_s->GetReceivedRFRays())<<" ";
		std::cout<<r_p->GetReceivedRFRays()<<" "<<std::sqrt(r_p->GetReceivedRFRays())<<std::endl;
		delete r_s;
		delete r_p;
	}  //The angle loop
}