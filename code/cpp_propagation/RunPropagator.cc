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
	p.AddReflector(-100.0,0.1);
	p.AddReflector(-90.0,0.1);
	p.AddReflector(-75.0,0.2);
	p.AddReflector(-70.0,0.2);
	p.AddReflector(-65.0,0.2);
	p.AddReflector(-60.0,0.2);
	p.AddReflector(0.0,1.0);
	p.AddReflector(-2700.0,1.0);
	for(float t=25.0;t<=35.0;t+=0.5)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<100;++i)
		{
			p.InitializePropagator(20000.0,1000.0,-2700.0,false,true,"SPICE",250.0,-600.0,t);
			p.Propagate();
			std::stringstream ss;
			ss<<count;
			std::string title = "data/propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}
