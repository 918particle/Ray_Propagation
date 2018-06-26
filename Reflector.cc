#include "Reflector.h"

void Reflector::CreateReflector(std::pair<float,float> x,std::pair<bool,float> y)
{
	_data.push_back(x);
	_reflectorTypes.push_back(y);
}

bool Reflector::CheckForAReflection(float &alpha,float z,std::vector<float> p)
{
	std::vector<std::pair<float,float> >::iterator i = _data.begin();
	std::vector<std::pair<bool,float> >::iterator j = _reflectorTypes.begin();
	bool flag = false;
	while(i!=_data.end())
	{
		if(std::abs((*i).first-z)<_reflectorRange) //Within range of reflector
		{
			float beta = std::abs(alpha);
			beta = 3.14159/2.0-beta;  //Angle with respect to vertical (Fresnel equations)
			float s = (1.0-(*i).second)/(1.0+(*i).second); //The ratio of the two indices of refraction
			//s-polarized component
			float a = s*cos(beta);
			float b = sqrt(1.0-(s*s*sin(beta)*sin(beta)));
			float rs = std::abs((a-b)/(a+b))*std::sqrt(p[0]*p[0]);
			//p-polarized component
			float c = s*sqrt(1.0-(s*s*sin(beta)*sin(beta)));
			float d = cos(beta);
			float rp = std::abs((c-d)/(c+d))*std::sqrt(p[1]*p[1]+p[2]*p[2]);
			if(float(rand())/float(RAND_MAX)<=0.5*(rs+rp)) //Account for reflection coefficient
			{
				if((*j).first == false) //Specular case
				{
					alpha = -alpha;
				}
				if((*j).first == true) //Diffuse Lambertian case
				{
					alpha = -alpha+RandomGauss((*j).second);
				}
				flag=true;
			}
		}
		++i;
		++j;
	}
	return flag;
}

float Reflector::RandomGauss(float stddev)
{
    static float n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        float x, y, r;
        do
        {
            x = 2.0*float(rand())/float(RAND_MAX) - 1;
            y = 2.0*float(rand())/float(RAND_MAX) - 1;
            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            float d = sqrt(-2.0*log(r)/r);
            float n1 = x*d;
            n2 = y*d;
            float result = n1*stddev;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev;
    }
}

void Reflector::SetReflectorRange(float r)
{
	_reflectorRange = r;
}