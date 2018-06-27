#include "Ice.h"
#include "Reflector.h"
#include "Emitter.h"
#include "RFRay.h"

#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

class Propagator : public Reflector, public RFRay
{
	public:
		bool _isInitialized; //1 if InitializePropagator has been executed.
		//Constructor
		Propagator() : 
			_isInitialized(false),
			_globalTime(10000.0),
			_timeStep(1.0) {};
		void InitializePropagator(float,float,float); //See source file.
		void AddReflector(std::pair<float,float>,std::pair<int,float>); //See source file.
		void Propagate(); //Propagate ray through medium
		void ReadoutPath(std::string); //Save path in an output file.
		float _globalTime; //Maximum propgation time of RFRay via Propagator before anything interrupts, nanoseconds
		float _timeStep; //Time step for Propagator, nanoseconds
		void SetGlobalTimeAndStep(float,float); //Set _globalTime and _timeStep
};

#endif
