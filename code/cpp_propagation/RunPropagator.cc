#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"

int main()
{
	//Seed rand()
	srand(time(NULL));
	int count = 0;
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(0.0,-800.0);
	std::pair<bool,bool> preferences(true,true);
	float globalTime = 10000.0;

	Propagator p;
	for(float z=-75.0;z<0.0;z+=5.0)
	{
		float r = float(rand())/float(RAND_MAX)/10.0;
		p.AddReflector(std::pair<float,float>(z,r));
		std::cout<<"Added reflecting layer at "<<z<<" meters with R= "<<r<<std::endl;
	}
	for(float t=10.0;t<=60.0;t+=1.0)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<100;++i)
		{
			p.InitializePropagator(globalTime,ice_d,preferences,"SPICE",emitter_p,t);
			p.Propagate();
			std::stringstream ss;
			ss<<count;
			std::string title = "data/propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}
