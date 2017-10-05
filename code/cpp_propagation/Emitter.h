#include <utility>
#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>

#ifndef EMITTER_H_
#define EMITTER_H_

class Emitter
{
	public:
	Emitter();
	Emitter(std::vector<float> x,float theta,std::vector<float> p,float a) :
		_emitterPosition(x),
		_initialAngle(theta*3.14159/180.0),
		_polarization(p),
		_emitterAmplitude(a)
		{
			_polarization.resize(3);
			float n = std::inner_product(_polarization.begin(),_polarization.end(),_polarization.begin(),0.0);
			_polarization[0]/=n;
			_polarization[1]/=n;
			_polarization[2]/=n;
		};
	float _initialAngle; //initial angle with respect to horizontal, entered as degrees
	std::vector<float> _emitterPosition; //(x,z) position
	float _emitterAmplitude;
	std::vector<float> _polarization; //The propagation is in the x direction, and z is depth, so polariation is in the y-z plane. 
};

#endif
