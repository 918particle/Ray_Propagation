RunPropagatorOMP: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagator -fopenmp RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagator: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagator RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

clean:
	rm RunPropagator
