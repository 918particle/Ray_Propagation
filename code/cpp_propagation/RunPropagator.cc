#include <iostream>
#include <sstream>
#include <string>
#include "Propagator.h"

int main()
{
	Propagator p;
	for(float t=35.0;t<=45.0;t+=0.5)
	{
		p.InitializePropagator(10000.0,1000.0,-2700.0,true,true,"SPICE",250.0,-900.0,t);
		p.Propagate();
		std::stringstream ss;
		ss<<t;
		std::string title = "propagation_path_"+ss.str()+".dat";
		p.ReadoutPath(title);
	}
}
