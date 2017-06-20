#include "Reflector.h"
#include <cmath>
#include <cstdlib>

void Reflector::CreateReflector(std::pair<float,float> x,std::pair<int,float> y)
{
	_data.push_back(x);
	_reflectorTypes.push_back(y);
}

float Reflector::CheckForAReflection(float &alpha,float z,std::vector<float> p)
{
	float currentAmplitude = 1.0;
	std::vector<std::pair<float,float> >::iterator i = _data.begin();
	std::vector<std::pair<int,float> >::iterator j = _reflectorTypes.begin();
	while(i!=_data.end())
	{
		if(std::abs((*i).first-z)<_range) //Within range of reflector
		{
			float r = (*i).second; //The maximum value of the reflection coefficient (theta = 0)
			float s = (sqrt(r)-1.0)/(sqrt(r)+1.0); //The ratio of the two indices of refraction
			//s-polarized component
			float a = s*cos(alpha);
			float b = sqrt(1.0-(s*s*sin(alpha)*sin(alpha)));
			float rs = (a-b)/(a+b); //reflected s-pol. relative amplitude (Jackson 7.39)
			//rs = rs*sqrt(p[2]*p[2]); //multiply by fraction of incomming wave that is in this polarization direction
			float ts = (2*a)/(a+b);	//transmitted s-pol. relative amplitude	(Jackson 7.39)	
			//p-polarized component
			float c = cos(alpha);
			float d = s*sqrt(1.0-(s*s*sin(alpha)*sin(alpha)));
			float rp = (c-d)/(c+d); //reflected p-pol. relative amplitude (Jackson 7.41)
			//rp = rp*sqrt(p[0]*p[0]+p[1]*p[1]); //multiply by fraction of incomming wave that is in this polarization direction
			float tp = (2*s*c)/(c+d); //transmitted p-pol. relative amplitude (Jackson 7.41)
			//Reflection and Transmission coefficients
			float R = rs*rs+rp*rp;
			float T = (b/a)*(ts*ts+tp*tp);
			if(float(rand())/float(RAND_MAX)<(R)) //Account for reflection coefficient
			{
				float s_amp = currentAmplitude*p[2]*rs;
				float p_amp = currentAmplitude*sqrt(p[0]*p[0]+p[1]*p[1])*rp;
				currentAmplitude = sqrt(s_amp*s_amp+p_amp*p_amp); //amplitude of reflected ray
				if((*j).first == 1) //Specular case
				{
					alpha = -alpha;

				}
				if((*j).first == 2) //Diffuse Lambertian case
				{
					alpha = -alpha+RandomGauss((*j).second);
				}


			}
		}
		++i;
		++j;
	}
	return currentAmplitude;
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
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;
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
