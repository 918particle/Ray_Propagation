#include <utility>
#include <vector>

#ifndef REFLECTOR_H_
#define REFLECTOR_H_

class Reflector
{
	public:
	Reflector()
	{
		_data.clear();
	};
	std::vector<std::pair<float,float> > _data; //structure of depths and reflection coefficients (meters and unitless)
	void CreateReflector(float,float); //Add a reflection surface
};

#endif
