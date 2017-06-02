#include "Emitter.h"

void Emitter::InitializeEmitter(std::pair<float,float> pos, float angle)
{
	_initialAngle = angle*3.14159/180.0;
	_emitterPosition = pos;
}
