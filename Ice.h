#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <fstream>

#ifndef ICE_H_
#define ICE_H_

class Ice
{
	public:
	Ice() : 
		_dimensions(std::pair<float,float>(10000.0,-10000.0)),
		_A(1.78),
		_B(0.427),
		_C(0.014),
		_iceUseIndexFit(true),
		_iceUseAttenuationLengthFit(true),
		_iceModelName("SPICE"){};
	std::pair<float,float> _dimensions; //dimensions in meters, (r,z)
	std::vector<std::pair<float,float> > _indexVsDepth; //index of refraction for RF frequencies vs. depth in meters
	std::vector<std::pair<float,float> > _attenuationLengthVsDepth; //attenuation length in meters vs. depth in meters
	float _A; //Index parameter A, no units because it's the ice index
	float _B; //Index parameter B, no units because it's the difference ice index and surface index
	float _C; //Index parameter C, units of 1/meters because it's the compactification rate
	void CreateIce(); //Load data describing index of refraction versus depth or density versus depth, or re-set constants
	float GetIndex(float); //Return index of refraction at given depth in meters
	float GetAttenuationLength(float); //Return attenuation length at given depth in meters
	void LoadIndexData(std::string); //Load and interpolate index of refraction data
	bool _iceUseIndexFit; //true = index is a smooth fit to data.  false = index is interpolated from data files.
	bool _iceUseAttenuationLengthFit; //true = attenuation length is a smooth fit to data.  false = interpolated from data
	std::string _iceModelName; //Name of the model giving density or index of refraction versus depth.
	void SetIceDimensions(float,float); //Sets the cylindrical radius and depth of the ice.
};

#endif
