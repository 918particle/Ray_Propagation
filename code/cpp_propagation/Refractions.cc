#include "Refractions.h"

float Refractions::Refract(std::vector<float> currentPosition, float &currentAngle, float dz, bool indexFit, float sigma, int stepNum, int scatterLength,float timeStep, float n, bool surfaceReflection, bool InAir)
{
	dndz = 0.0; //units: meters^(-1)
	if(std::abs(dz)>z0)
	{
		dndz = (GetIndex(currentPosition[2]+(dz/2),indexFit)-GetIndex(currentPosition[2]-(dz/2),indexFit))/dz;
	}
	else
	{
		dndz = (GetIndex(currentPosition[2],indexFit)-GetIndex(currentPosition[2]-z0,indexFit))/(z0);
	}
	dTheta = 0;
	if(!InAir)
	{
		if((stepNum % scatterLength) == 0 && currentPosition[2] > -200.0)
		{
			dTheta = timeStep*cos(currentAngle)*dndz*c0/(n*n);// + (RandomGauss(sigma));
		}
		else
		{
			dTheta = timeStep*cos(currentAngle)*dndz*c0/(n*n);				
		}
	}
	if(dz>0 && surfaceReflection)
	{
		dTheta = 0;
	};
	return dTheta;
}