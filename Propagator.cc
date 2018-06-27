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
		if(_surfaceTIR)
		{
			if(_currentAngle==0.0)
			{
				this->Update(_timeStep*c0/this->_iceBoundaryIndex,0,0);
			}
			else if(_currentAngle==pi)
			{
				this->Update(-1.0*_timeStep*c0/this->_iceBoundaryIndex,0,0);
			}
			theTime+=_timeStep;
			continue;
		}
		float n = GetIndex(_currentPosition.second);
		theTime+=_timeStep;
		//If the ray is near the surface, and close to the critical angle, horizontal propagation (increasing y)
		if(std::abs(_currentPosition.second)<this->_reflectorRange &&
			std::abs( (pi/2-_currentAngle) - pi*atan(1.0/_iceBoundaryIndex) )<=_tirRange && _currentAngle<=pi/2)
		{
			_surfaceTIR = true;
			_currentAngle = 0.0;
			this->Update(_timeStep*c0/this->_iceBoundaryIndex,0,0);
			std::cout<<(pi/2-_currentAngle)<<" "<<pi*atan(1.0/_iceBoundaryIndex)<<std::endl;
		}
		//If the ray is near the surface, and close to the critical angle, horizontal propagation (decreasing y)
		else if(std::abs(_currentPosition.second)<this->_reflectorRange && 
			std::abs( (pi/2-(pi-_currentAngle)) - pi*atan(1.0/_iceBoundaryIndex) )<=_tirRange && _currentAngle>pi/2)
		{
			_surfaceTIR = true;
			_currentAngle = pi;
			this->Update(-1.0*_timeStep*c0/this->_iceBoundaryIndex,0,0);
		}
		// //If the ray is near the surface, and greater than the critical angle, reflection (increasing y)
		// else if(std::abs(_currentPosition.second)<this->_reflectorRange &&
		// 	( (pi/2-_currentAngle) - pi*atan(1.0/_iceBoundaryIndex) )>_tirRange && _currentAngle<=pi/2 && !_surfaceTIR)
		// {
		// 	_currentAngle = -1.0*_currentAngle;
		// 	dy=cos(this->_currentAngle)*_timeStep*c0/n;
		// 	dz=sin(this->_currentAngle)*_timeStep*c0/n;
		// 	dndz = (GetIndex(this->_currentPosition.second+dz)-GetIndex(this->_currentPosition.second))/dz;
		// 	dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
		// 	this->Update(dy,dz,dTheta);
		// }
		// //If the ray is near the surface, and greater than the critical angle, reflection (decreasing y)
		// else if(std::abs(_currentPosition.second)<this->_reflectorRange && 
		// 	( (pi/2-(pi-_currentAngle)) - pi*atan(1.0/_iceBoundaryIndex) )>_tirRange && _currentAngle>pi/2 && !_surfaceTIR)
		// {
		// 	_currentAngle = -1.0*_currentAngle;
		// 	dy=cos(this->_currentAngle)*_timeStep*c0/n;
		// 	dz=sin(this->_currentAngle)*_timeStep*c0/n;
		// 	dndz = (GetIndex(this->_currentPosition.second+dz)-GetIndex(this->_currentPosition.second))/dz;
		// 	dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
		// 	this->Update(dy,dz,dTheta);
		// }
		//Normal propagation
		else
		{
			dy=cos(this->_currentAngle)*_timeStep*c0/n;
			dz=sin(this->_currentAngle)*_timeStep*c0/n;
			dndz = (GetIndex(this->_currentPosition.second+dz)-GetIndex(this->_currentPosition.second))/dz;
			dTheta = _timeStep*cos(_currentAngle)*dndz*c0/(n*n);
			this->Update(dy,dz,dTheta);
			CheckForAReflection(_currentAngle,_currentPosition.second,this->_polarization);
		}
	}
}

void Propagator::SetGlobalTimeAndStep(float a,float b)
{
	_globalTime = a;
	_timeStep = b;
}