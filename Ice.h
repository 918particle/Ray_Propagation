#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <fstream>
#include <map>

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
		_iceModelName("SPICE"),
		_perturbed(false)
		{
			_iceBoundaryIndex = this->GetIndex(0.0);
			_perturbations.clear();
		};
	std::pair<float,float> _dimensions; //dimensions in meters, (r,z)
	float _A; //Index parameter A, no units because it's the ice index
	float _B; //Index parameter B, no units because it's the difference ice index and surface index
	float _C; //Index parameter C, units of 1/meters because it's the compactification rate
	void CreateIce(); //Load data describing index of refraction versus depth or density versus depth, or re-set constants
	float GetIndex(float); //Return index of refraction at given depth in meters
	float GetAttenuationLength(float); //Return attenuation length at given depth in meters
	void LoadIndexData(std::string); //Load and interpolate index of refraction data
	std::string _iceModelName; //Name of the model giving density or index of refraction versus depth.
	void SetIceDimensions(float,float); //Sets the cylindrical radius and depth of the ice.
	void SetIceModelName(std::string); //Resets physical constants of the ice, given the name of the model.
	float _iceBoundaryIndex;
	void AddGaussianPerturbation(float,float,float); //Perturbs the index of refraction profile with a Gaussian.
	bool _perturbed;  //True if there are perturbations
	std::map<int,std::vector<float> > _perturbations; //A map containing perturbations to the n(z) profile.
};

#endif