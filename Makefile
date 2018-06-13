RunPropagatorOMP: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc
	g++ -g -Wall -o RunPropagator -fopenmp RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc -I.

RunPropagator: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc
	g++ -g -Wall -o RunPropagator RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc -I.

clean:
	rm RunPropagator
