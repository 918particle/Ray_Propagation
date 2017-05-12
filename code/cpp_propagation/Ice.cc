#include "Ice.h"
#include <cmath>
#include <fstream>
#include <iostream>

void Ice::CreateIce(float r,float z,bool fitn,bool fitl,std::string modelName)
{
	_r = r;
	_z = z;
	_useIndexFit = fitn;
	_useAttenuationLengthFit = fitl;
	if(modelName=="SPICE")
	{
		_A = 1.78;
		_B = 0.427;
		_C = 0.014;
		std::ifstream in("/home/jordan/ANewHope/SPICE_data.dat");
		float depth,density;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>density;
			_indexVsDepth.push_back(std::pair<float,float>(-depth,1.0+0.86*density/1000.0));
		}
		in.close();
	}
	else if(modelName=="Byrd")
	{
		_A = 1.78;
		_B = 0.464;
		_C = 0.0244;
		std::ifstream in("/home/jordan/ANewHope/Byrd_data.csv");
		float depth,index;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>index;
			_indexVsDepth.push_back(std::pair<float,float>(depth,index));
		}
		in.close();
	}
}

float Ice::GetIndex(float z)
{
	if(_useIndexFit)
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
	else
	{
		if(z>0.0)
		{
			return 1.0;
		}
		else
		{
			std::vector<std::pair<float,float> >::iterator i = _indexVsDepth.begin();
			while(i!=_indexVsDepth.end())
			{
				if(z<=(*i).first) ++i;
				else break;
			}
			if(i==_indexVsDepth.end()) return _A-_B*exp(_C*z);
			else
			{
				return (*i).second + ((*(i-1)).second-(*i).second)/((*(i-1)).first-(*i).first)*(z-(*i).first);
			}
		}
	}
}
