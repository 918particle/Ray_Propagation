#pragma once
#include "RFRay.h"
#include "Ice.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "Constants.h"
#include "Refractions.h"
#include <string>

class Reflections :  public Refractions
{
	public:
	Reflections()
	{
	};
	float n_i;
	float n_f;
	float n_i_fit;
	float n_f_fit;

	bool TIR(std::vector<float>,float&,float,bool,float,int,int); //angle (radians), depth (meters), polarization vector (unit-less), range
	bool Reflect(std::vector<float>,std::vector<float>,float&,float&,float,bool,float,int,int); //angle (radians), depth (meters), polarization vector (unit-less), range
	std::pair<float,float> index(std::vector<float>,float,bool);
	float RandomGauss(float); //For random numbers drawn from Gaussian distribution with Box-Muller method (0-radian mean, float stddev)
};
