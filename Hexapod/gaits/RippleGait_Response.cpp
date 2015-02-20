/////////////////////////////////////////////////////////////////////////////
// Workfile    : RippleGait_Response.cpp
// Author      : Lukas Plazovnik
// Date        :
// Description :
// Project	   :
// Revision    :
/////////////////////////////////////////////////////////////////////////////

#include "RippleGait_Response.h"

#include <unistd.h>

void RippleGait_Response::UpdateGoal() {

}

void RippleGait_Response::Start_Legs() {

}
void NotifyLeg(int LegNumber) {

}
/*
 * Proceed single Step in groups
 * waits for
 * @return 0 if successfully; Errorcode else
 */
int RippleGait_Response::Step() {

	while (true) {

		//std::cout << mLegCtrl->mLegs[0]->

		std::cout << "one" << std::endl;
		mLegCtrl->Excecute_Step(0, backzenith);
		mLegCtrl->Excecute_Step(2, backzenith);
		mLegCtrl->Excecute_Step(4, backzenith);
		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {
						bool tmp=(mLegCtrl->mLegZenith[0]&&mLegCtrl->mLegZenith[2]&&mLegCtrl->mLegZenith[4]);
						return tmp;
					});
			mLegCtrl->mLegZenith[0] = 0;
			mLegCtrl->mLegZenith[2] = 0;
			mLegCtrl->mLegZenith[4] = 0;

		}
		mLegCtrl->Excecute_Step(1, endback);
		mLegCtrl->Excecute_Step(3, endback);
		mLegCtrl->Excecute_Step(5, endback);
		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {
						bool tmp=(mLegCtrl->mLegFinished[1]&&mLegCtrl->mLegFinished[3]&&mLegCtrl->mLegFinished[5]);
						return tmp;
					});
			mLegCtrl->mLegFinished[1] = 0;
			mLegCtrl->mLegFinished[3] = 0;
			mLegCtrl->mLegFinished[5] = 0;
		}
		mLegCtrl->Excecute_Step(0, zenithend);
		mLegCtrl->Excecute_Step(2, zenithend);
		mLegCtrl->Excecute_Step(4, zenithend);

		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {
						bool tmp=(mLegCtrl->mLegEnd[0])&&(mLegCtrl->mLegEnd[2])&&(mLegCtrl->mLegEnd[4]);
						return tmp;
					});
			mLegCtrl->mLegEnd[0] = 0;
			mLegCtrl->mLegEnd[2] = 0;
			mLegCtrl->mLegEnd[4] = 0;
		}

		std::cout << "two" << std::endl;
		mLegCtrl->Excecute_Step(1, backzenith);
		mLegCtrl->Excecute_Step(3, backzenith);
		mLegCtrl->Excecute_Step(5, backzenith);
		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {

						bool tmp=(mLegCtrl->mLegZenith[1])&&(mLegCtrl->mLegZenith[3])&&(mLegCtrl->mLegZenith[5]);
						return tmp;

					});
			mLegCtrl->mLegZenith[1] = 0;
			mLegCtrl->mLegZenith[3] = 0;
			mLegCtrl->mLegZenith[5] = 0;
		}

		mLegCtrl->Excecute_Step(0, endback);
		mLegCtrl->Excecute_Step(2, endback);
		mLegCtrl->Excecute_Step(4, endback);

		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {
						bool tmp=(mLegCtrl->mLegFinished[0])&&(mLegCtrl->mLegFinished[2])&&(mLegCtrl->mLegFinished[4]);
						return tmp;
					});
			mLegCtrl->mLegFinished[0] = 0;
			mLegCtrl->mLegFinished[2] = 0;
			mLegCtrl->mLegFinished[4] = 0;
		}

		mLegCtrl->Excecute_Step(1, zenithend);
		mLegCtrl->Excecute_Step(3, zenithend);
		mLegCtrl->Excecute_Step(5, zenithend);
		{
			std::unique_lock<std::mutex> lck(mLegCtrl->mCondMutex);
			mLegCtrl->mCondVar_LegEvent.wait(lck,
					[&] {

						bool tmp=(mLegCtrl->mLegEnd[1])&&(mLegCtrl->mLegEnd[3])&&(mLegCtrl->mLegEnd[5]);
						return tmp;

					});
			mLegCtrl->mLegEnd[1] = 0;
			mLegCtrl->mLegEnd[3] = 0;
			mLegCtrl->mLegEnd[5] = 0;
		}
	}
	return 0;
}

//std::vector<std::vector<T3DPosition> > RippleGait_Response::CalcMotionPos(int h,
//		int r, double const &phi) {
//
//	// Calculate delta X and delta Y Position
//	vector<vector<T3DPosition> > retVal;
//	int x = cos(phi * 0.01745329251) * abs(r); //M_pie/180
//	int y = sin(phi * 0.01745329251) * abs(r);
//
//	// Calculate Targetpositions of each Leg
//	for (int i = 0; i < NUM_OF_LEGS; i++) {
//		vector<T3DPosition> tmpVec;
//		T3DPosition tmp;
//
//		//mLegCtrl->
//		T3DPosition pos = mLegCtrl->GetCurrentLegPosition(i);
//
//
//
//		//std::cout << "new leg"<<std::endl;
//
//		// pos 1
//		tmp.X = pos.X - x;
//		tmp.Y = pos.Y - y;
//		tmp.Z = pos.Z;
//		tmpVec.push_back(tmp);
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//		// pos 2
//		tmp.X = pos.X + x;
//		tmp.Y = pos.Y + y;
//		tmp.Z = pos.Z;
//		tmpVec.push_back(tmp);
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//
//		// pos 3
//		tmp.X = pos.X + x;
//		tmp.Y = pos.Y + y;
//		tmp.Z = pos.Z + h;
//		tmpVec.push_back(tmp);
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//		// pos 4
//		tmp.X = pos.X - x;
//		tmp.Y = pos.Y - y;
//		tmp.Z = pos.Z + h;
//		tmpVec.push_back(tmp);
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//		retVal.push_back(tmpVec);
//	}
//
//	std::cout << "new Targetpositions successfully calculated" << std::endl;
//	return retVal;
//}
