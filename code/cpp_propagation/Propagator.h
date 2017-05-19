#include "Ice.h"
#include "Reflector.h"
#include "Emitter.h"
#include "RFRay.h"
#include <string>

#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

class Propagator : public Reflector, public RFRay
{
	public:
		float _globalTime; //nanoseconds
		float _timeStep; //nanoseconds
		std::pair<float,float> _currentPosition; //(x,z) coordinates, meters
		bool _isInitialized; //1 if InitializePropagator has been executed.
		//Constructor
		Propagator() : _globalTime(0.0), _timeStep(10.0), _isInitialized(false)
		{
			_currentPosition.first = 0.0;
			_currentPosition.second = 0.0;
			_path.clear();
		};
		void InitializePropagator(float,float,float,bool,bool,std::string,float,float,float); //See source file.
		void AddReflector(float,float); //See source file.
		void Propagate(); //Propagate ray through medium
		void ReadoutPath(std::string); //Save path in an output file.
};

#endif
