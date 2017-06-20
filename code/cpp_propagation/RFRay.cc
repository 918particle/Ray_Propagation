#include "RFRay.h"
#include <cmath>

void RFRay::SetFreq(float f)
{
	_freq = f;
}

void RFRay::Update(float dx,float dz,float a)
{
	_priorPosition = _currentPosition;
	_currentAngle += a;
	_currentPosition.first += dx;
	_currentPosition.second += dz;
	_distanceTravelled += sqrt((_currentPosition.first-_priorPosition.first)*(_currentPosition.first-_priorPosition.first)
		+(_currentPosition.second-_priorPosition.second)*(_currentPosition.second-_priorPosition.second));
	_rfLoss = exp(-_distanceTravelled/this->GetAttenuationLength(_currentPosition.second));
	_currentAmplitude=this->_emitterAmplitude/_distanceTravelled*_rfLoss;
}
