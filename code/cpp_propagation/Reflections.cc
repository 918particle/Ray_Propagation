#include "Reflections.h"

std::pair<float,float> Reflections::index(std::vector<float> currentPosition,float dz,bool indexFit)
{
	if(dz<0)
	{
		n_i = Ice::GetIndex(currentPosition[2] + dz,indexFit);
		n_f = Ice::GetIndex(currentPosition[2] - dz,indexFit);
		n_i_fit = std::abs(Ice::GetIndex(currentPosition[2] + dz,true) - Ice::GetIndex(currentPosition[2] + dz,indexFit));
		n_f_fit = std::abs(Ice::GetIndex(currentPosition[2] - dz,true) - Ice::GetIndex(currentPosition[2] - dz,indexFit));
	}
	else
	{
		n_i = Ice::GetIndex(currentPosition[2] - dz,indexFit);
		n_f = Ice::GetIndex(currentPosition[2] + dz,indexFit);
		n_i_fit = std::abs(Ice::GetIndex(currentPosition[2] - dz,true) - Ice::GetIndex(currentPosition[2] - dz,indexFit));
		n_f_fit = std::abs(Ice::GetIndex(currentPosition[2] + dz,true) - Ice::GetIndex(currentPosition[2] + dz,indexFit));

	}
	n_i = n_i_fit * n_f_fit * n_i;  
	n_f = n_f_fit * n_i_fit * n_f; 
	std::pair<float,float> indexValues;
	indexValues.first = n_i;
	indexValues.second = n_f;
	return indexValues;
}

bool Reflections::TIR(std::vector<float> currentPosition, float &currentAngle, float dz, bool indexFit,float sigma, int stepNum,int scatterLength)
{
	float pi_2 = pi/2.0;
	std::pair<float,float> n = index(currentPosition,0.5,indexFit);
	float s = n.first/n.second;
	//    TIR
	if(std::abs(s*sin(pi_2 - currentAngle))>1.0)
	{
		if(stepNum % scatterLength != 0)
		{
			currentAngle = -currentAngle;
		}
		else
		{
			currentAngle = -currentAngle + RandomGauss(sigma);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Reflections::Reflect(std::vector<float> currentPosition, std::vector<float> polarization, float &currentAngle, float &currentAmplitude, float dz, bool indexFit,float sigma, int stepNum,int scatterLength)
{

	float pi_2 = pi/2.0;
	float distance = std::abs(dz/2) + 0.1;
	std::pair<float,float> n = index(currentPosition,distance,indexFit);
	float s = n.first/n.second;
	//s-polarized component
	float a = std::abs(s*cos(pi_2 - currentAngle));
	float b = sqrt(1.0-(s*s*sin(pi_2 - currentAngle)*sin(pi_2 - currentAngle)));
	float rs = (a-b)/(a+b); //reflected s-pol. relative amplitude (Jackson 7.39)
	float ts = (2*a)/(a+b);	//transmitted s-pol. relative amplitude	(Jackson 7.39)	
	//p-polarized component
	float c = std::abs(cos(pi_2 - currentAngle));
	float d = s*sqrt(1.0-(s*s*sin(pi_2 - currentAngle)*sin(pi_2 - currentAngle)));
	float rp = (c-d)/(c+d); //reflected p-pol. relative amplitude (Jackson 7.41)
	float tp = (2*s*c)/(c+d); //transmitted p-pol. relative amplitude (Jackson 7.41)
	//Reflection and Transmission coefficients
	float R = rs*rs+rp*rp;
	float T = (b/a)*(ts*ts+tp*tp);
	float random = float(rand())/float(RAND_MAX);
	///   Reflection
	if(random<(R)) //Account for reflection coefficient
	{
		if(stepNum % scatterLength != 0)
		{
			currentAngle = -currentAngle;
			return true;
		}	
		else
		{
			float s_amp = currentAmplitude*polarization[2]*rs;
			float p_amp = currentAmplitude*sqrt(polarization[0]*polarization[0]+polarization[1]*polarization[1])*rp;
			currentAmplitude *= sqrt(s_amp*s_amp+p_amp*p_amp); //amplitude of reflected ray
			float ran = RandomGauss(sigma);
			currentAngle = -currentAngle+ran;
			//3DCODEyangle += RandomGauss(_sigma);
		}
	return false;
	}
}

float Reflections::RandomGauss(float stddev)
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
