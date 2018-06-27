#include "Propagator.h"
#include <fstream>
#include <iostream>

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
	float c0 = 0.299792458; //speed of light in vacuum, meters per nanosecond
	float dndz = 0.0; //units: meters^(-1)
	float theTime=0.0; //Units: nanoseconds
	float dy,dz,dTheta; //units: meters, meters, radians
	this->_path.push_back(_emitterPosition);
	this->_currentPosition = _emitterPosition;
	this->_currentAngle = _emitterInitialAngle;
	while(theTime<_globalTime)
	{
		float n = this->GetIndex(_currentPosition.second);
		dy=cos(_currentAngle)*_timeStep*c0/n;
		dz=sin(_currentAngle)*_timeStep*c0/n;
		dndz = (GetIndex(_currentPosition.second+dz)-GetIndex(_currentPosition.second))/dz;
		dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
		this->Update(dy,dz,dTheta);
		CheckForAReflection(_currentAngle,_currentPosition.second,_polarization);
		theTime+=_timeStep;
	}
}

void Propagator::SetGlobalTimeAndStep(float a,float b)
{
	_globalTime = a;
	_timeStep = b;
}