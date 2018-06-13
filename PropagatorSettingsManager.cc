#include "PropagatorSettingsManager.h"

void PropagatorSettingsManager::SetIceDimensions(float y,float z)
{
	_propagator->_dimensions = std::pair<float,float>(y,z);
	//No reset required yet.
}

void PropagatorSettingsManager::SetIceModelName(std::string name)
{
	_propagator->_iceModelName = name;
	_propagator->CreateIce(); //resets constants.
}

void PropagatorSettingsManager::SetIceParameters(bool fitn,bool fitl)
{
	_propagator->_iceUseIndexFit = fitn;
	_propagator->_iceUseAttenuationLengthFit = fitl;
	//No reset required yet.
}

void PropagatorSettingsManager::ResetPolarization(std::vector<float> p)
{
	_propagator->SetPolarization(p);
}

void PropagatorSettingsManager::ResetReflectorRange(float r)
{
	_propagator->_reflectorRange = r;
}

void PropagatorSettingsManager::ResetEmitterInitial(float y,float z,float angle)
{
	_propagator->InitializeEmitter(y,z,angle);
}

void PropagatorSettingsManager::SetGlobalTime(float t)
{
	_globalTime = t;
}

void PropagatorSettingsManager::SetPropagationTimestep(float t)
{
	_timeStep = t;
}