#include <cmath>
#include <iostream>
#include <iomanip>

#include <iostream>

#include <cmath>

#include "Parser.h"
#include "InverseKinematic.h"

using namespace std;

/** \brief	Default CTor
 * \author	Patrick Felixberger
 */
InverseKinematic::InverseKinematic() {
   // Create Fileparser
   Parser parser(hexa_size);

   // Parse File to Get Hexapod Dimensions
   if(parser.ParseFile()) {
	   parser.GetData(mSizeData);
	   cout << "Parsed hexapod geometry successfully!" << endl;
   } else {
	   // Throw Exception on Error
	   throw "LegCtrl::LegCtrl(): can not open: Hexapod.txt";
   }
} // CTor

/** \brief Default DTor
 * \author Patrick Felixberger
 */
InverseKinematic::~InverseKinematic() {}

/** \brief	Calculates the angles of a leg
 * \author	Patrick Felixberger
 * \param	ID 	Number of leg
 * \param	Pos Position of leg
 * \return	Angles of leg
 */


TAngles InverseKinematic::CalculateAnglesServoIK(int ID, T3DPosition const &Pos) const {
	// See sheet "InverseKinematik" in documentation!

	TAngles tmp;
    TBodyPos nPos={0,0,0};

    double dist = 0.0;
    double c = 0.0;

    CalculateBodyIK(ID, nPos, Pos);

    double tau = 0;
    double gamma = 0;
    double rho = 0;
   // std::cout << "absolute target position: " << nPos.BodyPosX << nPos.BodyPosY << nPos.BodyPosZ << std::endl;
	// right and left side need different values
    if(ID > 2) {    // right side; legs 3, 4, 5
        // Calculate lengths
        dist = sqrt(pow(nPos.BodyPosX - mSizeData.NB, 2) + pow(nPos.BodyPosY, 2));
        c = sqrt(pow(nPos.BodyPosZ, 2) + pow(dist, 2));

		// calculate angles
        rho   = AngleFemur(nPos.BodyPosX, -nPos.BodyPosY);
        tau   = AngleTibia(mSizeData.a, mSizeData.b, c, nPos.BodyPosZ, dist);
        gamma = AngleTarsus(mSizeData.a, mSizeData.b, c);

    } else {		// left side
		// calculate lengths
        dist = sqrt(pow(nPos.BodyPosX + mSizeData.NB, 2) + pow(nPos.BodyPosY, 2));
        c =    sqrt(pow(nPos.BodyPosZ, 2) + pow(dist, 2));

		// calculate angles
        rho   = AngleFemur(nPos.BodyPosX, nPos.BodyPosY);
        tau   = AngleTibia(mSizeData.a, mSizeData.b, c, nPos.BodyPosZ, dist);
        gamma = AngleTarsus(mSizeData.a, mSizeData.b, c);

    }

//	// Debug info - remove later
//	std::cout << "[Leg:" << ID << "]"
//             << std::setiosflags (std::ios::fixed)
//             << std::setprecision(0)
//             << " x:" << std::setw(4) << Pos.X
//             << " y:" << std::setw(4) << Pos.Y
//             << " z:" << std::setw(4) << Pos.Z
//             << std::fixed << std::setprecision(0) << " [Angles]"
//             << " Rho:"   << std::setw(4) << rho
//             << " Tau:"   << std::setw(4) << tau
//             << " Gamma:" << std::setw(4) << gamma << endl;



	tmp.AngleFemur  = rho;
	tmp.AngleTibia  = tau;
	tmp.AngleTarsus = gamma;

    return tmp;
}


/** \brief	Calculates the angles of the legs for body
         * \author	Lukas Plazovnik
         * \param
         * \param
         * \return
         *///todo: implement body ik(currently for 1 not for all)
void InverseKinematic::CalculateBodyIK(int ID,TBodyPos & CurrentPos, T3DPosition const &Pos)const{


	switch(ID) {
	        case 0:

	        	CurrentPos.BodyPosX = Pos.X + mSizeData.OM;
	        	CurrentPos.BodyPosY = Pos.Y + mSizeData.OQ;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;

	        case 1:
	        	CurrentPos.BodyPosX = Pos.X + mSizeData.ON;
	        	CurrentPos.BodyPosY = Pos.Y;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;

	        case 2:
	        	CurrentPos.BodyPosX = Pos.X + mSizeData.OM;
	        	CurrentPos.BodyPosY = Pos.Y - mSizeData.OQ;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;

	        case 3:
	        	CurrentPos.BodyPosX = Pos.X - mSizeData.OM;
	        	CurrentPos.BodyPosY = Pos.Y - mSizeData.OQ;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;

	        case 4:
	        	CurrentPos.BodyPosX = Pos.X - mSizeData.ON;
	        	CurrentPos.BodyPosY = Pos.Y;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;

	        case 5:
	        	CurrentPos.BodyPosX = Pos.X - mSizeData.OM;
	        	CurrentPos.BodyPosY = Pos.Y + mSizeData.OQ;
	        	CurrentPos.BodyPosZ= Pos.Z;
	            break;
	    }


//	TODO: better body ik template, need to be converted for our system
    	/* BodyIK [PosX, PosZ, PosY, BodyOffsetX, BodyOffsetZ, RotationY]
    	   ;Calculating totals from center of the body to the feet
    	   TotalZ = BodyOffsetZ+PosZ
    	   TotalX = BodyOffsetX+PosX
    	   ;PosY are equal to a "TotalY"

    	   ;Successive global rotation matrix:
    	   ;Math shorts for rotation: Alfa (A) = Xrotate, Beta (B) = Zrotate, Gamma (G) = Yrotate
    	   ;Sinus Alfa = sinA, cosinus Alfa = cosA. and so on...

    	   ;First calculate sinus and cosinus for each rotation:
    	   gosub GetSinCos [TOFLOAT(BodyRotX)]
    	   sinG = sinA
    	   cosG = cosA
    	   gosub GetSinCos [TOFLOAT(BodyRotZ)]
    	   sinB = sinA
    	   cosB = cosA
    	   gosub GetSinCos [TOFLOAT(BodyRotY+RotationY)]

    	   ;Calcualtion of rotation matrix:
    	   BodyIKPosX = TotalX-TOINT(TOFLOAT(TotalX)*cosA*cosB - TOFLOAT(TotalZ)*cosB*sinA + TOFLOAT(PosY)*sinB)
    	   BodyIKPosZ = TotalZ-TOINT(TOFLOAT(TotalX)*cosG*sinA + TOFLOAT(TotalX)*cosA*sinB*sinG +TOFLOAT(TotalZ)*cosA*cosG-TOFLOAT(TotalZ)*sinA*sinB*sinG-TOFLOAT(PosY)*cosB*sinG)
    	   BodyIKPosY = PosY - TOINT(TOFLOAT(TotalX)*sinA*sinG - TOFLOAT(TotalX)*cosA*cosG*sinB + TOFLOAT(TotalZ)*cosA*sinG + TOFLOAT(TotalZ)*cosG*sinA*sinB + TOFLOAT(PosY)*cosB*cosG)
    	  return*/

     }




double InverseKinematic::AngleFemur(double const &x, double const &ny) const {
    double rho = 0.0;

    if(ny != 0) {
        rho = atan(ny / x) * 57.2957795131;//180 / M_PI);
    } else
        rho = 0.0;

    return rho;
}

double InverseKinematic::AngleTibia(double const &a, double const &b,
                                    double const &c, double const &z,
                                    double const &dist) const {

    double x = (pow(c, 2) + pow(a, 2) - pow(b, 2)) / (2*c*b);

    if(x > 1.0 || x < -1.0) {
//		std::cout << "            ++++++++Tibia: invalid pos+++++++" << std::endl;
//		std::cout << "            a:" << a << ";b:" << b <<";c:" << c << ";z:" << z << ";dist:" << dist << endl;
    }

    double beta = acos( x ) * 57.2957795131;//180 / M_PI;

    double tau = atan(dist / abs(z)) * 57.2957795131;//180 / M_PI;
    tau += beta;
    tau -= 90;

    return tau;
}

double InverseKinematic::AngleTarsus(double const &a, double const &b,
                                     double const &c) const {

    double x = (pow(b, 2) + pow(a, 2) - pow(c, 2)) / (2*a*b);

    if(x > 1.0 || x < -1.0) {
//		std::cout << "              ++++++++Tarsus: invalid pos+++++++|" << std::endl;
//		std::cout << "              a:" << a << ";b:" << b <<";c:" << c << endl;
    }

    double gamma = acos( x ) * 57.2957795131;//* 180 / M_PI;

    gamma -= 90;

    return gamma;
}
