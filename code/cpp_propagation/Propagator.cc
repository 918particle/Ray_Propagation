#include "Propagator.h"

//Initialize base classes with data, choose models
void Propagator::Initialize(char *argv[])
{
	_sigma = atof(argv[1]);  // Standard Deviation for diffuse scatters
	_ReflectionMethod = atof(argv[2]); // 1 = force reflecting surfaces, 2 = Every point treated as possible reflection
	_angleI = atof(argv[3]); // Initial Angle
	_angleF = atof(argv[4]); // Final Angle
	_dtheta = 0.1; // Angle step size
	_scatterLength = atof(argv[5]); // How many steps before a diffuse sactter is allowed (specular scatters possible at every step)
	_nrays = atof(argv[6]); // How many rays to launch at a specific angle
	float emitterX = atof(argv[7]);
	float emitterY = atof(argv[8]);
	float emitterZ = atof(argv[9]);
	std::stringstream ider;
	_pol = {0.0,0.0,0.1};
	_globalTime = 11000.0;
	_emitterPosition = {emitterX,emitterY,emitterZ};
	_iceSize = {1000.0,1000.0,2000.0};
	_preferences.first = false; //Preferences for index and attenuation length treatment
	_preferences.second = false;
	InitializeIce("SPICE"); //Ice Model
	std::vector<float> Reflector_pos = {100.0,0.0,-200.0};
	AddReflector(Reflector_pos,_sigma); // This is necessary for _ReflectionMethod 1 only
	InitializeCollector();
}

void Propagator::InitializePropagator(float angle_em, std::vector<float> p)
{
	this->_path.clear();
	this->InitializeEmitter(_emitterPos,angle_em,p);
	_currentPosition[0] = _emitterPosition[0];
	_currentPosition[1] = _emitterPosition[1];
	_currentPosition[2] = _emitterPosition[2];
	this->_path.push_back(_currentPosition);
	this->_currentAngle = _initialAngle;
	_isInitialized = true;
	yangle = 0.0;
	rAmplitude = 0.0;
	dx,dy,dz; //units: nanoseconds, meters, meters, radians
	theTime = 0.0;
	previousAngle = 0.0;
	InAir = false;
	stepNum = 1;
	indexFit = true; //reflections
	surfaceReflection = false;
}

void Propagator::InitializeIce(std::string ice_model)
{
	this->_path.clear();
	this->CreateIce(_iceSize,_preferences.first,_preferences.second,ice_model);
}
void Propagator::AddReflector(std::vector<float> x,float y)
{
	this->CreateReflector(x,y);
}
void Propagator::InitializeCollector()
{
	std::vector<float> collector_pos = {1400.0,0.0,-22.0};
	this->CreateCollectors(0.0,40,2.0,collector_pos,50.0);
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
	if(_ReflectionMethod == 1)  // Reflections only at hardcoded locations
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

	if(_ReflectionMethod == 2 && !InAir)  // Every point in propogation is treated as possibe reflector
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

