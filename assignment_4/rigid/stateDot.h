#ifndef __RIGIDSTATEA_H__
#define __RIGIDSTATEA_H__

#include "../3rdparty/quaternion.h"
#include "../3rdparty/Vector.h"
class StateDot {
public:
    Vector3d velocity;
    Quaternion quaterA;
    Vector3d force;
    Vector3d torque;
 

	StateDot() :velocity(Vector3d(0, 0, 0)),
		quaterA(Quaternion()),
		force(Vector3d(0, 0, 0)),
		torque(Vector3d(0, 0, 0))
	{

	}


	StateDot operator*(double h)
	{
		StateDot delta;

		delta.velocity = this->velocity * h;
		delta.quaterA = this->quaterA * h;
		delta.force = this->force*h;
		delta.torque = this->torque*h;

		return delta;
	}


	StateDot operator+(const StateDot a)
	{
		StateDot newA;

		newA.velocity = this->velocity + a.velocity;
		newA.quaterA = this->quaterA + a.quaterA;
		newA.force = this->force + a.force;
		newA.torque = this->torque + a.torque;
		return newA;


	}
};

#endif
