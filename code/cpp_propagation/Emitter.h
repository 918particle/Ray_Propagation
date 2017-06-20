#include <utility>
#include "Polarization.h"

#ifndef EMITTER_H_
#define EMITTER_H_
class Emitter : public Polarization
{
	public:
	Emitter() : _initialAngle(0.0)
	{
		_emitterPosition.first = 0.0;
		_emitterPosition.second = 0.0;
	}
	float _initialAngle; //initial angle with respect to horizontal, entered as degrees
	std::pair<float,float> _emitterPosition; //(x,z) position
	float _emitterAmplitude;
	void InitializeEmitter(std::pair<float,float>,float,std::vector<float>,float); //set emitter x, z, initial angle, polarization, and amplitude
};

#endif
