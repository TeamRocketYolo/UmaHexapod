/*
 * @file    : Movement.h
 * @author  : Daniel Wolfmayr
 * @date    : 2013/10/09
 *
 * Baseclass for Movements
 */

// Changelog:
//		- 2013-11-20 Florian Aigner: Redesign of Movement Classes

#include "Movement.h"

#include "../little_helpers/Definitions.h"
#include "../LegCtrl.h"

#include <vector>
#include <string>

using namespace std;

/*
 * \brief	CTor
 * \author	Daniel Wolfmayr
 * \param	lc	Legcontrol
 */
Movement::Movement(LegCtrl * lc) : mLegCtrl(lc), mdh(h), mds(s){
	mNewCommand= new bool;
}


int Movement::MoveAnyDirection(double const& phi, double const& radius, int rotate){
//rotate=0, 1=left, 2 right
	//!!!double-->int cast
	 mLegCtrl->CalcMotionPos(mdh, radius, phi, rotate);
	return 0;
}

/*
 * \brief	Move Hexapod forward
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::MoveForward()
{

	 mLegCtrl->CalcMotionPos(mdh, mds, phiForward,0);
	return Step();
}

//int Movement::MoveFrontRight(){
//
//}


/*
 * \brief	Move Hexapod backward
 * \author	Florian Aigner
 * \return 0 if Successfull; Errorcode else
 */
int Movement::MoveBackward()
{

 mLegCtrl->CalcMotionPos(mdh, mds, phiBackward,0);
	return Step();
}

/*
 * \brief	Move Hexapod to left side
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::MoveLeftSide()
{
	 mLegCtrl->CalcMotionPos(mdh, mds, phiLeft,0);
	return Step();
}

/*
 * \brief	Move Hexapod to right side
 * \author 	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::MoveRightSide()
{
	 mLegCtrl->CalcMotionPos(mdh, mds, phiRight,0);

	return Step();
}

/*
 * \brief	Rotate Hexapod to left side
 * \author	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::RotateLeft()
{
  // mMovePos = mLegCtrl->CalcRotatePos(mdh, alphaRotateLeft);

	return Step();
}

/*
 * \brief	Rotate Hexapod to right side
 * \author	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::RotateRight()
{
  // mMovePos = CalcRotatePos(mdh, alphaRotateRight);
   
	return Step();
}

/*
 * \brief	Move Legs to Sleep Position
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::Sleep()
{
	mLegCtrl->CalcMotionPos(0, 0, 0,0);
	return 0;
}

/*
 * \brief	Move Legs to Startposition
 * \author	Florian Aigner
 * @return 0 if Successfull; Errorcode else
 */
int Movement::WakeUp()
{
	mLegCtrl->CalcMotionPos(mdh, mds, 0,0);


	return 0;
}

int Movement::setNewCommand(bool * NewCommand){


	return 0;
}

/*
 * \brief	Set Height of Movement
 * \author	Florian Aigner
 * \param 	h Height
 */
void Movement::Setdh(int h)
{
	mdh = h;
}

/*
 * \brief	Get Height of Movement
 * \author	Florian Aigner
 * \return 	Height
 */
int Movement::Getdh()
{
	return mdh;
}

/*
 * \brief	Set Spatium
 * \author	Florian Aigner
 * \param 	s Spatium
 */
void Movement::Setds(int s)
{
	mds = s;
}

/*
 * \brief	Get Spatium
 * \author	Florian Aigner
 * \return	Spatium
 */
int Movement::Getds()
{
	return mds;
}

/*
 * \brief	Calculate Leg-Positions to do Movemennt!
 *  		new:**Can be overloaded in derived Class!
 * \author	Josef Sommerauer/Lukas Plazovnik
 * \param	h	height of Hexapod
 * \param	r	Step-width of Hexapod(not yet implemented)
 * \param	phi	Direction to Move
 */
//std::vector<std::vector<T3DPosition> > Movement::CalcMotionPos(
//	int h, int r, double const &phi) {
//
//	// Calculate delta X and delta Y Position
//	vector<vector<T3DPosition> > retVal;
//	int x = cos ( phi * 0.01745329251 ) * abs(r);//M_pie/180
//	int y = sin ( phi * 0.01745329251 ) * abs(r);
//
//	// Calculate Targetpositions of each Leg
//	for (int i=0;i<NUM_OF_LEGS;i++) {
//		vector<T3DPosition> tmpVec;
//		T3DPosition tmp;
//
//
//
//		//mLegCtrl->
//		T3DPosition pos = mLegCtrl->GetCurrentLegPosition(i);
//
//
//
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//		// pos 4
//		tmp.X = pos.X - x;
//		tmp.Y = pos.Y - y;
//		tmp.Z = pos.Z + h;
//		tmpVec.push_back(tmp);//finsihed next steps for 1 leg
//
//		//std::cout << tmp.X <<" "<< tmp.Y << " "<< tmp.Z<<std::endl;
//		retVal.push_back(tmpVec);
//	}
//std::cout << "new Targetpositions successfully calculated" << std::endl;
//	return retVal;
//}

///*
// * \brief	Calculate Leg-Positions to do Rotation
// * \author	Josef Sommerauer
// * \param	h		Height of Hexapod
// * \param	alpha	Direction to Rotate
// */
//std::vector<std::vector<T3DPosition> > Movement::CalcRotatePos(
//      int h, double const &alpha) {
//
//	vector<vector<T3DPosition> > retVal;
//
//	// Calculate Targetpositions of each Leg
//	for (int i=0;i<NUM_OF_LEGS;i++) {
//		vector<T3DPosition> tmpVec;
//		T3DPosition tmp;
//
//		T3DPosition pos =mLegCtrl->GetCurrentLegPosition(i);
//		//T3DPosition pos = mLegs[i]->GetOriginPos();
//
//		// calc absolute value and phase of xy-legposition
//		double absVal = sqrt(pow(pos.X, 2) + pow(pos.Y, 2));
//		double phi    = atan(pos.X / pos.Y) * 57.2957795131;
//
//		if (pos.Y != 0 && pos.X < 0) phi += 180; // 2. half of circuit
//		else if (pos.Y == 0) phi += 270;
//
//		std::cout << "x,y:"    << std::setw(4) << pos.X << " " << std::setw(4) << pos.Y << " "
//			   << "angle:"  << std::setw(4) << phi << " "
//			   << "absVal:" << std::setw(4) << absVal << " new => ";
//
//
//		// pos 1
//		tmp.X = absVal * cos((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
//		tmp.Y = absVal * sin((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
//		tmp.Z = pos.Z;
//		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
//		tmpVec.push_back(tmp);
//
//		// pos 2
//		tmp.X = absVal * cos((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
//		tmp.Y = absVal * sin((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
//		tmp.Z = pos.Z;
//		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
//		tmpVec.push_back(tmp);
//
//		// pos 3
//		tmp.X = absVal * cos((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
//		tmp.Y = absVal * sin((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
//		tmp.Z = pos.Z + h;
//		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
//		tmpVec.push_back(tmp);
//
//		// pos 4
//		tmp.X = absVal * cos((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
//		tmp.Y = absVal * sin((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
//		tmp.Z = pos.Z + h;
//		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
//		tmpVec.push_back(tmp);
//
//		retVal.push_back(tmpVec);
//		std::cout << std::endl;
//	}
//
//	return retVal;
//}
