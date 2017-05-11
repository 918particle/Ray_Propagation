#include "Ice.h"
#include <cmath>

void Ice::CreateIce(float r,float z,bool fitn,bool fitl,std::string modelName)
{
	_r = r;
	_z = z;
	_useIndexFit = fitn;
	_useAttenuationLengthFit = fitl;
	if(_useIndexFit)
	{
		if(modelName=="SPICE")
		{
			_A = 1.78;
			_B = 0.427;
			_C = 0.014;
		}
		else if(modelName=="Byrd")
		{
			_A = 1.78;
			_B = 0.464;
			_C = 0.0244;
		}
	}
}

float Ice::GetIndex(float z)
{
	if(z>0)
	{
		return 1.0;
	}
	else
	{
		return _A-_B*exp(_C*z);
	}
}
