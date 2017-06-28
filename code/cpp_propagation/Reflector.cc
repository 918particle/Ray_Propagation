#include "Reflector.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h> // For printf function

void Reflector::CreateReflector(std::pair<float,float> x,float y)
{
	_data.push_back(x);
	_reflectorTypes.push_back(y);
}

float Reflector::CheckForAReflection(float &alpha,float z,std::vector<float> p, float range)
{
	float currentAmplitude = 1.0;
	std::vector<std::pair<float,float> >::iterator i = _data.begin();
	std::vector<float>::iterator j = _reflectorTypes.begin();
	bool TIR = false;
	while(i!=_data.end())
	{
		if(std::abs((*i).first-z)<std::abs(range)) //Within range of reflector
		{
			float pi_2 = 3.14159/2.0;
			float r = (*i).second; //The maximum value of the reflection coefficient (theta = 0)
			float s = (sqrt(r)-1.0)/(sqrt(r)+1.0);
			//s-polarized component
			float a = std::abs(s*cos(pi_2 - alpha));
			float b = sqrt(1.0-(s*s*sin(pi_2 - alpha)*sin(pi_2 - alpha)));
			float rs = (a-b)/(a+b); //reflected s-pol. relative amplitude (Jackson 7.39)
			float ts = (2*a)/(a+b);	//transmitted s-pol. relative amplitude	(Jackson 7.39)	
			//p-polarized component
			float c = std::abs(cos(pi_2 - alpha));
			float d = s*sqrt(1.0-(s*s*sin(pi_2 - alpha)*sin(pi_2 - alpha)));
			float rp = (c-d)/(c+d); //reflected p-pol. relative amplitude (Jackson 7.41)
			float tp = (2*s*c)/(c+d); //transmitted p-pol. relative amplitude (Jackson 7.41)
			//Reflection and Transmission coefficients
			float R = rs*rs+rp*rp;
			float T = (b/a)*(ts*ts+tp*tp);
			if((s*s*sin(pi_2 - alpha)*sin(pi_2 - alpha))>1.0)
			{
				TIR = true;
				alpha = -alpha;
			}

			if((float(rand())/float(RAND_MAX)<(R)) && !TIR) //Account for reflection coefficient
			{
				float s_amp = currentAmplitude*p[2]*rs;
				float p_amp = currentAmplitude*sqrt(p[0]*p[0]+p[1]*p[1])*rp;
				currentAmplitude = sqrt(s_amp*s_amp+p_amp*p_amp); //amplitude of reflected ray
				alpha = -alpha+RandomGauss(*j);
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
