RunPropagatorFresnelOMP: RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnel -fopenmp RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorFresnel: RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnel RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorPerturbationsOMP: RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorPerturbations -fopenmp RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorPerturbations: RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorPerturbations RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorFresnelSurfaceOMP: RunPropagatorFresnelSurface.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnelSurface -fopenmp RunPropagatorFresnelSurface.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorFresnelSurface: RunPropagatorFresnelSurface.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnelSurface RunPropagatorFresnelSurface.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunOMP: Run.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunOMP -fopenmp Run.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

Run: Run.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o Run Run.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.
