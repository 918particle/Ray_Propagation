RunPropagatorFresnelOMP: RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnel -fopenmp RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorFresnel: RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorFresnel RunPropagatorFresnel.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorPerturbationsOMP: RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorPerturbations -fopenmp RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

RunPropagatorPerturbations: RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc
	g++ -g -Wall -o RunPropagatorPerturbations RunPropagatorPerturbations.cc Emitter.cc Propagator.cc Reflector.cc Ice.cc RFRay.cc Receiver.cc -I.

clean:
	rm RunPropagatorFresnel
	rm RunPropagatorFresnelOMP
	rm RunPropagatorPerturbations
	rm RunPropagatorPerturbationsOMP