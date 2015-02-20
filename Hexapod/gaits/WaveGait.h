/////////////////////////////////////////////////////////////////////////////
// Workfile    : WaveGait.h										
// Author      : Daniel Wolfmayr										
// Date        : 2013/10/23															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "Movement.h"

/// Implementation of Wave Gait Movement
class WaveGait : public Movement{
public:
    WaveGait(LegCtrl * lc) : Movement(lc) {}

private:
    void UpdateGoal(){}
       	void Start_Legs(){}
	/*
	 * Proceed single Step
	 * @return 0 if successfully; Errorcode else
	 */
	int Step();
};
