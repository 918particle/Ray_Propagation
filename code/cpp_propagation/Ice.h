#include <vector>
#include <string>
#include <utility>

#ifndef ICE_H_
#define ICE_H_

class Ice
{
	public:
	Ice() : _useIndexFit(true), _useAttenuationLengthFit(true), _A(1.78), _B(0.427), _C(0.014) {};
	std::vector<float> _dimensions; //dimensions in meters, (r,z)
	std::vector<std::pair<float,float> > _indexVsDepth; //index of refraction for RF frequencies vs. depth in meters
	std::vector<std::pair<float,float> > _attenuationLengthVsDepth; //attenuation length in meters vs. depth in meters
	bool _useIndexFit; //true = index is a smooth fit to data.  false = index is interpolated from data files.
	bool _useAttenuationLengthFit; //true = attenuation length is a smooth fit to data.  false = interpolated from data
	float _A; //Index parameter A, no units because it's the ice index
	float _B; //Index parameter B, no units because it's the difference ice index and surface index
	float _C; //Index parameter C, units of 1/meters because it's the compactification rate
	void CreateIce(std::vector<float>,bool,bool,std::string = "");
	float GetIndex(float,bool); //Return index of refraction at given depth in meters
	float GetAttenuationLength(float); //Return attenuation length at given depth in meters
	void LoadIndexData(std::string); //Load and interpolate index of refraction data
};

#endif
