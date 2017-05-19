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
	
	Propagator p;
	for(float z=-75.0;z<0.0;z+=5.0)
	{
		float r = float(rand())/float(RAND_MAX)/10.0;
		p.AddReflector(z,r);
		std::cout<<"Added reflecting layer at "<<z<<" meters with R= "<<r<<std::endl;
	}
	for(float t=-6.5;t<=-6.5;t+=0.5)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<1;++i)
		{
			p.InitializePropagator(20000.0,1000.0,-2700.0,true,true,"SPICE",0.0,20.0,t);
			p.Propagate();
			std::stringstream ss;
			ss<<count;
			std::string title = "propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}
