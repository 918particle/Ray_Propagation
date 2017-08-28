#include <utility>
#include "Polarization.h"

#ifndef EMITTER_H_
#define EMITTER_H_
class Emitter : public Polarization
{
	public:
	Emitter() : _initialAngle(0.0), _emitterAmplitude(1.0)
	{
		_emitterPosition = {0.0,0.0,0.0};
	}
	float _initialAngle; //initial angle with respect to horizontal, entered as degrees
	std::vector<float> _emitterPosition; //(x,z) position
	float _emitterAmplitude;
	void InitializeEmitter(std::vector<float>,float,std::vector<float>); //set emitter x, z, initial angle, polarization
};

#endif
