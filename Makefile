RunPropagator: RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc
	g++ -o RunPropagator RunPropagator.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc -I.

RunPropagator2: RunPropagator2.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc
	g++ -o RunPropagator2 RunPropagator2.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc RFRayTracker.cc -I.

clean:
	rm RunPropagator
	rm Runpropagator2
