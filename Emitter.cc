#include "Emitter.h"

void Emitter::InitializeEmitter(float x, float z, float angle)
{
	_initialAngle = angle*3.14159/180.0;
	_emitterPosition.first = x;
	_emitterPosition.second = z;
}
