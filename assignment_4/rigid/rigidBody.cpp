#include "rigidBody.h"

RigidBody::RigidBody()
{
	Vector3d center(0, 20, 0);
	vertexPos[0] = Vector3d(-halfSize, -halfSize, halfSize) + center;
	vertexPos[1] = Vector3d(halfSize, -halfSize, halfSize) + center;
	vertexPos[2] = Vector3d(halfSize, -halfSize, -halfSize) + center;
	vertexPos[3] = Vector3d(-halfSize, -halfSize, -halfSize) + center;
	vertexPos[4] = Vector3d(-halfSize, halfSize, halfSize) + center;
	vertexPos[5] = Vector3d(halfSize, halfSize, halfSize) + center;
	vertexPos[6] = Vector3d(halfSize, halfSize, -halfSize) + center;
	vertexPos[7] = Vector3d(-halfSize, halfSize, -halfSize) + center;

	for (int i = 0; i < number; i++) {
		vertexPosNew[i] = vertexPos[i];
	}

	//Matrix3x3 m(0, 0, 0, 0, 0, 0, 0, 0, 0);
	Matrix3x3 m(1, 0, 0, 0, 1, 0, 0, 0, 1);
	rigidState.xposition = center;
	rigidState.quater = Quaternion(m);
	rigidState.pfmom = Vector3d(0, 0, 0);
	rigidState.lamom = Vector3d(0, 0, 0);

	Vector3d w(0, 0, 0);
	rigidStateDot.velocity = Vector3d(0, 0, 0);
	rigidStateDot.quaterA = 0.5*w*(rigidState.quater);
	rigidStateDot.force = Vector3d(0, 0, 0);
	rigidStateDot.torque = Vector3d(0, 0, 0);

}

RigidBody::~RigidBody()
{
}

void RigidBody::startFall()
{
	resetSign = false;

	bodyForce = Vector3d(0, -9.8, 0);	//----------Change G here
}

Matrix3x3 RigidBody::getStateRotation()
{
	Matrix3x3 m = rigidState.quater.rotation();
	return m;
}
 
Vector3d RigidBody::getStatePosition()
{
	return rigidState.xposition;
}

void RigidBody::updateFall()
{
	double hStep = 0.01;		// ----------change Refresh Frequency here
	statesNumInt(rigidState, rigidStateDot, rigidStateNew, hStep);
	collisionDetect(rigidState, rigidStateDot, rigidStateNew, hStep);

	for (int i = 0; i<number; i++) {
		vertexForce[i] = Vector3d(0, 0, 0);
	}

	rigidState = rigidStateNew;
	center = rigidState.xposition;
	for (int i = 0; i<number; i++) {
		vertexPos[i] = vertexPosNew[i];
	}
}

StateDot RigidBody::F(Rigidstate& rigidState) // calculate the new stateDot value
{
	StateDot rigidStateDot;

	rigidStateDot.velocity = rigidState.pfmom*(1.0 / mass);  // v = p/m
	Matrix3x3 R = rigidState.quater.rotation();
	Matrix3x3 I_inverse = R * (Io.inv()) * (R.transpose());
	Vector3d w = I_inverse * rigidState.lamom;
	rigidStateDot.quaterA = 0.5 * w * rigidState.quater;

	// calculate the tot_force
	Vector3d tot_force(0, 0, 0);
	for (int i = 0; i < number; i++) {
		tot_force = tot_force + vertexForce[i];
	}
	tot_force = tot_force + bodyForce;
	if (addForce) {
		std::cout << "click!" << std::endl;
		tot_force = tot_force + Vector3d(0, 20, 0);
	}

	rigidStateDot.force = tot_force;
	// calculate the tot_torque
	Vector3d tot_torque(0, 0, 0);
	Vector3d torque(0, 0, 0);
	for (int i = 0; i<number; i++) {
		torque = (vertexPos[i] - center) % vertexForce[i];
		tot_torque = tot_torque + torque;
	}
	rigidStateDot.torque = tot_torque;

	return rigidStateDot;
}

void RigidBody::statesNumInt(Rigidstate& rigidState, StateDot& rigidStateDot, Rigidstate& rigidStateNew, double h)
{
	StateDot K1 = F(rigidState);
	StateDot K2 = F(rigidState + K1 * (h*0.5));
	rigidStateDot = (K1 + (K2 * 2))*(1.0 / 3);
	rigidStateNew = rigidState + rigidStateDot * (h);

	Vector3d centerNew = rigidStateNew.xposition;
	Matrix3x3 R = rigidStateNew.quater.rotation();

	vertexPosNew[0] = R * (Vector3d(-halfSize, -halfSize, halfSize)) + centerNew;
	vertexPosNew[1] = R * (Vector3d(halfSize, -halfSize, halfSize)) + centerNew;
	vertexPosNew[2] = R * (Vector3d(halfSize, -halfSize, -halfSize)) + centerNew;
	vertexPosNew[3] = R * (Vector3d(-halfSize, -halfSize, -halfSize)) + centerNew;
	vertexPosNew[4] = R * (Vector3d(-halfSize, halfSize, halfSize)) + centerNew;
	vertexPosNew[5] = R * (Vector3d(halfSize, halfSize, halfSize)) + centerNew;
	vertexPosNew[6] = R * (Vector3d(halfSize, halfSize, -halfSize)) + centerNew;
	vertexPosNew[7] = R * (Vector3d(-halfSize, halfSize, -halfSize)) + centerNew;
}

void RigidBody::collisionDetect(Rigidstate& rigidState, StateDot& rigidStateDot, Rigidstate& rigidStateNew, double h)
{
	double min_h = 9999;
	for (int i = 0; i < number; i++) {
		if (min_h > vertexPosNew[i].y) {
			min_h = vertexPosNew[i].y;
		}
	}
	if (min_h < -DepthEpsilon) {
		while (1) {
			h = h / 2;
			statesNumInt(rigidState, rigidStateDot, rigidStateNew, h);
			min_h = 9999;
			for (int i = 0; i < number; i++) {
				if (min_h > vertexPosNew[i].y) {
					min_h = vertexPosNew[i].y;
				}
			}
			if (min_h > -DepthEpsilon) {
				break;
			}
		}
	}
	
	for (int i = 0; i < number; i++) {
		if (vertexPosNew[i].y >= DepthEpsilon) {
			continue;
		}
		if (vertexPosNew[i].y < DepthEpsilon) {
			if (rigidStateDot.velocity.y < 0) {
				// judge if need to reset
				if (rigidStateDot.velocity.norm() < 0.1) {
					int counter = 0;
					for (int i = 0; i < number; i++) {
						if (vertexPosNew[i].y < 0.08) {
							counter++;
						}
					}
					if (counter >= 3) {		// over 3 corners are stastic = cube is stastic
						resetSign = true;
						return;
					}
				}
			}
				// update p and l
				Matrix3x3 R = rigidState.quater.rotation();
				Matrix3x3 Iinverse = R * (Io.inv()) * (R.transpose());
				Vector3d w = Iinverse * rigidState.lamom;
				Vector3d r = vertexPos[i] - center;

				Vector3d bt_norm = Vector3d(0, 1, 0);
				double Vn = bt_norm * (rigidStateDot.velocity + w % r);
				double j = -1 * (1 + Cr)*Vn / (1.0 / mass + bt_norm * (Iinverse*(r%bt_norm) % r));
				Vector3d J = j * bt_norm;

				rigidState.pfmom = rigidState.pfmom + J;
				rigidState.lamom = rigidState.lamom + r % J;
				statesNumInt(rigidState, rigidStateDot, rigidStateNew, h);
			
		}
	}









	/*



	for (int i = 0; i<number; i++) {
		if (vertexPosNew[i].y<DepthEpsilon) {

			if ((vertexPosNew[i] - vertexPos[i]).norm() > 0.05) {
				while (1)
				{
					statesNumInt(rigidState, rigidStateDot, rigidStateNew, h*0.5);
					if (vertexPosNew[i].y<0) {
						break;
					}
					else
					{
						rigidState = rigidStateNew;
						center = rigidState.xposition;
						for (int i = 0; i<number; i++) {
							vertexPos[i] = vertexPosNew[i];
						}
					}
				}
				collisionDetect(rigidState, rigidStateDot, rigidStateNew, h*0.5);

			}
			else
			{
				// judge if need to reset
				if (rigidStateDot.velocity.norm()<0.1) {
					int counter = 0;
					for (int i = 0; i<number; i++) {
						if (vertexPosNew[i].y < 0.08) {
							counter++;
						}
					}
					if (counter >= 3) {		// over 3 corners are stastic = cube is stastic
						resetSign = true;
						return;
					}
				}

				// update p and l
				Matrix3x3 R = rigidState.quater.rotation();
				Matrix3x3 Iinverse = R * (Io.inv()) * (R.transpose());
				Vector3d w = Iinverse * rigidState.lamom;
				Vector3d r = vertexPos[i] - center;

				Vector3d bt_norm = Vector3d(0, 1, 0);
				double Vn = bt_norm * (rigidStateDot.velocity + w % r);
				double j = -1 * (1 + Cr)*Vn / (1.0 / mass + bt_norm * (Iinverse*(r%bt_norm) % r));
				Vector3d J = j * bt_norm;

				rigidState.pfmom = rigidState.pfmom + J;
				rigidState.lamom = rigidState.lamom + r % J;
				statesNumInt(rigidState, rigidStateDot, rigidStateNew, h);
			}
		}
	}
	*/
}

void RigidBody::ResolveCollisions(int cornerIndex)
{
	
}
