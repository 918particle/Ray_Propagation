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

class Collector
{
	public:
	float seperation;
	float radius;
	float angle;
	float area;
	int num_phot;
	std::vector<float> position;
	std::vector<float> flux;
	void CreateCollectors(float,int,float,std::vector<float>,float);
	void Collect(std::vector<float>,std::vector<float>);
	void ReadoutPath();
};