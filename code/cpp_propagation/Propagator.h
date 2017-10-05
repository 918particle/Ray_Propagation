#include "Ice.h"
#include "Reflections.h"
#include "Refractions.h" 
#include "Emitter.h"
#include "RFRay.h"
#include "RFRayTracker.h"
#include "Constants.h"
#include "Collector.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
#pragma once
#include <cstdlib>
#include <string>

#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

class Propagator : public Reflections, public RFRay, public Collector
{
	public:
		// Global Variables
		float _globalTime; //nanoseconds
		float _timeStep; //nanoseconds
		bool _isInitialized;
		int _reflectionMethod; //Reflecting layers = 1, Any-point = 2

		//Constructor
		Propagator() : _globalTime(0.0), _timeStep(0.5), _isInitialized(false){};
		void InitializePropagator(float,std::vector<float>); //See source file.
		void AddReflector(std::vector<float>,float); //See source file.
		void ReflectionSmoothness(float);
		void Propagate(int); //Propagate ray through medium
		void ReadoutPath(int); //Save path in an output file.
		void ReadoutAngles(); //Save path in an output file.
		float Reflect(std::vector<float>);
		void CheckForReflection();
};

#endif
