#include "Leg.h"
#include <unistd.h>

//std::mutex Leg::i2cMutex;
/*
 * \brief CTor
 * \author Josef Sommerauer
 * \param Femur Servo of Femur
 * \param Tibia Servo of Tibia
 * \param Tarsus Servo of Tarsus
 */
Leg::Leg(int Nr, int uCAddress, T3DPosition Ori, Servo const& Femur,
		Servo const& Tibia, Servo const& Tarsus) :
		mNr(Nr), mOriginPos(Ori), mFemur(Femur), mTibia(Tibia), mTarsus(Tarsus) {

	mValid = false;
	mNewGoalPosition = false;

	mCurrentPos.X = 0;
	mCurrentPos.Y = 0;
	mCurrentPos.Z = 0;

	if (i2c.setAddress(uCAddress) == -1) {
		//std::cout << "error in setting Address i2c for 0x" <<uCAddress <<std::endl;

	} else {
		//std::cout << "Adress: " << uCAddress << " sucessfully set for Leg: " << Nr << std::endl;
	}

	//set i2c port(/dev/i2c-0 and /dev/i2c-1) and address(hex)
	if (i2c.setDevicefile("/dev/i2c-1") == -1) {
		std::cout << "error in opening Device File" << std::endl;
	}

}

bool Leg::compareInt(int A, int B) {
	{
		std::lock_guard<std::mutex> tempMutex(coutMutex);
		return A == B;
	}

}
void Leg::Leg_Thread(int GaitOrder) {

}

bool Leg::Leg_Step(TAngles nextPosition) {

	return 0;
}
/*
 * \brief DTor
 * \author Josef Sommerauer
 */
Leg::~Leg() {
}

/*
 * \brief 	sets the angles of servos.
 * \author	Josef Sommerauer
 * \param 	Angles a set of 3 angles. one angle for each servo.
 * \return 	false if angle is out of range or in case of an driver error.
 */

bool Leg::SetAngle(TAngles Angles) {
//	std::cout << "Leg::SetAngle " << Angles.AngleFemur << std::endl;
//	std::cout << "Leg::SetAngle " << Angles.AngleTibia << std::endl;
//	std::cout << "Leg::SetAngle " << Angles.AngleTarsus << std::endl;

	{
		std::lock_guard<std::mutex> tempMutex(AngleMutex);

		mFemur.SetAngle(Angles.AngleFemur);  //usleep(200000);
		mTibia.SetAngle(Angles.AngleTibia);   //usleep(200000);
		mTarsus.SetAngle(Angles.AngleTarsus); //usleep(200000);
		mNewGoalPosition = true;
	}

	return false;
}

unsigned char calcChecksum(unsigned char Buffer[], int size) {
	unsigned char cs = Buffer[0];
//std::cout << "so ein ultrascheissdreck" << std::endl;
	for (int i = 1; i < size - 1; i++) {
		cs ^= Buffer[i];
		//std::cout <<"checksum " << (unsigned int) cs << std::endl;
	}

	return cs;
}

/**
 * \brief	send angle via i2c
 * \author	Lukas Plazovnik
 * \param
 * \return	false if couldn't be send
 */
bool Leg::SendAngle() {

	unsigned char buffer[4] = { 0, 0, 0, 0 }; //char == byte

	unsigned char val0 = ((double) mFemur.GetAngle() + 90)
			* (double) mFemur.GetAngle_Factor();

	unsigned char val1 = ((double) mTibia.GetAngle() + 90)
			* (double) mTibia.GetAngle_Factor();



	unsigned char val2 = ((double) mTarsus.GetAngle() + 90)
			* (double) mTarsus.GetAngle_Factor();
//toDo: hotfix, servos sind falsch rum
	if (mNr > 2) {
			val0= 255- val0;
			val1 = 255 - val1;
			val2 = 255 - val2;
		}
//	   std::cout << val0 << " "<< val1 << " "<< val2 << std::endl;

	buffer[0] = val0;
	buffer[1] = val1;
	buffer[2] = val2;

//	  std::cout << (double)buffer[0] << " "<< (double)buffer[1] << " "<< (double)buffer[2] << std::endl;

	buffer[3] = calcChecksum(buffer, 4);

	//	std::cout << "calc factor :    " <<mFemur.GetAngle_Factor()<< std::endl;

	while (!i2c.send(buffer, 4)) {

	}

	return true;

}

/**
 * \brief	receive data  from slave
 * \author	Lukas Plazovnik
 * \param
 * \return	false if could be received
 */
bool Leg::RequestAngle() {
	//possible to read a register from uC?
	//i2c.receive(RegisterAddress, RxBuf, length);
	unsigned char rxbuffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	while (!i2c.receive(rxbuffer, 7)) {
	}

	std::cout << "Leg Nr.  " << mNr << " received Angles from " << std::hex
			<< i2c.getAddress() << std::endl;
	std::cout << "rx0:" << (unsigned int) rxbuffer[0] << ", rx1:"
			<< (unsigned int) rxbuffer[1] << ", rx2:"
			<< (unsigned int) rxbuffer[2] << ", pos1:"
			<< (unsigned int) rxbuffer[3] << ", pos2:"
			<< (unsigned int) rxbuffer[4] << ", pos3:"
			<< (unsigned int) rxbuffer[5] << ", cs:"
			<< (unsigned int) rxbuffer[6] << std::endl;

	{
		std::lock_guard<std::mutex> tempMutex(AngleMutex);
		mFemur.SetAngle(rxbuffer[3]);
		mTibia.SetAngle(rxbuffer[4]);
		mTarsus.SetAngle(rxbuffer[5]);
		mValid = true;

	}
	return true;
}

/*
 * \brief Get Angle of Each Servo
 * \author Lukas Plazovnik
 * \return angle of each servo.
 */
TAngles Leg::GetAngle() {

	TAngles tmp;

	if (mValid == false) {
		RequestAngle();

	}
	tmp.AngleFemur = mFemur.GetAngle();
	tmp.AngleTibia = mTibia.GetAngle();
	tmp.AngleTarsus = mTarsus.GetAngle();

	return tmp;
}

/*
 * \brief	returns default position (normal stand) of leg.
 * \author	Josef Sommerauer
 * \return 	default 3D Positions
 */
T3DPosition Leg::GetOriginPos() const {
	return mOriginPos;
}

T3DPosition Leg::GetCurrentPosition() const {

	//RequestAngle();

	//calc from angles to x,y,z
	//mCurrentPos.
	return mCurrentPos;
}
/*
 * \brief	turns of pwm generation for each servo of leg.
 * \author	Josef Sommerauer
 * \return 	in case of an driver error.
 */
bool Leg::Off() {
	return mFemur.Off() && mTibia.Off() && mTarsus.Off();
}

/*
 * \brief 	pull in leg. parking position.
 * \author	Josef Sommerauer
 */
void Leg::Sleep() {
	mFemur.SetAngle(0);
	mTibia.SetAngle(mTibia.GetMaxAngle());
	mTarsus.SetAngle(mTarsus.GetMinAngle());
}

bool Leg::isValid() {
	return mValid;
}
