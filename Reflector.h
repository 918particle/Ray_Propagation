#include <utility>
#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef REFLECTOR_H_
#define REFLECTOR_H_

class Reflector
{
	public:
	std::vector<std::pair<float,float> > _data; //Structure of depths and reflection coefficients (meters and unitless)
	std::vector<std::pair<int,float> > _reflectorTypes; //First: 1) specular 2) Lambertian diffuse Second: Width of scattering angle distribution
	void CreateReflector(std::pair<float,float>,std::pair<int,float>); //Add a reflection surface
	void CheckForAReflection(float&,float,std::vector<float>); //angle (radians), depth (meters), polarization vector (unit-less)
	float RandomGauss(float); //For random numbers drawn from Gaussian distribution with Box-Muller method (0-radian mean, float stddev)
	Reflector() : _reflectorRange(0.1)
	{
		_data.clear();
		_reflectorTypes.clear();
	};
	float _reflectorRange; //If the ray is within _range meters of the depth of some reflector, introduce a reflection
	void SetReflectorRange(float); //Reset the default minimum distance between ray and reflector to trigger CheckForAReflection
	void Nothing(); //Nothing doing.
};

#endif
