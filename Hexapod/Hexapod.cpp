/////////////////////////////////////////////////////////////////////////////
// Workfile    : Hexapod.cpp											
// Author      : Daniel Wolfmayr										
// Date        : 2013/11/21															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "Hexapod.h"
#include <iostream>
#include <unistd.h>

// terminate programm proper
bool run = true;

/// Handler to Catch CTRL + C to terminate Program
void handler(int signum) {
	std::cout << std::endl << "+++ [Ctrl] + C caught: exit programm! +++"
			<< std::endl;
	run = false;
}

//! CTor
/*!
 \param PortNbr Portnumber of Server
 \param lc Textfile for servo-definition
 */
Hexapod::Hexapod(int PortNbr, LegCtrl * lc) :
		mSocket(PortNbr), mCmd(CtrlSocket::TCmd::nocmd), newCommand(0) {

	// Init Movement-Algorithmns
	Movement * pTripod = new TripodGait(lc);
	Movement * pWave = new WaveGait(lc);
	Movement * pRipple = new RippleGait(lc);
	Movement * pTripod_Response = new TripodGait_Response(lc);
	Movement * pRippleGait_Response = new RippleGait_Response(lc);

	// Add Algorithmns to Container
	mMov.push_back(pTripod);
	mMov.push_back(pWave);
	mMov.push_back(pRipple);
	mMov.push_back(pTripod_Response);
	mMov.push_back(pRippleGait_Response);

	// Set Tripod-Gait as Default
	mpGait = mMov[RIPPLE_RESPONSE];

	mContPos.R = 0;
	mContPos.X = 0;
	mContPos.Y = 0;
	mContPos.theta = 0;
	CommandID = 0;
	MyController = new XBOXController;

} // CTor

//! DTor
Hexapod::~Hexapod() {
	// Remove all Algorithmns from Container
	for (size_t i = 0; i < mMov.size(); i++) {
		delete mMov[i];
		mMov[i] = 0;
	}
	delete MyController;
} // DTor

//! starts the server and the Hexapod and interprets the commands from server
/*!
 \return a "1" in case of stop-command
 */

//! setGait
/*! Lukas Plazovnik
 \param enum Gait Movement/predefined
 */
void Hexapod::SetGait(TGait Gait) {
	mpGait = mMov[Gait];
}
int Hexapod::CurrentPos() {
//			if(MyController->IsButtonPressed(BUTTON_A)){
//				if(MyController->IsButtonPressed(BUTTON_B)){return 0;}
//				else{return 1;}
//			}
//			else if(MyController->IsButtonPressed(BUTTON_X)){
//				return 2;
//			}
	return 0;
}

void Hexapod::Run() {

	joining_thread Thread2([&] {RunHexapod_Thread();});
	joining_thread Thread1([&] {ControllerThread();});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	joining_thread Thread_Leg0([&] {mpGait->mLegCtrl->LegCtrl_Thread(0);});
	joining_thread Thread_Leg1([&] {mpGait->mLegCtrl->LegCtrl_Thread(1);});
	joining_thread Thread_Leg2([&] {mpGait->mLegCtrl->LegCtrl_Thread(2);});
	joining_thread Thread_Leg3([&] {mpGait->mLegCtrl->LegCtrl_Thread(3);});
	joining_thread Thread_Leg4([&] {mpGait->mLegCtrl->LegCtrl_Thread(4);});
	joining_thread Thread_Leg5([&] {mpGait->mLegCtrl->LegCtrl_Thread(5);});
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	joining_thread Thread_Gait([&] {mpGait->Step();});

}
void Hexapod::RunHexapod_Thread() {
	// Open Socket
	//run = mSocket.Run();

	// Connect Signal Handler
	//signal(SIGINT, handler);

	std::cout << "Debbuging Version for Hexapod: " << std::endl;
	std::cout << "Program successfully started... " << std::endl;

	std::cout << "ready to receive input from xbox controller..." << std::endl;
//	mpGait->setNewCommand(&newCommand);	//testing purpose

	std::cout << "GOOOOOOD MOOOORNING!!!!!" << std::endl;
	mpGait->WakeUp();
	while (true) {

		{

			std::cout << "new command calculated" << std::endl;
			{
				std::unique_lock<std::mutex> lck(mCommandMutex);
				mCondVar_newCommand.wait(lck);


				if (CommandID == BUTTON_START) {
					mpGait->WakeUp();
				} else if (CommandID == BUTTON_BACK) {
					mpGait->Sleep();
				} else if (CommandID == BUTTON_LB) {
					mpGait->MoveAnyDirection(mContPos.theta, mContPos.R, 1);
				}else if (CommandID == BUTTON_RB) {
					mpGait->MoveAnyDirection(mContPos.theta, mContPos.R, 2);
				} else if (CommandID == MOVING) {

					mpGait->MoveAnyDirection(mContPos.theta, mContPos.R, 0); //CALC WITH THE CURRENT VALUES
					//std::lock_guard<std::mutex> commandLock(mCommandMutex);

				}

			}

		}

	}

	return;
}
bool IfDifferent(js_position const& oldPos, js_position const& newPos) {

int oldt = oldPos.theta;
int newt = newPos.theta;
if (oldt - newt > 10 || oldt - newt < -10) {

	return true;
}

return false;
}
void Hexapod::ControllerThread() {

XBOXController MyController;
std::cout << "controller thread started" << std::endl;
while (true) {
	MyController.readEvent(); //should block until new command received

	{
		//std::lock_guard<std::mutex> commandLock(mCommandMutex);

		js_position oldPos = mContPos;
		js_position newPos = MyController.GetPosition(POS_LSTICK);

		if (MyController.IsButtonPressed(BUTTON_START)) {

			newCommand = true;
			CommandID = BUTTON_START;
			mCondVar_newCommand.notify_one();
		} else if (MyController.IsButtonPressed(BUTTON_BACK)) {
			newCommand = true;
			CommandID = BUTTON_BACK;
			mCondVar_newCommand.notify_one();

		} else if (MyController.IsButtonPressed(BUTTON_LB)) {
			newCommand = true;
			CommandID = BUTTON_LB;
			mCondVar_newCommand.notify_one();

		}
		else if (MyController.IsButtonPressed(BUTTON_RB)) {
			newCommand = true;
			CommandID = BUTTON_RB;
			mCondVar_newCommand.notify_one();

		}
		else if (IfDifferent(oldPos, newPos)) {
			mContPos = newPos;
			newCommand = true;
			CommandID = MOVING;
			mCondVar_newCommand.notify_one();

			//MyController.DoRumble(5);
		}

//			else if (newPos.R < DEAD_ZONE_LEFT_JOYSTICK) {
//				newPos.theta=0;
//				mContPos = newPos;
//				newCommand = true;
//				mCondVar_newCommand.notify_one();
//			}

	}

	std::this_thread::sleep_for(std::chrono::milliseconds(300));

}

}
/*
 while(true){
 // Read Command
 mCmd = mSocket.GetCmd();

 // end server on ctrl-c
 if(!run) mCmd = CtrlSocket::shutdown;

 //interpret command
 switch (mCmd)
 {
 // Move Left
 case CtrlSocket::moveleft:
 mpGait->MoveLeftSide();
 break;

 // Move Right
 case CtrlSocket::moveright:
 mpGait->MoveRightSide();
 break;

 // Move Forward
 case CtrlSocket::moveforward:
 mpGait->MoveForward();
 break;

 // Move Backward
 case CtrlSocket::movereverse:
 mpGait->MoveBackward();
 break;

 // Turn to the left Side
 case CtrlSocket::turnleft:
 mpGait->RotateLeft();
 break;

 // Turn to the right Side
 case CtrlSocket::turnright:
 mpGait->RotateRight();
 break;

 // Select Tripod Gait
 case CtrlSocket::tripod:
 mpGait = mMov[TRIPOD];
 break;

 // Select Wave Gait
 case CtrlSocket::wave:
 mpGait = mMov[WAVE];
 break;

 // Select Ripple Gait
 case CtrlSocket::ripple:
 mpGait = mMov[RIPPLE];
 break;

 // Send Hexapod to Standby
 case CtrlSocket::sleep:
 mpGait->Sleep();
 break;

 // Wakeup Hexapod
 case CtrlSocket::wakeup:
 mpGait->WakeUp();
 break;

 // Do nothing
 case CtrlSocket::nocmd:
 break;

 // shutdown Server
 case CtrlSocket::shutdown:
 mpGait->Sleep();
 usleep(500000);
 mSocket.Stop();
 return 0;
 break;

 // Socketerror
 case CtrlSocket::error:
 std::cout << std::endl << "+++ Error in Socket Communication +++" << std::endl;
 break;

 // No Valid Command -> do nothing
 default:
 break;
 }
 }

 return 1;
 } // RunHexapod
 */
