#pragma once
#include "Ice.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "Constants.h"
#include <string>

class Refractions : public Ice
{
	public:
	Refractions(){};
	float n_i;
	float n_f;
	float n_i_fit;
	float n_f_fit;
	float dndz;
	float dTheta;
	float Refract(std::vector<float>,float&,float,bool,float,int,int,float,float,bool,bool);
};
