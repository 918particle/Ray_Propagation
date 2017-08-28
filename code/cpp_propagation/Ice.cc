#include "Ice.h"
#include <cmath>
#include <fstream>
#include <iostream>

void Ice::CreateIce(std::vector<float> d,bool fitn,bool fitl,std::string modelName)
{
	_dimensions = d;
	_useIndexFit = fitn;
	_useAttenuationLengthFit = fitl;
	if(modelName=="SPICE" || modelName=="RICE")
	{
		_A = 1.78;
		_B = 0.427;
		_C = 0.014;
		std::ifstream in("/home/geoffrey/Ray_Propagation/code/cpp_propagation/Maker2/SPICE_data.dat");
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
	else if(modelName=="MB1")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.029;
		std::ifstream in("/home/jordan/ANewHope/ARIANNA_Borehole_1.dat");
		float depth,density;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>density;
			_indexVsDepth.push_back(std::pair<float,float>(-depth,1.0+0.86*density/1000.0));
		}
		in.close();
	}
	else if(modelName=="MB2")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.027;
		std::ifstream in("/home/jordan/ANewHope/ARIANNA_Borehole_2.dat");
		float depth,density;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>density;
			_indexVsDepth.push_back(std::pair<float,float>(-depth,1.0+0.86*density/1000.0));
		}
		in.close();
	}
	else if(modelName=="Mizuho")
	{
		_A = 1.78;
		_B = 0.423;
		_C = 0.027;
		std::ifstream in("/home/jordan/ANewHope/Mizuho_data.csv");
		float depth,index;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>index;
			_indexVsDepth.push_back(std::pair<float,float>(depth,index));
		}
		in.close();
	}
	else if(modelName=="Eisen")
	{
		_A = 1.78;
		_B = 0.48;
		_C = 0.02;
		std::ifstream in("/home/jordan/ANewHope/Eisen_Maud_data.csv");
		float depth,index;
		while(in.good() && ~in.eof())
		{
			in>>depth;
			in>>index;
			_indexVsDepth.push_back(std::pair<float,float>(depth,index));
		}
		in.close();
	}
	else if(modelName=="Gow")
	{
		_A = 1.78;
		_B = 0.435;
		_C = 0.016;
		std::ifstream in("/home/jordan/ANewHope/Gow_withOnePlus86_data.csv");
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

float Ice::GetIndex(float z, bool fit)
{
	if(fit)
	{
		if(z>0)
		{
			return 1.0;
		}
		if(z < -200.0)
		{
			return _A-_B*exp(_C*-200.0);
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
			if(z < -200.0)
			{
				return _A-_B*exp(_C*-200.0);
			}
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

float Ice::GetAttenuationLength(float z)
{
	if(_useAttenuationLengthFit)
	{
		if(z>0)
		{
			return 1.0e6;
		}
		else
		{
			return 1000.0;
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
			std::vector<std::pair<float,float> >::iterator i = _attenuationLengthVsDepth.begin();
			while(i!=_attenuationLengthVsDepth.end())
			{
				if(z<=(*i).first) ++i;
				else break;
			}
			if(i==_indexVsDepth.end()) return 1000.0;
			else
			{
				return (*i).second + ((*(i-1)).second-(*i).second)/((*(i-1)).first-(*i).first)*(z-(*i).first);
			}
		}
	}
}
