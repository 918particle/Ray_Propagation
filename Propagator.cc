#include "Propagator.h"
#include <fstream>
#include <cmath>
#include <iostream>

//Initialize base classes with data, choose models
void Propagator::InitializePropagator
(
float t, //Global time tracked in nanoseconds
float r_ice, //Radius of ice block in meters
float z_ice, //Depth of ice block in meters, below surface is negative
bool useFitN, //Use a fit to index data rather than data intperpolation
bool useFitL, //Use a fit to attenuation length data rather than interpolation
std::string ice_model, //The name of the ice model for index vs. depth
float r_em, //The horizontal position of the emitter in meters
float z_em, //The depth of the emitter in meters, below surface is negative
float angle_em //The initial angle of the emitter in degrees, with respect to horizontal
)
{
	this->_path.clear();
	_globalTime = t;
	this->CreateIce(r_ice,z_ice,useFitN,useFitL,ice_model);
	this->InitializeEmitter(r_em,z_em,angle_em);
	_isInitialized = true;
}
void Propagator::AddReflector(float z,float coeff)
{
	this->CreateReflector(z,coeff);
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
void Propagator::Propagate()
{
	float tolerance = 0.02; //To prevent unphysical propagation, treat specical angles
	float c0 = 0.299792458; //speed of light in vacuum, meters per nanosecond
	float dz = 1.0e-4; //units: meters
	float dndz = 0.0; //units: meters^(-1)
	float theTime = 0.0;
	this->_path.push_back(_emitterPosition);
	bool flag = true;
	while(theTime<_globalTime)
	{
		float n = GetIndex(_emitterPosition.second);
    	if(n<1)
    	{
    		std::cout<<"Warning: Index of Refraction < 1. Emitter Position (x,z): ("<<_emitterPosition.first<<","<<_emitterPosition.second <<")"<<std::endl;
    	}
		theTime+=_timeStep;
		std::pair<float,float> old_pos = _emitterPosition;

		_emitterPosition.first+=cos(_initialAngle)*_timeStep*c0/n;
		_emitterPosition.second+=sin(_initialAngle)*_timeStep*c0/n;
		this->_path.push_back(_emitterPosition);
		bool ray_refracted = CheckForAReflection(_initialAngle,_emitterPosition.second,scatteringType,gaussianDist);
		if(std::abs(old_pos.second-_emitterPosition.second)>dz)
		{
			dndz = (GetIndex(_emitterPosition.second)-GetIndex(old_pos.second))/(_emitterPosition.second-old_pos.second);
		}
		else
		{
			dndz = (GetIndex(_emitterPosition.second)-GetIndex(_emitterPosition.second-dz))/(_emitterPosition.second-dz);
		}
		if(scatteringType == 1) //specular reflection
		{
			float dTheta = _timeStep*cos(_initialAngle)*dndz*c0/(n*n);
			//std::cout<<dTheta*180.0/3.14159<<" ";
			if(dTheta>=3.14159/2.0 && dTheta<3.14159) dTheta-=3.14159/2.0;
			else if(dTheta>=3.14159) dTheta-=3.14159;
			//std::cout<<dTheta*180.0/3.14159<<" "<<_initialAngle*180.0/3.14159<<" "<<_initialAngle*180.0/3.14159+dTheta*180.0/3.14159<<std::endl;
			_initialAngle+=dTheta;
		}
		if(scatteringType == 2 && ray_refracted)
		{
			float n_new = GetIndex(_emitterPosition.second);
			_initialAngle = asin((n/n_new)*sin(_initialAngle));
		}
		this->_currentAngle = _initialAngle; //Change this after today, May 19th, 2017.  Let RFRay track the angle, not Emitter.
	}
}
