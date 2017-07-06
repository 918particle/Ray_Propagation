#include "Reflector.h"

#ifndef REFLECTION_H_
#define REFLECTION_H_

class Reflection : public Reflector
{
	public:
	Reflection() : _smoothness(0.0){};
	float _smoothness; // Gaussian STD
	void Smoothness(float); // For andling STD of Gaussian
	float ReflectionOrRefraction(float&,float,std::vector<float>,float, float, float); //angle (radians), depth (meters), polarization vector (unit-less), range, n_i, n_f
};

#endif
