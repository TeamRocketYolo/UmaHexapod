/*
 * @file    : Movement.h
 * @author  : Daniel Wolfmayr
 * @date    : 2013/10/09
 *
 * Baseclass for Movements
 */

// Changelog:
//		- 2013-11-20 Florian Aigner: Redesign of Movement Classes
#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED

#include "../LegCtrl.h"

#include <vector>

size_t const t = 1000000;

int const h = 25;
int const s = 20;

/*
 * Define Angles for Movement
 */
int const phiForward = 90;
int const phiBackward = 270;
int const phiLeft = 0;
int const phiRight = 180;

int const alphaRotateLeft = 5;
int const alphaRotateRight = -5;

/*
 * \brief	Base-Class for all Movements
 * \author	Daniel Wolfmayr
 * \date	2013/10/09
 */
class Movement: public Object {
public:
	/*
	 * \brief	CTor
	 * \author	Daniel Wolfmayr
	 * \param	lc	Legcontrol
	 */
	Movement(LegCtrl * lc);

	/*
	 * \brief	DTor
	 * \author	Daniel Wolfmayr
	 */
	virtual ~Movement() {
		delete mNewCommand;
	}
	;

	/*
	 * \brief	Calculate Leg-Positions to do Rotation
	 * \author	Josef Sommerauer
	 * \param	h		Height of Hexapod
	 * \param	alpha	Direction to Rotate
	 */
	std::vector<std::vector<T3DPosition> > CalcRotatePos(int h,
			double const &alpha);

	//Run control
	/*
	 * \brief	Move Hexapod forward
	 * \author	Florian Aigner
	 * \return 	0 if Successfull; Errorcode else
	 */
	int MoveForward();

	/*
	 * \brief	Move Hexapod in any direction
	 * \author	Lukas Plazovnik
	 *
	 * 		90=forward
	 * 0=left		180=right
	 * 		270=back
	 *
	 * \return 	0 if Successfull; Errorcode else
	 */
	int MoveAnyDirection(double const &phi,double const& radius, int rotate);

	/*
	 * \brief	Move Hexapod backward
	 * \author	Florian Aigner
	 * \return 0 if Successfull; Errorcode else
	 */
	int MoveBackward();

	/*
	 * \brief	Move Hexapod to left side
	 * \author	Florian Aigner
	 * \return 	0 if Successfull; Errorcode else
	 */
	int MoveLeftSide();

	/*
	 * \brief	Move Hexapod to right side
	 * \author 	Florian Aigner
	 * \return	0 if Successfull; Errorcode else
	 */
	int MoveRightSide();

	/*
	 * \brief	Rotate Hexapod to left side
	 * \author	Florian Aigner
	 * \return	0 if Successfull; Errorcode else
	 */
	int RotateLeft();

	/*
	 * \brief	Rotate Hexapod to right side
	 * \author	Florian Aigner
	 * \return	0 if Successfull; Errorcode else
	 */
	int RotateRight();

	/*
	 * \brief	Move Legs to Sleep Position
	 * \author	Florian Aigner
	 * \return 	0 if Successfull; Errorcode else
	 */
	int Sleep();

	/*
	 * \brief	Move Legs to Startposition
	 * \author	Florian Aigner
	 * @return 0 if Successfull; Errorcode else
	 */
	int WakeUp();

	/*
	 * \brief	Set Height of Movement
	 * \author	Florian Aigner
	 * \param 	h Height
	 */
	void Setdh(int h);

	/*
	 * \brief	Get Height of Movement
	 * \author	Florian Aigner
	 * \return 	Height
	 */
	int Getdh();

	/*
	 * \brief	Set Spatium
	 * \author	Florian Aigner
	 * \param 	s Spatium
	 */
	void Setds(int s);

	/*
	 * \brief	Get Spatium
	 * \author	Florian Aigner
	 * \return	Spatium
	 */
	int Getds();

	int setNewCommand(bool * NewCommand);




protected:

//	/*
//	 * \brief	Calculate Leg-Positions to do Movement
//	 * \author	Josef Sommerauer
//	 * \param	h	height of Hexapod
//	 * \param	r	Step-width of Hexapod
//	 * \param	phi	Direction to Move
//	 */
//	virtual std::vector<std::vector<T3DPosition> > CalcMotionPos(int h, int r,
//			double const &phi);

public:
	/// Class controlling Legs
	LegCtrl * mLegCtrl;

	/// Positions to Move
	std::vector<std::vector<T3DPosition> > mMovePos;

	/*
	 * \brief	Proceed Single Step
	 * \author	Florian Aigner
	 * \return 	0 is Successfull; Errorcode else
	 */
	virtual int Step() = 0;
	virtual void UpdateGoal()=0;
	virtual void Start_Legs()=0;

protected:
	//new command flag
	bool *mNewCommand;
	std::condition_variable mStepFinished;

private:

	/// Height to Lift Legs
	int mdh;


	/// Spatium to Move Legs
	int mds;
};

#endif
