#include "Emitter.h"
#include "Ice.h"
#include <utility>

#ifndef RFRAY_H_
#define RFRAY_H_
class RFRay : public Emitter, public Ice
{
	public:
	RFRay() : _distanceTravelled(0.0), _rfLoss(0.0), _freq(0.0){};
	void SetFreq(float);
	void Update(std::pair<float,float>); //Updates _distanceTravelled and _rfLoss, after changing the position
	//Variables
	std::pair<float,float> _currentPosition; //Current position in (x,z)
	std::pair<float,float> _priorPosition; //Current position in (x,z)
	float _distanceTravelled; //Keeps track of the propagation distance
	float _rfLoss; //Amount of attenuation (apart from distance) accumulated
	float _freq; //Frequency of the ray
};

#endif