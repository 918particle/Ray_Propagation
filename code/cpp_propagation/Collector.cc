#include "Collector.h"

void Collector::CreateCollectors(float t, int n, float a, std::vector<float> pos,float d)
{ 
	angle = t*3.14/180.0;
	num_phot = n;
	area = a;
	position = pos;
	seperation = d;
	for(int i = 0; i < num_phot; i++)
	{
		flux.push_back(0);
	}
}


void Collector::Collect(std::vector<float> currentPosition,std::vector<float> previousPosition)
{
	float PhotometerAngle = angle;
	int NumberOfPhotometers = num_phot;
	float PhotometerArea = area;
	float PhotometerX = position[0];
	float PhotometerZ = position[2];
	float PhotometerSeperation = seperation;

	for(int j = 0; j < num_phot; j++)
	{			
		float PhotometerXAtJ = PhotometerX + (j*PhotometerSeperation*cos(PhotometerAngle));
		float PhotometerZAtJ = PhotometerZ + (j*PhotometerSeperation*sin(PhotometerAngle));
		float xBeforePhotometer = previousPosition[0];
		float xAfterPhotometer = currentPosition[0];
		float zBeforePhotometer = previousPosition[2];
		float zAfterPhotometer = currentPosition[2];
		if(xAfterPhotometer != xBeforePhotometer)
		{
			float slope = (zAfterPhotometer - zBeforePhotometer)/(xAfterPhotometer - xBeforePhotometer);
			float zPositionAtPhotometer = (slope * (PhotometerXAtJ - xBeforePhotometer)) + zBeforePhotometer;
			if(zPositionAtPhotometer < (PhotometerZAtJ + PhotometerArea/2) && zPositionAtPhotometer > (PhotometerZAtJ - PhotometerArea/2))
			{
				flux[j] = flux[j] + 1;	
			}
		}
	}
}

void Collector::ReadoutPath()
{
	std::string title ="data/flux.dat";
	std::ofstream out(title.c_str());
	std::vector<float>::iterator i = flux.begin();
	while(i!=flux.end())
	{
		out<<(*i)<<std::endl;
		++i;
	}
	out.close();
	flux.clear();
}
