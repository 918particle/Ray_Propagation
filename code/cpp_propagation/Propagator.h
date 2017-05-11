#include "Ice.h"
#include "Reflector.h"
#include "Emitter.h"
#include <string>

#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

class Propagator : public Ice, public Reflector, public Emitter
{
	public:
		float _globalTime; //nanoseconds
		float _timeStep; //nanoseconds
		std::pair<float,float> _currentPosition; //(x,z) coordinates, meters
		//The path traveled in (x,z) coordinates by the ray, and each pair goes with one time-step.
		std::vector<std::pair<float,float> > _path;
		bool _isInitialized; //1 if InitializePropagator has been executed.
		//Constructor
		Propagator() : _globalTime(0.0), _timeStep(0.1), _isInitialized(false)
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
