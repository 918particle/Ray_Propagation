#include "Ice.h"
#include <iostream>

void Ice::CreateIce()
{
	if(_iceModelName=="SPICE" || _iceModelName=="RICE")
	{
		_A = 1.78;
		_B = 0.427;
		_C = 0.014;
	}
	else if(_iceModelName=="Byrd")
	{
		_A = 1.78;
		_B = 0.464;
		_C = 0.0244;
	}
	else if(_iceModelName=="MB1")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.029;
	}
	else if(_iceModelName=="MB2")
	{
		_A = 1.78;
		_B = 0.46;
		_C = 0.027;
	}
	else if(_iceModelName=="Mizuho")
	{
		_A = 1.78;
		_B = 0.423;
		_C = 0.027;
	}
	else if(_iceModelName=="Eisen")
	{
		_A = 1.78;
		_B = 0.48;
		_C = 0.02;
	}
	else if(_iceModelName=="Gow")
	{
		_A = 1.78;
		_B = 0.435;
		_C = 0.016;
	}
	_iceBoundaryIndex = this->GetIndex(0.0);
}

float Ice::GetIndex(float z)
{
	if(z>0) return 1.0;
	else
	{
		if(_perturbed)
		{
			float result = _A-_B*exp(_C*z);
			std::map<int,std::vector<float> >::iterator i;
			for(i=_perturbations.begin();i!=_perturbations.end();++i)
			{
				result += (*i).second[1]*exp(-0.5*((z-(*i).second[0])/(*i).second[2])*((z-(*i).second[0])/(*i).second[2]));
			}
			return result;
		}
		else
		{
			return _A-_B*exp(_C*z);
		}
	}
}

float Ice::GetAttenuationLength(float z)
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

void Ice::SetIceDimensions(float y,float z)
{
	_dimensions.first = y;
	_dimensions.second = z;
}

void Ice::SetIceModelName(std::string name)
{
	_iceModelName = name;
	this->CreateIce(); //resets constants.
}

void Ice::AddGaussianPerturbation(float z,float a,float sigma)
{
	_perturbed=true;
	int n = _perturbations.size();
	std::vector<float> p;
	p.push_back(z); //Depth of perturbation
	p.push_back(a); //Size of perturbation
	p.push_back(sigma); //Width of perturbation
	_perturbations[n+1] = p;
}