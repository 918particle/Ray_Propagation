# Ray_Propagation
1. Installation
The Makefile for this code is quite simple:

RunPropagatorOMP: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagator -fopenmp RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagator: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagator RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

clean:
	rm RunPropagator

By typing "make RunPropagatorOMP" you will compile with the OpenMP flag, -fopenmp.  This will compile the example program RunPropagator.cc.  OpenMP is useful for sending many rays in the same direction from the same Emitter object, because they are all independent of each other.  Use the code #pragma omp parallel for above a ray-loop to get a speed boost.  For example:

int nrays = 1000;
#pragma omp parallel for
for(int i=0;i<nrays;++i)
{
	Propagator *p = new Propagator();
	p->InitializePropagator(...);
	p->Propagate();
	p->ReadoutPath(...);
}

This loop will be split among your cores, depending on the environmental variable OMP_NUM_THREADS.  If you do not want to run in parallel mode, simply type "make RunPropagator" to compile without the -fopenmp flag.

2. Running the code
There are are 7 classes in this package, and the total number of lines of code is less than 600.  So when in doubt, just read the code.  The 7 classes are:

Emitter.h
Ice.h
Polarization.h
Propagator.h
Receiver.h
Reflector.h
RFRay.h

The base classes are: Polarization, Ice, and Reflector.  Emitter and Receiver inherit from Polarization, and RFRay inherits from Emitter and Ice. Propagator inherits from Reflector and RFRay.  There are get and set functions in many of the classes, but most of the time we rely on the default initialization of the classes.  For example, Ice.h defaults to the South Pole n(z) profile, but it can be changed if necessary.  All classes eventually lead to Propagator except Receiver, so if you want to change something, just alter the Propagator instance.  For example, suppose we wanted to change the the "Byrd" n(z) profile:

Propagator *p = new Propagator();
p->InitializePropagator(...);
p->SetIceModelName("Byrd");
p->Propagate();
p->SetIceModelName("MB1");
p->Propagate();

The least amount of setup that must be done before the propagation can be done is to initialize the propagator with two spatial coordinates and an angle with respect to horizontal.  The function ReadoutPath(...) allows one to print the data contained in 

RFRay::_path

so that it can be plotted.  There is a shell script that can be modified to suit your purposes with gnuplot.  I encourage you to plot paths in a way that makes the most sense for your project and in the plotting language that is most comfortable.  If you are having trouble getting the rays to reach a particular point in space, make sure you look at the function Propagator::SetGlobalTimeAndStep(), to add more time to the global clock.  Once time has reached the global time limit, the Propagator stops propagating.

3. Adding reflectors
The function AddReflector(...) creates a reflector that is infinite in horizontal extent with a reflection coefficient and either specular or Lambertian reflection.  The Fresnel equations are implemented (still checking all possible cases).  When reflectors are in play, you start to see interesting behavior beyond the normal shadowing effect.

4. Receivers
There is a new class called receiver, which I hope will eventually lead to antennas, RF channels, etc.  If an RFRay is near a receiver, where near means within a specified receiverRange, the Receiver will count it and record the dot-product between the polarization of the RFRay and the polarization of the Receiver.  Receivers instances also store Propagator objects that made it to the instance of the Receiver, so you can later write code that reports RFRay properties for only rays that are observed.

My contact information with any questions:
jhanson2@whittier.edu
918particle@gmail.com
(562) 824-5130 (Whittier College)