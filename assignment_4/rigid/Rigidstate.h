#pragma once
#include "../3rdparty/quaternion.h"
#include "../3rdparty/Vector.h"
#include "stateDot.h"
class Rigidstate {
public:
	Vector3d xposition;
	Quaternion quater;
	Vector3d pfmom;
	Vector3d lamom;
 
	Rigidstate() : xposition(Vector3d(0, 0, 0)),
		quater(Quaternion()),
		pfmom(Vector3d(0, 0, 0)),
		lamom(Vector3d(0, 0, 0))
	{

	}

	Rigidstate operator+(const Rigidstate state)
	{
		Rigidstate stateNew;
		stateNew.xposition = this->xposition + state.xposition;
		stateNew.quater = this->quater + state.quater;
		stateNew.pfmom = this->pfmom + state.pfmom;
		stateNew.lamom = this->lamom + state.lamom;

		return stateNew;
	}

	Rigidstate operator+(const StateDot delta)
	{
		Rigidstate stateNew;
		stateNew.xposition = this->xposition + delta.velocity * 1;
		stateNew.quater = (this->quater + delta.quaterA * 1).normalize();

		stateNew.pfmom = this->pfmom + delta.force * 1;
		stateNew.lamom = this->lamom + delta.torque * 1;
		return stateNew;
	}

};
