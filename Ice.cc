#include "Ice.h"

void Ice::CreateIce()
{
	if(_iceModelName=="SPICE" || _iceModelName=="RICE")
	{
		_A = 1.78;
		_B = 0.427;
		_C = 0.014;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="Byrd")
	{
		_A = 1.78;
		_B = 0.464;
		_C = 0.0244;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="MB1")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.029;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="MB2")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.027;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="Mizuho")
	{
		_A = 1.78;
		_B = 0.423;
		_C = 0.027;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="Eisen")
	{
		_A = 1.78;
		_B = 0.48;
		_C = 0.02;
		_indexVsDepth.clear();
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
	else if(_iceModelName=="Gow")
	{
		_A = 1.78;
		_B = 0.435;
		_C = 0.016;
		_indexVsDepth.clear();
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

float Ice::GetIndex(float z)
{
	if(_iceUseIndexFit)
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

float Ice::GetAttenuationLength(float z)
{
	if(_iceUseAttenuationLengthFit)
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

void Ice::SetIceDimensions(float y,float z)
{
	_dimensions.first = y;
	_dimensions.second = z;
}