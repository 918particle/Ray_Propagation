#include <iostream>
#include "Propagator.h"

int main()
{
	Propagator p;
	p.InitializePropagator(10000.0,1000.0,-2700.0,true,true,"SPICE",0.0,300.0,-30.0);
	p.Propagate();
	p.ReadoutPath("propagation_path_1.dat");
	//~ p.InitializePropagator(1000.0,1000.0,-2700.0,true,true,"SPICE",10.0,-400.0,10.0);
	//~ p.Propagate();
	//~ p.ReadoutPath("propagation_path_2.dat");
	//~ p.InitializePropagator(1000.0,1000.0,-2700.0,true,true,"SPICE",10.0,-1000.0,10.0);
	//~ p.Propagate();
	//~ p.ReadoutPath("propagation_path_3.dat");
}
