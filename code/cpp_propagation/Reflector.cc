#include "Reflector.h"

void Reflector::CreateReflector(float z,float r)
{
	_data.push_back(std::pair<float,float>(z,r));
}
