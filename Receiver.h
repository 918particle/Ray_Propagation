#include "Polarization.h"
#include "Propagator.h"
#include <utility>

#ifndef RECEIVER_H_
#define RECEIVER_H_
class Receiver : public Polarization
{
	public:
	Receiver() : _receiverInitialAngle(0.0), _receiverRange(1.0)
	{
		_receiverPosition.first = 0.0;
		_receiverPosition.second = 0.0;
	};
	float _receiverInitialAngle; //initial angle with respect to horizontal, entered as degrees
	std::pair<float,float> _receiverPosition; //(y,z) position
	void InitializeReceiver(float,float,float); //set emitter y, z, initial angle
	bool ReceiveRFRay(Propagator*); //True if the RFRay is near the receiver, controlled by _receiverRange.
	float _receiverRange;
	std::vector<Propagator> _observedRFRays;
	void SetReceiverRange(float); //Set function for _receiverRange
	std::vector<float> _receiverDotProducts; //Results of the dot products of received rays with Receiver polarization.
};

#endif
