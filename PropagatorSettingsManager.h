#include "Propagator.h"

#ifndef PROPAGATORSETTINGSMANAGER_H_
#define PROPAGATORSETTINGSMANAGER_H_

class PropagatorSettingsManager : public Propagator
{
	public:
	PropagatorSettingsManager(Propagator *p)
	{
		_propagator = p;
	};
	void SetIceDimensions(float,float); //cylinder radius in meters, vertical depth (negative), e.g. -2500.0 meters
	void SetIceModelName(std::string); //Set the name of the model giving
	void SetIceParameters(bool,bool); //Determines whether to use fits or load data for index and attenuation length, respectively
	void ResetPolarization(std::vector<float>); //Changes the initial polarization.
	void ResetReflectorRange(float); //The distance in meters from a hard reflector that triggers CheckForAReflection
	void ResetEmitterInitial(float,float,float); //The emitter (y,z) in meters, and the angle in degrees
	void SetGlobalTime(float); //Control the total propagation time allowed, in nanoseconds.
	void SetPropagationTimestep(float); //Control the propagation time step, in nanoseconds.
	Propagator *_propagator;
};

#endif