#include "RFRay.h"
#include <cmath>

void RFRay::SetFreq(float f)
{
	_freq = f;
}

void RFRay::Update(float dx,float dy,float dz,float a)
{
	_priorPosition = _currentPosition;
	_currentAngle += a;
	_currentPosition.at(0) += dx;
	_currentPosition.at(1) += dx;
	_currentPosition.at(2) += dz;
	_distanceTravelled += sqrt((_currentPosition.at(0)-_priorPosition.at(0))*(_currentPosition.at(0)-_priorPosition.at(0))
		+(_currentPosition.at(2)-_priorPosition.at(2))*(_currentPosition.at(2)-_priorPosition.at(2)));
	_rfLoss = 0;//exp(-_distanceTravelled/this->GetAttenuationLength(_currentPositionat.at(2)));
	_currentAmplitude=this->_emitterAmplitude/_distanceTravelled*_rfLoss;
}
