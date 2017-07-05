#include "RFRayTracker.h"
#include <fstream>

void RFRayTracker::StoreNewReflection(std::pair<float,float> angles,std::pair<float,float> positions,float r)
{
	_depthsOfReflections.push_back(positions);
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
	out<<"Initial angle (deg)...Inital Range (m)...Initial depth (m): ";
	out<<_initialAngle_tracking*180.0/pi<<" "<<_initialRange<<" "<<_initialDepth<<std::endl;
	//Print all reflections
	out<<"Total number of reflections: "<<_numberOfReflections<<std::endl;
	out<<"Initial angle (deg)...Final angle (deg)...x position (m)...z position (m)...Coefficient: "<<std::endl;
	for(int i=0;i<_numberOfReflections;++i)
	{
		out<<i<<": "<<_intialFinalAnglesDuringReflections[i].first*180.0/pi<<" "<<_intialFinalAnglesDuringReflections[i].second*180.0/pi;
		out<<" "<<_depthsOfReflections[i].first<<" "<<_depthsOfReflections[i].second<<" "<<_reflectionCoefficients[i]<<std::endl;
	}
	//Print final state
	out<<"Final angle (deg)...Final Range (m)...Final depth (m): "<<_finalAngle*180.0/pi<<" "<<_finalRange<<" "<<_finalDepth<<std::endl;
	out.close();
}