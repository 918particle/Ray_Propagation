#include "Receiver.h"

void Receiver::InitializeReceiver(float y,float z,float angle)
{
	_receiverInitialAngle = angle*3.14159/180.0;
	_receiverPosition.first = y;
	_receiverPosition.second = z;
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
		if(std::abs((*i).first - _receiverPosition.first)<_receiverRange 
			&& std::abs((*i).second - _receiverPosition.second)<_receiverRange)
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