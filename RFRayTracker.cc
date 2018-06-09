#include "RFRayTracker.h"
#include <fstream>

void RFRayTracker::StoreNewReflection(std::pair<float,float> angles,float d,float r)
{
	_depthsOfReflections.push_back(d);
	_intialFinalAnglesDuringReflections.push_back(angles);
	_reflectionCoefficients.push_back(r);
	++_numberOfReflections;
}
void RFRayTracker::StoreFinalData(float angle,std::pair<float,float> pos)
{
	_finalAngle = angle;
	_finalDepth = pos.second;
	_finalRange = pos.first;
}
void RFRayTracker::ReadoutTracker(std::string title)
{
	float pi = 3.14159;
	std::ofstream out(title.c_str());
	//Print initial state
	out<<"Initial angle (deg)...Initial depth (m): ";
	out<<_initialAngle_tracking*180.0/pi<<" "<<_initialDepth<<std::endl;
	//Print all reflections
	out<<"Total number of reflections: "<<_numberOfReflections<<std::endl;
	out<<"Initial angle (deg)...Final angle (deg)...Depth (m)...Coefficient: ";
	for(int i=0;i<_numberOfReflections;++i)
	{
		out<<i<<": "<<_intialFinalAnglesDuringReflections[i].first*180.0/pi<<" "<<_intialFinalAnglesDuringReflections[i].second*180.0/pi;
		out<<" "<<_depthsOfReflections[i]<<" "<<_reflectionCoefficients[i]<<std::endl;
	}
	//Print final state
	out<<"Final angle (deg)...Final depth (m): "<<_finalAngle*180.0/pi<<" "<<_finalDepth<<std::endl;
	out.close();
}