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
		bool _tir; //Flag to record if TIR has occurred, reserved for surface.
		//Constructor
		Propagator() : _globalTime(0.0), _timeStep(5.0), _isInitialized(false)
		{
			_currentPosition.first = 0.0;
			_currentPosition.second = 0.0;
			_path.clear();
		};
		void InitializePropagator(float,std::pair<float,float>,std::pair<bool,bool>,std::string,std::pair<float,float>,float); //See source file.
		void AddReflector(std::pair<float,float>); //See source file.
		void Propagate(); //Propagate ray through medium
		void ReadoutPath(std::string); //Save path in an output file.
};

#endif
