#include "Emitter.h"

void Emitter::InitializeEmitter(float y,float z,float angle)
{
	_emitterInitialAngle = angle*3.14159/180.0;
	_emitterPosition.first = y;
	_emitterPosition.second = z;
}
