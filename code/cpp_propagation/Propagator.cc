#include "Propagator.h"
#include "RFRayTracker.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>

//Initialize base classes with data, choose models
void Propagator::InitializePropagator
(
float t, //Global time tracked in nanoseconds
std::pair<float,float> ice_d, //dimensions of the ice (r,z) coordinates
std::pair<bool,bool> pref, //Preferences for index and attenuation length treatment
std::string ice_model, //The name of the ice model for index vs. depth
std::pair<float,float> pos, //Position of the underlying emitter: (r,z) coordinates
float angle_em, //The initial angle of the emitter in degrees, with respect to horizontal
std::vector<float> p //The polarization vector of the emitter
)
{
	this->_path.clear();
	_globalTime = t;
	this->CreateIce(ice_d,pref.first,pref.second,ice_model);
	this->InitializeEmitter(pos,angle_em,p);
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

//Propagation from t=0 to the global time.  This makes it trivial to run
// the same propagation effect for different times with the same settings.
void Propagator::Propagate(int tag)
{
	float pi = 3.14159;
	float total_internal_reflection_tolerance = 0.1; //To prevent unphysical answers, units of meters
	float c0 = 0.299792458; //speed of light in vacuum, meters per nanosecond
	float z0 = 1.0e-4; //units: meters
	float dndz = 0.0; //units: meters^(-1)
	float currentReflection = 0.0;
	float n_snow = 1.5;
	float theTime,dx,dz,dTheta; //units: nanoseconds, meters, meters, radians
	this->_path.push_back(_emitterPosition);
	this->_currentPosition = _emitterPosition;
	this->_currentAngle = _initialAngle;
	RFRayTracker *T = new RFRayTracker(_initialAngle,_currentPosition);
	while(theTime<_globalTime)
	{
		float n = GetIndex(_currentPosition.second);
		theTime+=_timeStep;
		dx=cos(this->_currentAngle)*_timeStep*c0/n;
		dz=sin(this->_currentAngle)*_timeStep*c0/n;
		if(std::abs(this->_currentPosition.second)<total_internal_reflection_tolerance) //Only applying to the surface right now
		{
			if(this->_currentAngle<(pi/2.0-asin(1.0/n))/n_snow)
			{
				_tir = true;
				this->_currentAngle = 0.0; //JCH: June 20th, 2017.  This doesn't have to remain this way.
			}
		}
		if(dz>z0)
		{
			dndz = (GetIndex(this->_currentPosition.second+dz)-GetIndex(this->_currentPosition.second))/dz;
		}
		else
		{
			dndz = (GetIndex(this->_currentPosition.second)-GetIndex(this->_currentPosition.second-z0))/(z0);
		}
		dTheta = _timeStep*cos(this->_currentAngle)*dndz*c0/(n*n);
		this->Update(dx,dz,dTheta);
		this->_path.push_back(_currentPosition);
		float unreflected_angle = _currentAngle; // Used to check if ray got reflected
		float n_i = GetIndex(_currentPosition.second - z0);
		float n_f = GetIndex(_currentPosition.second + z0);
		if((currentReflection = CheckForAReflection(this->_currentAngle,this->_currentPosition.second,this->_polarization,dz,n_i,n_f)))
		{
			T->StoreNewReflection(std::pair<float,float>(),_currentPosition.second,currentReflection);
		}
		if(unreflected_angle != _currentAngle) // Insures same reflector doesn't reflect a single ray twice in a row
		{
			_currentPosition.second -= dz; // This forces the next CheckForReflection to not be true at same reflecter that just reflected
		}                                  // Logic necessary for when dz causes ray to pass the reflector before being reflected
		this->_currentAmplitude*=currentReflection;
		//CheckForAReflection(_currentAngle,_currentPosition.second,this->_polarization,dz)
	}
	T->StoreFinalData(this->_currentAngle,_currentPosition);
	std::stringstream ss;
	ss<<tag;
	T->ReadoutTracker("data/report_"+ss.str()+".dat");
	delete T;
}
