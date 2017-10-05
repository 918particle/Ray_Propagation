#include "Ice.h"
#include "Reflector.h"
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

class Propagator : public Reflector, public Reflections, public RFRay, public Collector
{
	public:
		// Global Variables
		float _globalTime; //nanoseconds
		float _timeStep; //nanoseconds
		bool _isInitialized; //1 if InitializePropagator has been executed.
		float _sigma;
		float _angleI;
		float _angleF;
		float _dtheta; 
		int _ReflectionMethod;
		int _scatterLength;
		int _nrays;
		std::vector<float> _emitterPos;
		std::vector<float> _pol;
		std::vector<float> anglesAt1400;
		std::vector<float> _iceSize;
		std::vector<float> previousPosition;
		std::pair<bool,bool> _preferences;
		std::string ideration;

		// Initial variables for each ray
		float dndz;
		float rAmplitude;
		float n_snow;
		float dx,dy,dz;
		float yangle;
		float theTime;
		float previousAngle;
		bool InAir;
		int stepNum;
		bool indexFit;
		bool surfaceReflection;
		//Constructor
		Propagator() : _globalTime(0.0), _timeStep(0.5), _isInitialized(false){};
		void Initialize(char**);
		void InitializePropagator(float,std::vector<float>); //See source file.
		void InitializeIce(std::string); //See source file.
		void AddReflector(std::vector<float>,float); //See source file.
		void InitializeCollector();
		void ReflectionSmoothness(float);
		void Propagate(int); //Propagate ray through medium
		void ReadoutPath(int); //Save path in an output file.
		void ReadoutAngles(); //Save path in an output file.
		float Reflect(std::vector<float>);
		void CheckForReflection();
};

#endif
