//#include <curses.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include "little_helpers/Definitions.h" // PI
#include "Servo.h"
#include "LegCtrl.h"
#include "little_helpers/Parser.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>

using namespace std;

/**
 * \brief	Default CTor
 * \author	Josef Sommerauer
 * \param servo_dev Path to Servo-Interface
 */
LegCtrl::LegCtrl(std::string const & servo_dev) {
	// Open servo file stream
	/*mFs.open(servo_dev.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	 if(!mFs.is_open())
	 throw "LegCtrl::LegCtrl(): can not open: " + servo_dev;
	 */

	mLegCounter = new atomic<bool> [6];
	mLegZenith = new atomic<bool> [6];
	mLegFinished = new atomic<bool> [6];
	mLegEnd = new atomic<bool> [6];
	mLegCommando = new atomic<int> [6];
	mMovePos = new std::vector<T3DPosition>[6];

	// Read default leg pos
	mPars = new Parser("/home/Hexapod/DefaultLegPos.txt");

	if (mPars && mPars->ParseFileDefaultPos() && mPars->GetData(mDefLegPos)) {
		cout << "Parsed default position successfully!" << endl;
	} else {
		throw "LegCtrl::LegCtrl(): can not open: DefaultLegPos.txt";
	}
	if (mPars)
		delete mPars;

	// Read servo values
	mPars = new Parser("/home/Hexapod/ServoValues.txt");

	if (mPars && mPars->ParseFileServos() && mPars->GetData(mServoValues)) {
		cout << "Parsed servo values successfully!" << endl;
	} else {
		throw "LegCtrl::LegCtrl(): can not open: ServoValues.txt";
	}
	if (mPars)
		delete mPars;

	// build legs
	for (int i = 0; i < NUM_OF_LEGS; ++i) {
		mLegs[i] = new Leg(i, mServoValues[i * 3][0], mDefLegPos[i],

		// id, angle_min, angle_max
		// Femur
				Servo(1, mServoValues[(3 * i) + 0][1],
						mServoValues[(3 * i) + 0][2]),
				// Tibia
				Servo(2, mServoValues[(3 * i) + 1][1],
						mServoValues[(3 * i) + 1][2]),
				// Tarsus
				Servo(3, mServoValues[(3 * i) + 2][1],
						mServoValues[(3 * i) + 2][2]));

	}

	std::cout << "legctrl successfully started" << std::endl;
	mReady = false;
}

/**
 * \brief	Default DTor
 * \author	Josef Sommerauer
 */
LegCtrl::~LegCtrl() {
	for (int i = 0; i < NUM_OF_LEGS; i++) {
		mLegs[i]->Off();
		delete mLegs[i];
	}
	delete[] mLegCounter;
	delete[] mLegFinished;
	delete[] mLegZenith;
	delete[] mLegEnd;
	delete[] mMovePos;
	delete[] mLegCommando;
	mFs.close();
}

void LegCtrl::Excecute_Step(int LegID, int LegCommand) {
	mLegCounter[LegID] = 1;
	mLegCommando[LegID] = LegCommand;
	//std::unique_lock<std::mutex> lck1(mCondMutex);

	mCondVar_LegExcecute.notify_all();
	//std::cout <<"woke up all excecute"<< std::endl;
}

void LegCtrl::LegCtrl_Thread(int LegID) {

	//std::cout << " Leg_thread" << LegID << "started" << std::endl;
	while (true) {
		{
//			std::cout << "LegCtrl_thread: " << LegID << " go to bed with: "
//					<< mLegCounter[LegID] << std::endl;
			std::unique_lock<std::mutex> lck(mCondMutex);
			mCondVar_LegExcecute.wait(lck, [&] {
				bool tmp =mLegCounter[LegID];
				return tmp;
			});


			mLegFinished[LegID] = 0;
			mLegCounter[LegID] = 0;
			//std::cout << "LegCtrl_thread: " << LegID << " woke up: " << mLegCounter[LegID]<< std::endl;
		}
//start of step
		int Commando = mLegCommando[LegID];
		switch (Commando) {

		case backzenith:
			for (int i = 0; i < 7; i++) {
				{
					std::lock_guard<std::mutex> PositionLock(mPosMutex);
					SetXYZ(LegID, mMovePos[LegID][i]);

					//NEED BOTH MUTEX FOR SENDING, BECAUSE setxyz writes movepos,
					//and senangle reads it
					std::lock_guard<std::mutex> i2cLock(i2cMutex);
					mLegs[LegID]->SendAngle();
				}
				//wait until leg finished
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
			}

			mLegZenith[LegID] = 1;
			mCondVar_LegEvent.notify_all();
			break;
		case zenithend:
			for (int i = 7; i < 14; i++) {
				{
					std::lock_guard<std::mutex> PositionLock(mPosMutex);
					SetXYZ(LegID, mMovePos[LegID][i]);

					std::lock_guard<std::mutex> i2cLock(i2cMutex);
					mLegs[LegID]->SendAngle();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
			}
			mLegEnd[LegID] = 1;
			mCondVar_LegEvent.notify_all();
			break;
		case endback:
			for (int i = 14; i < 20; i++) {
				{
					std::lock_guard<std::mutex> PositionLock(mPosMutex);
					SetXYZ(LegID, mMovePos[LegID][i]);

					std::lock_guard<std::mutex> i2cLock(i2cMutex);
					mLegs[LegID]->SendAngle();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
			}
			//end of step
			mLegFinished[LegID] = 1;
			mCondVar_LegEvent.notify_all();

			break;
		case def:
		default: {
			std::lock_guard<std::mutex> PositionLock(mPosMutex);
			SetXYZ(LegID, mMovePos[LegID][19]);

			std::lock_guard<std::mutex> i2cLock(i2cMutex);
			mLegs[LegID]->SendAngle();
		}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			//simulation

		}

	}

}
/** \brief	Sets a leg to the specified position
 * \author	Josef Sommerauer
 * \param 	ID 	Number of leg
 * \param 	Pos Koordinates of the leg
 * \return 	True if function succeedes
 */
bool LegCtrl::SetXYZ(int ID, T3DPosition const &Pos) {
	if (ID < 0 || ID >= NUM_OF_LEGS)
		throw "LegCtrl::SetXYZ: ID out of range";

	m3DPosLeg[ID] = Pos; // save last position
	return mLegs[ID]->SetAngle(mInvKin.CalculateAnglesServoIK(ID, Pos));
	//return mLegs[ID]->SetAngle(mInvKin.CalculateAnglesServoIK(ID, Pos));
}

/**
 * \brief	Sets the angles of a leg
 * \author	Josef Sommerauer
 * \param	ID	Number of leg
 * \param	Ang	Angle set of a leg
 * \return	True if function succeeded
 */
bool LegCtrl::SetAngle(int ID, TAngles const &Ang) {
	if (ID < 0 || ID >= NUM_OF_LEGS)
		return false;
	std::cout << "legCtrl::SetAngle " << std::endl;
	return mLegs[ID]->SetAngle(Ang);
}

/**
 * \brief	Returns the coordinates of a leg
 * \author	Josef Sommerauer
 * \param	ID	Number of leg
 * \return	Current coordinates of a leg
 */
T3DPosition LegCtrl::GetXYZServo(int ID) const {
	if (ID < 0 || ID >= NUM_OF_LEGS)
		throw "LegCtrl::GetXYZ: ID out of range";

	return m3DPosLeg[ID];
}

/**
 * \brief	Returns the angles of a leg//check valid before
 * \author	Josef Sommerauer
 * \param	ID Number of leg
 * \return	angles of a leg
 */
TAngles LegCtrl::GetAngles(int ID) const {
	if (ID < 0 || ID >= NUM_OF_LEGS)
		throw "LegCtrl::GetAngles: ID out of range";

	return mLegs[ID]->GetAngle();

}
/**
 * \brief	necessary to valid if the postion is current
 * \author	Lukas Plazovnik
 * \param	ID Number of leg
 * \return	returns true if valid
 */
bool LegCtrl::isLegValid(int ID) {
	return mLegs[ID]->isValid();

}
/**
 * \brief	Sets all legs to a default position
 * \author	Josef Sommerauer
 */
void LegCtrl::ReadyPosition() {
	// set 1. pair of legs
	for (int i = 0; i < NUM_OF_LEGS; i += 2) {
		m3DPosLeg[i] = mLegs[i]->GetOriginPos();
		mLegs[i]->SetAngle(mInvKin.CalculateAnglesServoIK(i, m3DPosLeg[i]));
	}

	// set 1. pair of legs
	for (int i = 1; i < NUM_OF_LEGS; i += 2) {
		m3DPosLeg[i] = mLegs[i]->GetOriginPos();
		mLegs[i]->SetAngle(mInvKin.CalculateAnglesServoIK(i, m3DPosLeg[i]));
	}
	std::cout << "Ready Position finsihed" << std::endl;

	mReady = true;
}

/*
 * \brief	Checks if Leg Ctrl is ready
 * \author	Josef Sommerauer
 * \return	True if legs are ready
 */
bool LegCtrl::IsReady() const {
	return mReady;
}

/**
 * \brief	Disables all legs
 * \author	Josef Sommerauer
 */
void LegCtrl::AllOff() {
	for (int i = 0; i < NUM_OF_LEGS; i += 2) {
		mLegs[i]->Off();
	}

	for (int i = 1; i < NUM_OF_LEGS; i += 2) {
		mLegs[i]->Off();
	}
}

/**
 * \brief	Sets all legs to sleep mode
 * \author	Josef Sommerauer
 */
void LegCtrl::AllSleep() {
	for (int i = 0; i < NUM_OF_LEGS; i++) {
		mLegs[i]->Sleep();
	}
}
T3DPosition LegCtrl::GetCurrentLegPosition(int const& i) {
	return mLegs[i]->GetCurrentPosition();

}

bool LegCtrl::UpdateLegPosition(int const& i) {
	return mLegs[i]->RequestAngle();
}

/*
 * \brief	Calculate Leg-Positions to do Movement
 * \author	Josef Sommerauer
 * \param	h	height of Hexapod
 * \param	r	Step-width of Hexapod
 * \param	phi	Direction to Move
 */
void LegCtrl::CalcMotionPos(int h, int radius, double const &phi, int rotate) {

	// Calculate delta X and delta Y Position
	vector<vector<T3DPosition> > retVal;
	double x;
	double y;
	int r;

	if (radius <= 6000 &&rotate==0) {
		x = 0;
		y = 0;
		r = 0;
		h = 0;
	} else {
		if (radius > 6000 && radius <= 20000) {
			r = 10;
		} else {
			r = 20;
		}
		x = cos(phi * 0.01745329251) * r;
		y = sin(phi * 0.01745329251) * r;

	}

//	std::cout << "phi: " << phi << std::endl;
//	std::cout << "y: " << y << std::endl;
//	std::cout << "x: " << x << std::endl;
//	std::cout << "h: " << h << std::endl;
//	std::cout << "r: " << r << std::endl;
//	if (x == 0) {
//		h = 0;
//	}

	/*h=25
	 * x=-19.7087
	 * y= -3.40104
	 * phi 189,791
	 * r=20
	 */

	// Calculate Targetpositions of each Leg
	{
		std::lock_guard<std::mutex> coutLock2(mPosMutex);
		for (int i = 0; i < NUM_OF_LEGS; i++) {
			vector<T3DPosition> tmpVec;
			T3DPosition tmp;

			T3DPosition pos = mLegs[i]->GetOriginPos();

			//next try

			if (i > 2) {
				if (rotate == 1) { //left
					y=-y;
				}
				h=10;
			}
			else{
				if (rotate == 2) { //left
									y=-y;
					}
				h=10;
			}
			// pos 1
//start position

////
//			std::cout << "position for calc" << pos.X << pos.Y << pos.Z
//					<< std::endl;
			//back zenith
			tmp.X = pos.X;
			tmp.Y = pos.Y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.10;
			tmp.Z = pos.Z - h * 0.32;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.20;
			tmp.Z = pos.Z - h * 0.48;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.35;
			tmp.Z = pos.Z - h * 0.64;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.40;
			tmp.Z = pos.Z - h * 0.80;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.5;
			tmp.Z = pos.Z - h * 0.90;
			tmpVec.push_back(tmp);

			//tmpVec.push_back(tmp);

			// pos 2

//zenit position
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.6;
			tmp.Z = pos.Z - h;
			tmpVec.push_back(tmp);
//zenithend
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.65;
			tmp.Z = pos.Z - h * 0.90;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.70;
			tmp.Z = pos.Z - h * 0.80;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.75;
			tmp.Z = pos.Z - h * 0.64;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.8;
			tmp.Z = pos.Z - h * 0.48;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.85;
			tmp.Z = pos.Z - h * 0.32;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.90;
			tmp.Z = pos.Z - h * 0.16;
			tmpVec.push_back(tmp);
//end back

			// pos 3
			tmp.X = pos.X;
			tmp.Y = pos.Y - y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			//tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.80;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.60;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.40;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.20;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.10;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			//tmpVec.push_back(tmp);
//standard
			mMovePos[i] = tmpVec;
			//retVal.push_back(tmpVec);
		}
	}

}

/*
 * \brief	Calculate Leg-Positions to do Rotation
 * \author	Josef Sommerauer
 * \param	h		Height of Hexapod
 * \param	alpha	Direction to Rotate
 */
void LegCtrl::CalcRotatePos(int h, int radius, double const &alpha) {

	// Calculate delta X and delta Y Position
	vector<vector<T3DPosition> > retVal;
	double x;
	double y;
	int r;

	if (radius <= 6000) {
		x = 0;
		y = 0;
		r = 0;
		h = 0;
	} else {
		if (radius > 6000 && radius <= 20000) {
			r = 10;
		} else {
			r = 20;
		}
		x = cos(alpha * 0.01745329251) * r;
		y = sin(alpha * 0.01745329251) * r;

	}

//	std::cout << "phi: " << phi << std::endl;
//	std::cout << "y: " << y << std::endl;
//	std::cout << "x: " << x << std::endl;
//	std::cout << "h: " << h << std::endl;
//	std::cout << "r: " << r << std::endl;
//	if (x == 0) {
//		h = 0;
//	}

	/*h=25
	 * x=-19.7087
	 * y= -3.40104
	 * phi 189,791
	 * r=20
	 */

	// Calculate Targetpositions of each Leg
	{
		std::lock_guard<std::mutex> coutLock2(mPosMutex);
		for (int i = 0; i < NUM_OF_LEGS; i++) {
			vector<T3DPosition> tmpVec;
			T3DPosition tmp;

			T3DPosition pos = mLegs[i]->GetOriginPos();

			//next try

			// pos 1
//start position

////
//			std::cout << "position for calc" << pos.X << pos.Y << pos.Z
//					<< std::endl;
			//back zenith
			tmp.X = pos.X;
			tmp.Y = pos.Y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.10;
			tmp.Z = pos.Z - h * 0.32;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.20;
			tmp.Z = pos.Z - h * 0.48;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.35;
			tmp.Z = pos.Z - h * 0.64;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.40;
			tmp.Z = pos.Z - h * 0.80;
			tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.5;
			tmp.Z = pos.Z - h * 0.90;
			tmpVec.push_back(tmp);

			//tmpVec.push_back(tmp);

			// pos 2

//zenit position
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.6;
			tmp.Z = pos.Z - h;
			tmpVec.push_back(tmp);
//zenithend
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.65;
			tmp.Z = pos.Z - h * 0.90;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.70;
			tmp.Z = pos.Z - h * 0.80;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.75;
			tmp.Z = pos.Z - h * 0.64;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.8;
			tmp.Z = pos.Z - h * 0.48;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.85;
			tmp.Z = pos.Z - h * 0.32;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.90;
			tmp.Z = pos.Z - h * 0.16;
			tmpVec.push_back(tmp);
//end back

			// pos 3
			tmp.X = pos.X;
			tmp.Y = pos.Y - y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			//tmpVec.push_back(tmp);
			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.80;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.60;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.40;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.20;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y - y * 0.10;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);

			tmp.X = pos.X;
			tmp.Y = pos.Y;
			tmp.Z = pos.Z;
			tmpVec.push_back(tmp);
			//tmpVec.push_back(tmp);
//standard
			mMovePos[i] = tmpVec;
			//retVal.push_back(tmpVec);
		}
	}

}

//void LegCtrl::TestSetXYZ() {
//   T3DPosition pos;
//   int Xoff = 0;
//   int Yoff = 0;
//   int Zoff = 0;
//
//   char c = 0;
//
//   initscr();
//
//   while(c!='q') {
//
//      for(int i=0;i<NUM_OF_LEGS;++i) {
//         pos = mLegs[i]->GetOriginPos();
//         if(i<3) pos.X += Xoff;
//         else    pos.X -= Xoff;
//         pos.Y += Yoff;
//         pos.Z += Zoff;
//         printw(" %i\n",SetXYZ(i, pos));
//      }
//
//      printw("\n");
//      printw("Xoff: %i",Xoff); printw(" ");
//      printw("Yoff: %i",Yoff); printw(" ");
//      printw("Zoff: %i",Zoff); printw("\n");
//      refresh();
//
//      c = getch();
//      switch(c) {
//	      case 'a' : Yoff++;break;
//	      case 'y' : Yoff--;break;
//
//	      case 's' : Xoff++;break;
//	      case 'x' : Xoff--;break;
//
//	      case 'd' : Zoff++;break;
//	      case 'c' : Zoff--;break;
//
//	      default: break;
//      }
//      clear();
//
//    }
// endwin();
//}
