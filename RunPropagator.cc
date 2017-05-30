#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"

int main(int argc, char *argv[])
{
	//Seed rand()
	srand(time(NULL));
	int count = 0;
	Gaussian g;
	Propagator p;
	p.scatteringType = atoi(argv[1]); // 1: Speculac, 2: Diffuse
	int pos_angle = 0;

	for(float z=-75.0;z<=-70.0;z+=5.0)
	{
		float r = float(rand())/float(RAND_MAX)/10.0;
		p.AddReflector(z,r);
		std::cout<<"Added reflecting layer at "<<z<<" meters with R= "<<r<<std::endl;
	}
	for(float t=35;t<=35;t+=0.5)
	{
		std::cout<<"Angle: "<<t<<std::endl;
		for(int i=0;i<3000;++i)
		{
			// Get the right random gaussion number based off of incident direction
			if(t < 0)
			{
				pos_angle = -t; 
			}
			else
			{
				pos_angle = t;
			}
			p.gaussianDist = g.rand_normal(pos_angle,5.0); // inputs are mean,STD
			if(p.gaussianDist < 0.0)
			{
				p.gaussianDist = 0.0;
			}			
			if(p.gaussianDist > 180.0)
			{
				p.gaussianDist = 180.0;
			}
			if(t < 0)
			{
				p.gaussianDist = -p.gaussianDist;
			}
			// Run propagation functions
			p.InitializePropagator(20000.0,1000.0,-2700.0,true,true,"SPICE",250.0,-600.0,t);
			p.Propagate();
			// Terminal output
			std::stringstream ss;
			ss<<count;
			// Create data files
			std::string title = "data/propagation_path_"+ss.str()+".dat";
			p.ReadoutPath(title);
			++count;
		}
	}
}