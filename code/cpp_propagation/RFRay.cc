#include "RFRay.h"
#include <cmath>

void RFRay::SetFreq(float f)
{
	_freq = f;
}

void RFRay::Update(std::pair<float,float> q,float a)
{
	_priorPosition = _currentPosition;
	_currentAngle = a;
	_currentPosition = q;
	float change = sqrt((_currentPosition.first-_priorPosition.first)*(_currentPosition.first-_priorPosition.first)
		+(_currentPosition.second-_priorPosition.second)*(_currentPosition.second-_priorPosition.second));
	_distanceTravelled += change;
	_rfLoss *= exp(-change/this->GetAttenuationLength(_currentPosition.second));
}