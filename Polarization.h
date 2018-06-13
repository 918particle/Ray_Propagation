#include <vector>
#include <numeric>
#include <cmath>

#ifndef POLARIZATION_H_
#define POLARIZATION_H_

class Polarization
{
	public:
	Polarization()
	{
		_polarization.clear();
		_polarization.push_back(0.0);
		_polarization.push_back(0.0);
		_polarization.push_back(1.0); //Default polarization is in the z-direction (see PropagatorSettingsManager).
	};
	std::vector<float> _polarization; //a unit-norm vector that tracks the polarization
	void SetPolarization(std::vector<float> pol)
	{
		//Ensure vector is only three elements
		pol.resize(3);
		_polarization.clear();
		_polarization.push_back(pol[0]);
		_polarization.push_back(pol[1]);
		_polarization.push_back(pol[2]);
		//Check if it is not normalized, then divide if not.
		float n = std::inner_product(_polarization.begin(),_polarization.end(),_polarization.begin(),0.0);
		_polarization[0]/=n;
		_polarization[1]/=n;
		_polarization[2]/=n;
	}
};

#endif
