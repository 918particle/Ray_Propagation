#include "RFRay.h"
#include <vector>
#include <utility>
#include <string>

#ifndef RFRAYTRACKER_H_
#define RFRAYTRACKER_H_
class RFRayTracker
{
	public:
		RFRayTracker(float initAng,std::pair<float,float> initP)
		{
			_initialAngle_tracking = initAng;
			_initialRange = initP.first;
			_initialDepth = initP.second;
		};
		void StoreNewReflection(std::pair<float,float>,float,float);
		void StoreFinalData(float,std::pair<float,float>);
		void ReadoutTracker(std::string);
		int _numberOfReflections;
		float _initialAngle_tracking;
		float _finalAngle;
		float _initialDepth;
		float _finalDepth;
		float _initialRange;
		float _finalRange;
		std::vector<std::pair<float,float> > _intialFinalAnglesDuringReflections;
		std::vector<float> _depthsOfReflections;
		std::vector<float> _reflectionCoefficients;
};

#endif
