#include "Reflector.h"
#include <cmath>
#include <cstdlib>

void Reflector::CreateReflector(float z,float r)
{
	//That's the depth, and the reflection coefficient
	_data.push_back(std::pair<float,float>(z,r));
}

bool Reflector::CheckForAReflection(float &alpha,float z, int scattering_type, double diffuse_angle)
{
	std::vector<std::pair<float,float> >::iterator i = _data.begin();
	bool ray_refracted = false;
	while(i!=_data.end())
	{
		if(std::abs((*i).first-z)<_range)
		{
			if(float(rand())/float(RAND_MAX)<(*i).second)
			{
				if(alpha>0.0 && alpha<=90.0*3.14159/180.0)
				{
					if(scattering_type == 1) // Specular
					{
						alpha = -alpha;
						ray_refracted = false;
					}
					if(scattering_type == 2) // diffuse lambertian
					{
						alpha = -diffuse_angle*3.14159/180.0;
						ray_refracted = false;
					}
				}
				else
				{
					ray_refracted = true;
				}
			}
		}
		++i;
	}
	return ray_refracted;
}
