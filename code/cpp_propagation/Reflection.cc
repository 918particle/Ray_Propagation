#include "Reflection.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h> // For printf function

void Reflection::Smoothness(float smoothness)
{
	_smoothness = smoothness; //Gaussian STD
}

float Reflection::ReflectionOrRefraction(float &alpha,float z,std::vector<float> p, float range, float n_i, float n_f)
{
	float currentAmplitude = 1.0;
	bool TIR = false; //Total internal reflection
	float pi_2 = 3.14159/2.0;
	float s = std::abs(n_i/n_f);
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
	if((s*s*sin(pi_2 - alpha)*sin(pi_2 - alpha))>1.0) //Check for total internal reflection
	{
		TIR = true;
	}

	if((float(rand())/float(RAND_MAX)<(R)) || TIR) //Account for reflection coefficient
	{
		float s_amp = currentAmplitude*p[2]*rs;
		float p_amp = currentAmplitude*sqrt(p[0]*p[0]+p[1]*p[1])*rp;
		currentAmplitude = sqrt(s_amp*s_amp+p_amp*p_amp); //amplitude of reflected ray
		alpha = -alpha+Reflector::RandomGauss(_smoothness);
	}
	return currentAmplitude;
}
