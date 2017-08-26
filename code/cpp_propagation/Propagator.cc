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
	_pol = {0.0,0.0,0.1};
	_globalTime = 15000.0;
	_emitterPosition = {emitterX,emitterY,emitterZ};
	_iceSize = {1000.0,1000.0,2000.0};
	_preferences.first = false; //Preferences for index and attenuation length treatment
	_preferences.second = false;
	InitializeIce("SPICE"); //Ice Model
	std::vector<float> Reflector_pos = {100.0,0.0,-200.0};
	AddReflector(Reflector_pos,_sigma); // This is necessary for _ReflectionMethod 1 only
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
	dndz = 0.0; //units: meters^(-1)
	rAmplitude = 0.0;
	dx,dz,dTheta; //units: nanoseconds, meters, meters, radians
	theTime = 0.0;
	previousAngle = 0.0;
	InAir = false;
	stepNum = 1;
	indexFit = false; //reflections
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

void Propagator::ReadoutPath(int count)
{
	std::stringstream ss;
	ss<<count;
	std::string title = "data/propagation_path_"+ss.str()+".dat";
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

//Propagation from t=0 to the global time.  This makes it trivial to run
// the same propagation effect for different times with the same settings.
void Propagator::Propagate(int tag)
{
	RFRayTracker *T = new RFRayTracker(_initialAngle,_currentPosition);
	while(theTime<_globalTime)
	{
		theTime+=_timeStep;
		// Continuous Refraction
		Refract();
		this->Update(dx,dy,dz,dTheta);
		this->_path.push_back(_currentPosition);

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

void Propagator::Refract()
{
	if(this->_currentPosition[2] > 0)
	{
		InAir = true;
	}
	float n = GetIndex(_currentPosition[2],indexFit);
	dx=cos(this->_currentAngle)*_timeStep*c0/n;
	//3DCODEdy=sin(yangle)*_timeStep*c0/n;
	dy=0;
	dz=sin(this->_currentAngle)*_timeStep*c0/n;
	if(std::abs(dz)>z0)
	{
		dndz = (GetIndex(this->_currentPosition[2]+(dz/2),indexFit)-GetIndex(this->_currentPosition[2]-(dz/2),indexFit))/dz;
	}
	else
	{
		dndz = (GetIndex(this->_currentPosition[2],indexFit)-GetIndex(this->_currentPosition[2]-z0,indexFit))/(z0);
	}
	dTheta = 0;
	if(!InAir)
	{
		if((stepNum % _scatterLength) == 0 && this->_currentPosition[2] > -200.0)
		{
			dTheta = _timeStep*cos(this->_currentAngle)*dndz*c0/(n*n);// + (RandomGauss(_sigma));
		}
		else
		{
			dTheta = _timeStep*cos(this->_currentAngle)*dndz*c0/(n*n);				
		}
	}
	if(dz>0 && surfaceReflection)
	{
		dTheta = 0;
	}
}

void Propagator::CheckForReflection()
{
	// For Reflections
	float n_i_true = GetIndex(_currentPosition[2] - 0.5,indexFit);
	float n_f_true = GetIndex(_currentPosition[2] + 0.5,indexFit);
	float n_i_dev = std::abs(GetIndex(_currentPosition[2] - 0.5,true) - GetIndex(_currentPosition[2] - 0.5,indexFit));
	float n_f_dev = std::abs(GetIndex(_currentPosition[2] + 0.5,true) - GetIndex(_currentPosition[2] + 0.5,indexFit));
	float n_i_TIR = n_i_dev * n_f_dev * n_i_true;  
	float n_f_TIR = n_f_dev * n_i_dev * n_f_true; 	

	float n_i_trueR = GetIndex(_currentPosition[2] - (dz/2) - 0.01,indexFit);
	float n_f_trueR = GetIndex(_currentPosition[2] + (dz/2) + 0.01,indexFit);
	float n_i_devR = std::abs(GetIndex(_currentPosition[2] - (dz/2) - 0.01,true) - GetIndex(_currentPosition[2] - (dz/2) - 0.01,indexFit));
	float n_f_devR = std::abs(GetIndex(_currentPosition[2] + (dz/2) + 0.01,true) - GetIndex(_currentPosition[2] + (dz/2) + 0.01,indexFit));
	float n_i_reflection = n_i_dev * n_f_dev * n_i_true;  
	float n_f_reflection = n_f_dev * n_i_dev * n_f_true; 
	//  For TIR condition
	if(dz>0)
	{
		float n_i_true = GetIndex(_currentPosition[2] + 0.5,indexFit);
		float n_f_true = GetIndex(_currentPosition[2] - 0.5,indexFit);
		float n_i_dev = std::abs(GetIndex(_currentPosition[2] + 0.5,true) - GetIndex(_currentPosition[2] + 0.5,indexFit));
		float n_f_dev = std::abs(GetIndex(_currentPosition[2] - 0.5,true) - GetIndex(_currentPosition[2] - 0.5,indexFit));
		float n_i_TIR = n_i_dev * n_f_dev * n_i_true;  
		float n_f_TIR = n_f_dev * n_i_dev * n_f_true; 
	}

	if(_ReflectionMethod == 1)  // Reflections only at hardcoded locations
	{
		rAmplitude = CheckForAReflection(this->_currentAngle,this->_currentPosition[2],this->_polarization,dz);
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
		rAmplitude = Reflect(n_i_TIR,n_f_TIR,n_i_reflection,n_f_reflection);
		if(previousAngle == -this->_currentAngle)
		{
			this->Update(dx,dy,-dz,dTheta);
			this->_path.push_back(_currentPosition);
		}
		else
		{
			//#DCODEyangle += RandomGauss(_sigma);
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
	this->_currentAmplitude*=rAmplitude;
}


float Propagator::Reflect(float n_i_TIR, float n_f_TIR, float n_i_reflection, float n_f_reflection)
{
	float currentAmplitude = 1.0;
	float pi_2 = pi/2.0;
	float s = n_i_TIR/n_f_TIR;
	//    TIR
	if(std::abs(s*sin(pi_2 - this->_currentAngle))>1.0)
	{
		if(stepNum % _scatterLength != 0)
		{
			this->_currentAngle = -this->_currentAngle;
		}
		else
		{
			float ran = Reflector::RandomGauss(_sigma);
			this->_currentAngle = -this->_currentAngle+ran;
		}
	}
	else
	{
		s = n_i_reflection/n_f_reflection;
		//s-polarized component
		float a = std::abs(s*cos(pi_2 - this->_currentAngle));
		float b = sqrt(1.0-(s*s*sin(pi_2 - this->_currentAngle)*sin(pi_2 - this->_currentAngle)));
		float rs = (a-b)/(a+b); //reflected s-pol. relative amplitude (Jackson 7.39)
		float ts = (2*a)/(a+b);	//transmitted s-pol. relative amplitude	(Jackson 7.39)	
		//p-polarized component
		float c = std::abs(cos(pi_2 - this->_currentAngle));
		float d = s*sqrt(1.0-(s*s*sin(pi_2 - this->_currentAngle)*sin(pi_2 - this->_currentAngle)));
		float rp = (c-d)/(c+d); //reflected p-pol. relative amplitude (Jackson 7.41)
		float tp = (2*s*c)/(c+d); //transmitted p-pol. relative amplitude (Jackson 7.41)
		//Reflection and Transmission coefficients
		float R = rs*rs+rp*rp;
		float T = (b/a)*(ts*ts+tp*tp);
		float random = float(rand())/float(RAND_MAX);
		///   Reflection
		if(random<(R)) //Account for reflection coefficient
		{
			if(stepNum % _scatterLength != 0)
			{
				this->_currentAngle = -this->_currentAngle;
			}	
			else
			{
				float s_amp = currentAmplitude*this->_polarization[2]*rs;
				float p_amp = currentAmplitude*sqrt(this->_polarization[0]*this->_polarization[0]+this->_polarization[1]*this->_polarization[1])*rp;
				currentAmplitude = sqrt(s_amp*s_amp+p_amp*p_amp); //amplitude of reflected ray
				float ran = Reflector::RandomGauss(_sigma);
				this->_currentAngle = -this->_currentAngle+ran;
			}
		}
	}
	return currentAmplitude;
}