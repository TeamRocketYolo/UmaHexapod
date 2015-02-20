///////////////////////////////////////////////////////////////////////////
// Workfile    : Leg.h
// Author      : Patrick Felixberger, Josef Sommerauer
// Date        :
// Description : Leg
// Remarks     : -
// Revision    : 1
///////////////////////////////////////////////////////////////////////////
#ifndef LEG_H
#define LEG_H

#include "little_helpers/Object.h"
#include "Servo.h"
#include "little_helpers/Definitions.h"
#include "little_helpers/gnublin.h"
#define BOARD RASPBERRY_PI
#include <mutex>
#include <condition_variable>
#include <atomic>

/**
 * \brief Controls the Servos of a Leg
 * \author JosefSommerauer, Patrick Felixberger
 * \date 2013/12/04
 */
class Leg: public Object {
public:

	/*
	 * \brief CTor
	 * \author Josef Sommerauer
	 * \param Femur Servo of Femur
	 * \param Tibia Servo of Tibia
	 * \param Tarsus Servo of Tarsus
	 */
	Leg(int Nr, int uCAddress, T3DPosition Ori, Servo const& Femur,
			Servo const& Tibia, Servo const& Tarsus);

	/*
	 * \brief DTor
	 * \author Josef Sommerauer
	 */
	~Leg();

	// setParams , getParams ...

	/*
	 * \brief 	thread for single leg, 6time open
	 * \author	Lukas Plazovnik
	 * \param
	 * \return --
	 */
	void Leg_Thread(int GaitOrder);

	/*
	 * \brief  excecute 1 step
	 * \author	Lukas Plazovnik
	 * \param
	 * \return 1 ==success
	 */
	bool Leg_Step(TAngles nextPosition);
	/*
	 * \brief Set Angle of Each Servo
	 * \author Lukas Plazovnik
	 * \return angle of each servo and set mValid
	 */

	bool SetAngle(TAngles Angles);

	/*
	 * \brief Get Angle of Each Servo
	 * \author Lukas Plazovnik
	 * \return angle of each servo and set mValid
	 */
	TAngles GetAngle();

	/*
	 * \brief	returns default position (normal stand) of leg.
	 * \author	Josef Sommerauer
	 * \return 	default 3D Positions
	 */
	T3DPosition GetOriginPos() const;

	/*
	 * \brief	returns default position (normal stand) of leg.
	 * \author	Lukas Plazovnik
	 * \return 	returns the current postion
	 */
	T3DPosition GetCurrentPosition() const;

	/*
		 * \brief	returns default position (normal stand) of leg.
		 * \author	Lukas Plazovnik
		 * \return 	returns the current postion
		 */
		void SetCondVar(int LegID);

	/*
	 * \brief	turns of pwm generation for each servo of leg.
	 * \author	Josef Sommerauer
	 * \return 	in case of an driver error.
	 */
	bool Off();

	/*
	 * \brief 	pull in leg. parking position.
	 * \author	Josef Sommerauer
	 */
	void Sleep();

	/*
	 * \brief 	returns true if current position is true;
	 * \author	Josef Sommerauer
	 */
	bool isValid();

	/**
	 * \brief	send angel via uart
	 * \author	Lukas Plazovnik
	 * \param
	 * \return	false if could be send
	 */
	bool SendAngle();
	/**
	 * \brief	receive data  from i2c slave
	 * \author	Lukas Plazovnik
	 * \param
	 * \return	false if could be received
	 */
	bool RequestAngle();
	bool compareInt(int A, int B);
private:
	//i2c access
	gnublin_i2c i2c;


	/// leg number
	int mNr;
	/// position on hexapod
	T3DPosition mOriginPos;
	T3DPosition mCurrentPos;
	/// Femur-Servo
	Servo mFemur;
	/// Tibia-Servo
	Servo mTibia;
	/// Tarsus-Servo
	Servo mTarsus;

//	/// Femur-Servo
//		Servo mGoalFemur;
//		/// Tibia-Servo
//		Servo mGoalTibia;
//		/// Tarsus-Servo
//		Servo mGoalTarsus;
	//mutex for i2c
		std::mutex coutMutex;

		 std::mutex mCondMutex;
		std::mutex AngleMutex;



	bool mNewGoalPosition;
	//postion is valid true=valid, false=invalid(position in mFemur, mTibia, mTarsus is old)
	bool mValid;
};

#endif // LEG_H
