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
	for(float t=-10.0;t<=10.0;t+=0.5)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<100;++i)
		{
			p.InitializePropagator(20000.0,1000.0,-576.0,true,true,"MB1",20.0,-2.0,t);
			p.Propagate();
			std::stringstream ss;
			ss<<count;
			std::string title = "propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}
