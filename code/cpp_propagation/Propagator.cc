#include "Propagator.h"

void Propagator::InitializePropagator(float angle_em, std::vector<float> p)
{
	this->_path.clear();
	_currentPosition = this->_emitterPosition;
	this->_path.push_back(_currentPosition);
	this->_currentAngle = _initialAngle;
	_isInitialized = true;
}

void Propagator::AddReflector(std::vector<float> x,float y)
{
	this->CreateReflector(x,y);
}

void Propagator::ReadoutPath(int count)
{
	std::stringstream ss;
	ss<<count;
	std::string title ="data/propagation_path_"+ss.str()+".dat";
	std::ofstream out(title.c_str());
	std::vector<std::vector<float> >::iterator i = this->_path.begin();
	while(i!=this->_path.end())
	{
		//3DCODEout<<(*i)[0]<<" "<<(*i)[1]<<" "<<(*i)[2]<<std::endl;
		out<<(*i)[0]<<" "<<(*i)[2]<<std::endl;
		++i;
	}
	out.close();
	this->_path.clear();
}

void Propagator::ReadoutAngles()
{
	std::string title = "anglesAt1400plus.dat";
	std::ofstream out(title.c_str());
	std::vector<float>::iterator i = this->anglesAt1400.begin();
	while(i!=this->anglesAt1400.end())
	{
		//3DCODEout<<(*i)[0]<<" "<<(*i)[1]<<" "<<(*i)[2]<<std::endl;
		out<<(*i)<<std::endl;
		++i;
	}
	out.close();
	this->anglesAt1400.clear();
	Collector::ReadoutPath();
}
//Propagation from t=0 to the global time.  This makes it trivial to run
// the same propagation effect for different times with the same settings.
void Propagator::Propagate(int tag)
{
	RFRayTracker *T = new RFRayTracker(_initialAngle,_currentPosition);
	int angle_bool = false;
	while(theTime<_globalTime)
	{
		theTime+=_timeStep;
		// Continuous Refraction
		//Refractions::Refract();

		if(this->_currentPosition[2] > 0)
		{
			InAir = true;
		}

		float n = GetIndex(_currentPosition[2],indexFit);
		dx=cos(this->_currentAngle)*_timeStep*c0/n;
		dy=0; //3DCODEdy=sin(yangle)*_timeStep*c0/n;
		dz=sin(this->_currentAngle)*_timeStep*c0/n;

		dTheta = Reflections::Refractions::Refract(this->_currentPosition, this->_currentAngle, dz, indexFit, _sigma, stepNum, _scatterLength,_timeStep, n,surfaceReflection,InAir);
		previousPosition = this->_currentPosition;
		this->Update(dx,dy,dz,dTheta);
		this->_path.push_back(_currentPosition);
		Collector::Collect(previousPosition,this->_currentPosition);
		// Check for Reflection
		previousAngle = this->_currentAngle; // Used to check if ray got reflected
		CheckForReflection();
		if(previousAngle * _currentAngle < 0) // switched condition statemnt to include TIR (reflection causes sign flip)
		{
			T->StoreNewReflection(std::pair<float,float>(previousAngle,_currentAngle),_currentPosition,rAmplitude);
		}
		stepNum++;
	}
	T->StoreFinalData(this->_currentAngle,_currentPosition);
	std::stringstream ss;
	ss<<tag;
	T->ReadoutTracker("data/report_"+ss.str()+".dat");
	delete T;
}

void Propagator::CheckForReflection()
{
	if(_reflectionMethod == 1)  // Reflections only at hardcoded locations
	{
		CheckForAReflection(this->_currentAngle,this->_currentPosition[2],this->_polarization,dz);
		if(previousAngle * _currentAngle < 0) // switched condition statemnt to include TIR (reflection causes sign flip)
		{
			_currentPosition[2] -= dz; // This forces the next CheckForReflection to not be true at same reflecter that just reflected
			_currentPosition[1] += dy; // This forces the next CheckForReflection to not be true at same reflecter that just reflected
			_currentPosition[0] += dx; 
			//3DCODEyangle += RandomGauss(_sigma);
		}
	}

	if(_reflectionMethod == 2 && !InAir)  // Every point in propogation is treated as possibe reflector
	{
		float currentAmplitude = 1.0;			
		//    TIR
		if(!Reflections::TIR(this->_currentPosition, this->_currentAngle, dz, indexFit, _sigma, stepNum, _scatterLength))
		{
			//		Reflections
			if(Reflections::Reflect(this->_currentPosition,this->_polarization, this->_currentAngle,currentAmplitude, dz, indexFit, _sigma, stepNum, _scatterLength))
			{
				//		Account for symmetry in specular reflection
					//	this->Update(dx,dy,-dz,dTheta);
					//	this->_path.push_back(_currentPosition);
			}
		}
	}		


	if(this->_currentPosition[2] + (dz/2) > 0 || this->_currentPosition[2] - (dz/2) > 0 )
	{
		surfaceReflection = true;
	}
	else
	{
		surfaceReflection = false;
	}
}

