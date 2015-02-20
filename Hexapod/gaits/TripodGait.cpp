/////////////////////////////////////////////////////////////////////////////
// Workfile    : TripodGait.cpp										
// Author      : Daniel Wolfmayr										
// Date        : 2013/10/16															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "TripodGait.h"

#include <unistd.h>

/*
 * Proceed single Step
 * @return 0 if successfully; Errorcode else
 */
int TripodGait::Step()
{		//no calculation, just moving
	std::cout << "start the step" << std::endl;
	
	// group 1 - up
	for (size_t i=0;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][2]);
	usleep(t);

	// group 2 - move ground
	for (size_t i=1;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][1]);

	usleep(t);

	// group 1 - move air
	for (size_t i=0;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][3]);
	usleep(t);

	// group 1 - down
	for (size_t i=0;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][0]);
	usleep(t);



	// group 2 - up
	for (size_t i=1;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][2]);
	usleep(t);

	// group 1 - move ground
	for (size_t i=0;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][1]);
	usleep(t);

	// group 2 - move air
	for (size_t i=1;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][3]);
	usleep(t);

	// group 2 - down
	for (size_t i=1;i<mMovePos.size();i+=2)
		mLegCtrl->SetXYZ(i, mMovePos[i][0]);
	usleep(t);


	std::cout << "i did a step and i liked it :D:D:D:D:D" << std::endl;
	return 0;
}
