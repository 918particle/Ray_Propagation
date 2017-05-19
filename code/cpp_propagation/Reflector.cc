#include "Reflector.h"
#include <cmath>
#include <cstdlib>

void Reflector::CreateReflector(float z,float r)
{
	//That's the depth, and the reflection coefficient
	_data.push_back(std::pair<float,float>(z,r));
}

void Reflector::CheckForAReflection(float &alpha,float z)
{
	std::vector<std::pair<float,float> >::iterator i = _data.begin();
	while(i!=_data.end())
	{
		if(std::abs((*i).first-z)<_range)
		{
			if(float(rand())/float(RAND_MAX)<(*i).second)
			{
				if(alpha>0.0 && alpha<=90.0*3.14159/180.0)
				{
					alpha = -alpha;
				}
			}
		}
		++i;
	}
}
