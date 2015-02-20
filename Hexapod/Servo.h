/*
 * Servo.h
 *
 *  Created on: 02.07.2013
 *      Author: josef, patrick
 */
#ifndef SERVO_H_
#define SERVO_H_

#include "little_helpers/Object.h"
// api used for i2c http://www.raspberrypi.org/forums/viewtopic.php?f=33&t=43725

//#include "gnublin.h"
#include <string>
#include "little_helpers/gnublin.h"
#define BOARD RASPBERRY_PI

/**
 * \brief	Interface between Software and Kernelmodule
 * \author	Josef Sommerauer
 * \date	2013/07/02
 */
class Servo: public Object {
public:
	/**
	 * \brief	CTor
	 * \author	Josef Sommerauer
	 deleted//////////// \param	Dev			Block Device of Servo
	 \param	uCAddress	Address of uC
	 \param	ServoID 	ID of Serv(0=Femur, 1=Tibia, 2=Tarsus)
	 deleted//////////// \param	PWM_Min		Minimal PWM-Value
	 deleted//////////// \param	PWM_Max		Maximal PWM-Value
	 * \param	Angle_Min	Minmal Angle
	 * \param	Angle_Max	Maximal Angle
	 */
	Servo(unsigned char ServoID, double Angle_Min,
			double Angle_Max);


	/**
	 * \brief	DTor
	 * \author	Josef Sommerauer
	 */
	~Servo();

	/**
	 * \brief	sets the angle of the servo.
	 * \author	Lukas Plazovnik
	 * \param	value	angle in degree
	 * \return	false if angle is out of range or in case of an driver error.
	 */
	bool SetAngle(double const& value);

	/**
	 * \brief	send angel via uart
	 * \author	Lukas Plazovnik
	 * \param
	 * \return	false if could be send
	 */


	/**
	*\brief get factor for calculation pwm<-->angle
	*
	*
	*
       	* return   value of factor
	*/
	double GetAngle_Factor() const;



	int GetAngle() const;

	/**
	 * \brief	return the maximal angle of the servo.
	 * \author	Josef Sommerauer
	 * \return maximal angle of the servo
	 */
	int GetMaxAngle() const;

	/**
	 * \brief	return the minimal angle of the servo.
	 * \author	Josef Sommerauer
	 * \return minimal angle of the servo
	 */
	int GetMinAngle() const;

	/**
	 * \brief	deactivates servo, stop to send pwm values to servo.
	 * \author	Josef Sommerauer
	 * \return false in case of an driver error.
	 */
	bool Off();

	/**
	 * \brief		calibrate servo. the user has the enter + and - to find the maximal und minimal angle of the servo.
	 * \author		Josef Sommerauer
	 * \attention	{only works on pandaboard with connected servos}
	 */
	void Calibrate();

	/**
	 * \brief	prints all settings of an servo.
	 * \author	Josef Sommerauer
	 * \return	false in case of an driver error.
	 */
	void Print(std::ostream & o) const;

private:

	/**
	 * \brief	sets the pwm value for a servo.
	 * \author	Josef Sommerauer
	 * \param	value pwm value
	 * \return	false if pwm is out of range or in case of an driver error.
	 */
	//bool SetPWM(int value);
	/// Stream to write on Blockdevice
	//deleted/////std::ostream *mDev;
	// Address for i2c
	//gnublin_i2c i2c;
	/// Minimal PWM Value
	//deleted/////int mPWM_Min;
	/// Maximal PWM Value
	//deleted/////int mPWM_Max;
	//i2c adreess
	//gnublin_i2c i2c;

	/// Minimal Angle
	double mAngle_Min;

	/// Maximal Angle
	double mAngle_Max;
	
	/// factor to calculate angle<-->pwm
	double mAngle_factor;

	/// Current Angle
	double mAngle;

	//ID of Servo on current Leg
	unsigned char mServoID;

};

#endif /* SERVO_H_ */
