#include "Propagator.h"
#include <fstream>

//Initialize base classes with data, choose models
void Propagator::InitializePropagator(float y,float z,float angle)
{
	this->_path.clear();
	this->InitializeEmitter(y,z,angle);
	_isInitialized = true;
}
void Propagator::AddReflector(std::pair<float,float> x,std::pair<int,float> y)
{
	this->CreateReflector(x,y);
}

void Propagator::ReadoutPath(std::string title)
{
	std::ofstream out(title.c_str());
	std::vector<std::pair<float,float> >::iterator i = this->_path.begin();
	while(i!=this->_path.end())
	{
		out<<(*i).first<<" "<<(*i).second<<std::endl;
		++i;
	}
	out.close();
}

void Propagator::Propagate()
{
	float pi = 3.14159;
	float c0 = 0.299792458; //speed of light in vacuum, meters per nanosecond
	float dndz = 0.0; //units: meters^(-1)
	float theTime=0.0; //Units: nanoseconds
	float dy,dz,dTheta; //units: meters, meters, radians
	this->_path.push_back(_emitterPosition);
	this->_currentPosition = _emitterPosition;
	this->_currentAngle = _emitterInitialAngle;
	while(theTime<_globalTime)
	{
		float n = GetIndex(_currentPosition.second);
		theTime+=_timeStep;
		dy=cos(this->_currentAngle)*_timeStep*c0/n;
		dz=sin(this->_currentAngle)*_timeStep*c0/n;
		if(std::abs(this->_currentPosition.second)<this->_reflectorRange)
		{
			if(this->_currentAngle<(pi/2.0-asin(1.0/n))/this->_iceBoundaryIndex)
			{
				_surfaceTIR = true;
				_currentAngle = 0.0; //JCH: June 20th, 2017.
			}
		}
		dndz = (GetIndex(this->_currentPosition.second+dz)-GetIndex(this->_currentPosition.second))/dz;
		dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
		this->Update(dy,dz,dTheta);
		this->_path.push_back(_currentPosition);
		CheckForAReflection(_currentAngle,_currentPosition.second,this->_polarization);
	}
}

void Propagator::SetGlobalTimeAndStep(float a,float b)
{
	_globalTime = a;
	_timeStep = b;
}