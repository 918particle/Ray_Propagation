#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Propagator.h"

int main(int argc,char **argv)
{
	//Seed rand()
	srand(time(NULL));
	std::pair<float,float> ice_d(1000.0,-2000.0);
	std::pair<float,float> emitter_p(100.0,-10);
	std::pair<bool,bool> preferences(true,true);
	std::string title;
	
	Propagator p;
	p.InitializePropagator(400.0,ice_d,preferences,"SPICE",emitter_p,atof(argv[1]));
	p.Propagate();
	std::stringstream ss;
	ss<<atof(argv[1]);
	title = "propagation_path_"+ss.str()+".dat";
	p.ReadoutPath(title);
}