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
		_reflectorTypes.clear();
	};
	float _range; //If the ray is within _range meters of the depth of some reflector, introduce a reflection
	std::vector<std::pair<float,float> > _data; //Structure of depths and reflection coefficients (meters and unitless)
	std::vector<float > _reflectorTypes; // Gaussian STD
	void CreateReflector(std::pair<float,float>,float); //Add a reflection surface
	float CheckForAReflection(float&,float,std::vector<float>,float); //angle (radians), depth (meters), polarization vector (unit-less), range
	float RandomGauss(float); //For random numbers drawn from Gaussian distribution with Box-Muller method (0-radian mean, float stddev)
};

#endif
