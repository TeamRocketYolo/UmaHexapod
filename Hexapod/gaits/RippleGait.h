/////////////////////////////////////////////////////////////////////////////
// Workfile    : RippleGait.h										
// Author      : Daniel Wolfmayr										
// Date        : 2013/10/23															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "Movement.h"

/**
 * \brief	Implementation of Ripple Gait Movement
 * \author	Daniel Wolfmayr
 * \date	2013/10/23
 */
class RippleGait : public Movement{
public:
	/**
	 * \brief	CTor
	 * \author	Daniel Wolfmayr
	 * \param	lc	Leg Control used to Move
	 */
    RippleGait(LegCtrl * lc) : Movement(lc) {}

private:
    void UpdateGoal(){}
    void Start_Legs(){}
	/**
	 * \brief	Proceed single Step
	 * \author	Florian Aigner
	 * \return	0 if successfully; Errorcode else
	 */
	int Step();
};
