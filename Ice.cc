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
	return _A-_B*exp(_C*z);
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