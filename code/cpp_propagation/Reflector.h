#include <utility>
#include <vector>
#include "Ice.h"

#ifndef REFLECTOR_H_
#define REFLECTOR_H_

class Reflector
{
	public:
	Reflector() : _range(0.1)
	{
		_data.clear();
	};
	float _range; //If the ray is within _range meters of the depth of some reflector, introduce a reflection
	std::vector<std::pair<float,float> > _data; //structure of depths and reflection coefficients (meters and unitless)
	void CreateReflector(std::pair<float,float>); //Add a reflection surface
	void CheckForAReflection(float&,float); //If the ray-depth is near a reflector, change the angle (specularly)
};

#endif
