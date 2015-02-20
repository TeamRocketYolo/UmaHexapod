/////////////////////////////////////////////////////////////////////////////
// Workfile    : TripodGait_Response.cpp
// Author      : Lukas Plazovnik
// Date        :
// Description :
// Project	   :
// Revision    :
/////////////////////////////////////////////////////////////////////////////

#include "TripodGait_Response.h"

#include <unistd.h>



/*
 * Proceed single Step in groups
 * waits for
 * @return 0 if successfully; Errorcode else
 */

int TripodGait_Response::Step() {

	int LegCounter = 0;
	bool finished = 0;

	int WalkStateCounter = 0;


//no calculation, just moving
	while (!finished) {

		if (LegCounter == 0) {
			switch (WalkStateCounter) {
			case 0: //g1 up
				for (int i = 0; i <= 4; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][up]);
				}


				std::cout << "Group1, feets up" << std::endl;
				WalkStateCounter++;
				LegCounter += 3;
				break;

			case 1: //g2 move ground
				for (int i = 1; i <= 5; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][moveground]);
				}
				std::cout << "Group2, feets moveground" << std::endl;
				LegCounter += 3;
				WalkStateCounter++;

				break;

			case 2: //g1 move air
				for (int i = 0; i <= 4; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][moveair]);
				}
				std::cout << "Group1, feets moveair" << std::endl;

				LegCounter += 3;
				WalkStateCounter++;

				break;

			case 3: //g1 move down
				std::cout << "Group1, feets down"
						<< std::endl;
				for (int i = 0; i <= 4; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][down]);
				}

				LegCounter += 3;
				WalkStateCounter++;

				break;

			case 4: //g2 up
				std::cout << "Group2, feets up" << std::endl;
				for (int i = 1; i <= 5; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][up]);
				}

				LegCounter += 3;
				WalkStateCounter++;
				break;

			case 5: //g1 move ground
				std::cout << "Group1, feets moveground"
						<< std::endl;
				for (int i = 0; i <= 4; i += 2) {
					mLegCtrl->SetXYZ(i,
							mMovePos[i][moveground]);
				}

				LegCounter += 3;
				WalkStateCounter++;

				break;

			case 6: //g2 move air
				std::cout << "Group2, feet: , moveair"
						<< std::endl;
				for (int i = 1; i <= 5; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][moveair]);
				}

				LegCounter += 3;
				WalkStateCounter++;
				break;

			case 7: //g2 move down
				std::cout << "Group2, feet: down"
						<< std::endl;
				for (int i = 1; i <= 5; i += 2) {
					mLegCtrl->SetXYZ(i, mMovePos[i][down]);
				}
				LegCounter += 3;
				WalkStateCounter++;

				finished = 1;
				*mNewCommand = false;
				std::cout << "finished \n";

				break;
			default:
				break;
			}
		} else {

			sleep(1); //wait for foots moved
			LegCounter = 0;
		}

	}
	std::cout << "walk finished" << std::endl;
	return 0;
}


