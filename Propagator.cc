#include "Propagator.h"
#include <fstream>
#include <iostream>

//Initialize base classes with data, choose models
void Propagator::InitializePropagator(float y,float z,float angle)
{
	this->_path.clear();
	this->InitializeEmitter(y,z,angle);
	_isInitialized = true;
	float surface_coefficient = std::abs((_iceBoundaryIndex-1.0)/(_iceBoundaryIndex+1.0));
	this->CreateReflector(std::pair<float,float>(0.0,surface_coefficient),std::pair<bool,float>(false,0.01));
	this->SetReflectorRange(0.15);
}

void Propagator::AddReflector(std::pair<float,float> x,std::pair<bool,float> y)
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
		float n = GetIndex(_currentPosition.second);
		dy=cos(_currentAngle)*_timeStep*c0/n;
		dz=sin(_currentAngle)*_timeStep*c0/n;
		if(_currentPosition.second>0) dndz = 0.0;
		else dndz = (GetIndex(_currentPosition.second+dz)-GetIndex(_currentPosition.second))/dz;
		dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
		Update(dy,dz,dTheta);
		theTime+=_timeStep;
		CheckForAReflection(_currentAngle,_currentPosition.second,_polarization);
	}
}

void Propagator::SetGlobalTimeAndStep(float a,float b)
{
	_globalTime = a;
	_timeStep = b;
}

bool Propagator::CanReflect()
{
	if(_currentPosition.second>0) return false;
	else
	{
		std::vector<std::pair<float,float> >::iterator i;
		for(i=_data.begin();i!=_data.end();++i)
		{
			if(std::abs(_currentPosition.second - (*i).first)<=_reflectorRange && 
				std::abs(_priorPosition.second - (*i).first)>_reflectorRange)
			{
				return true;
			}
		}
		return false;
	}
}