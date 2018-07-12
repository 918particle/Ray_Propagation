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
	std::vector<float> p;
	p.push_back(1.0);
	p.push_back(0.0);
	p.push_back(0.0);
	float globalTime = 4000.0;
	float timeStep = 1;
	float propagator_z = -0.5;
	float propagator_y = 0.0;
	int number_of_receivers = 40;
	float receiver_y = 550.0;
	float receiver_spacing = -1.0;

	float layer_depth = -100.0;
	float reflection_coefficient = 0.2;
	bool diffuse_reflection = true;
	float delta_angle_reflection = 0.01;
	float perturbation_depth = -0.5;
	float perturbation_strength = 0.1;
	float perturbation_width = 3.0;

	std::pair<float,float> refl1(layer_depth,reflection_coefficient);
	std::pair<bool,float> refl2(diffuse_reflection,delta_angle_reflection);

	std::vector<Receiver> *receivers = new std::vector<Receiver>;
	for(int i=0;i<number_of_receivers;++i)
	{
		Receiver *current_receiver = new Receiver();
		current_receiver->InitializeReceiver(receiver_y,i*receiver_spacing,0.0);
		current_receiver->SetPolarization(p);
		receivers->push_back(*current_receiver);
	}

	for(float angle=angle_i;angle<=angle_f;angle+=delta_angle)
	{
		for(int i=0;i<nrays;++i)
		{
			Propagator *R = new Propagator();
			R->SetIceModelName("MB1");
			R->SetGlobalTimeAndStep(globalTime,timeStep);
			R->InitializePropagator(propagator_y,propagator_z,angle);
			R->SetPolarization(p);
			R->AddReflector(refl1,refl2);
			R->AddGaussianPerturbation(perturbation_depth,perturbation_strength,perturbation_width);
			R->Propagate();
			std::stringstream ss1,ss2;
			ss1<<angle;
			ss2<<i;
			R->ReadoutPath("data/sp_propagation_"+ss1.str()+"_"+ss2.str()+".dat");
			std::vector<Receiver>::iterator j=receivers->begin();
			while( !((*j).ReceiveRFRay(R)) && j!=receivers->end()) ++j;
			delete R;
		} //The ray loop
	}  //The angle loop
	std::vector<Receiver>::iterator k;
	for(k=receivers->begin();k!=receivers->end();++k)
	{
		std::vector<float> t = (*k).GetReceivedTimes();
		for(unsigned int l=0;l<t.size();++l) std::cout<<t[l]<<std::endl;
	}
	delete receivers;
}