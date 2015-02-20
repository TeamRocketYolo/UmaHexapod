/////////////////////////////////////////////////////////////////////////////
// Workfile    : TripodGait_Response.h
// Author      : Luaks Plazovnik
// Date        :
// Description :
// Project	   : RoboMotion
// Revision    :
/////////////////////////////////////////////////////////////////////////////

#include "Movement.h"


#define down 0
#define up 2
#define moveground 1
#define moveair 3

/// Implementation of Tripod Gait Movement
class TripodGait_Response : public Movement{

public:
   TripodGait_Response(LegCtrl * lc) : Movement(lc) {}

private:
   void UpdateGoal(){}
void Start_Legs(){}

	/*
	 * Proceed single Step
	 * @return 0 if successfully; Errorcode else
	 */
	int Step();


};

