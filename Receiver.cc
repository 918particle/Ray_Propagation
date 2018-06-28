#include "Receiver.h"

void Receiver::InitializeReceiver(float y,float z,float angle,float t)
{
	_receiverInitialAngle = angle*3.14159/180.0;
	_receiverPosition.first = y;
	_receiverPosition.second = z;
	this->SetReceiverRange(t*0.299792458/1.3*1.1);
}

void Receiver::SetReceiverRange(float r)
{
	_receiverRange = r;
}

bool Receiver::ReceiveRFRay(Propagator *p)
{
	bool flag = false;
	std::vector<std::pair<float,float> >::iterator i;
	for(i=p->_path.begin();i!=p->_path.end();++i)
	{
		if(std::abs((*i).first - _receiverPosition.first)<_receiverRange //check y-coordinates
			&& std::abs((*i).second - _receiverPosition.second)<_receiverRange) //check z-coordinates
		{
			_observedRFRays.push_back(*p);
			_receiverDotProducts.push_back(std::inner_product(_polarization.begin(),_polarization.end(),
				p->_polarization.begin(),0.0));
			flag=true;
			break;
		}
		_numReceivedRFRays = _observedRFRays.size();
	}
	return flag;
}

int Receiver::GetReceivedRFRays()
{
	return _numReceivedRFRays;
}

float Receiver::GetReceivedDotProduct(int i)
{
	return _receiverDotProducts[i];
}