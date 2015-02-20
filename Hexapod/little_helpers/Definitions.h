/////////////////////////////////////////////////////////////////////////////
// Workfile    : Definitions.h
// Author      : Brennsteiner / Jung / Felixberger
// Date        : 2014/01/--
// Description : SP 1/1:
// Project	   : RoboSoft3G | Robomotion
// Remarks     : -
// Revision    : 1
////////////////////////////////////////////////////////////////////////////
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//# define WIN_32
# include <string>
# include <vector>
# include <cmath>
//#include <math.h>
#include <stdlib.h>




#define backzenith 1
#define zenithend 2
#define endback 3
#define def 0


/// Define Number of Legs
int const NUM_OF_LEGS = 6;

/// Define Servos per Leg
int const SERVOS_PER_LEG = 3;

/// Define Number of servo Values in Structure
int const NUM_OF_SERVO_VALS = 3;

// forward declarations
class LegDefinition;

////////////////////////////////////////////////////
// D E F I N I T I O N S :
////////////////////////////////////////////////////

/// Define Precision of 3D Position
static const double T3DVectorDefaultPrecision = 0.01;

/// A vector for 3D calculations:
template <class T>
struct T3DVector{
	/// the X, Y and Z coordinates of the vector originating at (0,0,0)
	T X, Y, Z;
	/// if a coordinate's value is less it is considered zero
	double Precision;

	/// helper function
	size_t whichiszero() const{
		if((abs(Y) <= Precision) && (abs(Z) <= Precision)) return 4;
		if (abs(Y) <= Precision) return 2;		// Frontview
		if (abs(Z) <= Precision) return 3;		// Topview
		if (abs(X) <= Precision) return 1;
		return 0;
	}

	/// CTor
	/// @param x X-Coordinate
	/// @param y Y-Coordinate
	/// @param z Z-Coordinate
	/// @param precision Precision of Position
	T3DVector(T x = 0, T y = 0, T z = 0,
		double precision = T3DVectorDefaultPrecision) :
			X(x), Y(y), Z(z), Precision(precision) {}

	/// assignment operator
	T3DVector& operator= (const T3DVector &vect){
		if (this != &vect)
		{
			X = vect.X;
			Y = vect.Y;
			Z = vect.Z;
		}
		return *this;
	}

	/// binary operators

	/// Subtract Operator
	const T3DVector operator- (const T3DVector &vect) const{
		T3DVector temp;
		temp.X = X - vect.X;
		temp.Y = Y - vect.Y;
		temp.Z = Z - vect.Z;
		return temp;
	}

	/// Add Operator
	const T3DVector operator+ (const T3DVector &vect) const{
		T3DVector temp;
		temp.X = X + vect.X;
		temp.Y = Y + vect.Y;
		temp.Z = Z + vect.Z;
		return temp;
	}

	/// compound operators

	/// Add and Assign
	T3DVector & operator+=(const T3DVector &vect){
		X += vect.X;
		Y += vect.Y;
		Z += vect.Z;
		return *this;
	}

	/// Subtract and Assign
	T3DVector & operator-=(const T3DVector &vect){
		X -= vect.X;
		Y -= vect.Y;
		Z -= vect.Z;
		return *this;
	}
};

/// Type of 3D Position
// positions are stored with double precision floating point values
typedef T3DVector<double> T3DPosition;

/// Angles of Legs
struct TAngles{

	/// CTor
	/// @param fe Femur Angle
	/// @param ti Tibia Angle
	/// @param ta Tarsus Angle
	TAngles(double fe=0.0, double ti=0.0, double ta=0.0) :
	   /// Femur Servo
	   AngleFemur(fe),
	   /// Tibia Servo
	   AngleTibia(ti),
	   /// Tarsus
	   AngleTarsus(ta) {};

	/// Femur Angle
	double AngleFemur;
	/// Tibia Angle
	double AngleTibia;
	/// Tarsus Angle
	double AngleTarsus;
};


//type of 3d Position for Hexa body
//for Rotation
struct TBodyPos{

	double BodyPosX;
	double BodyPosY;
	double BodyPosZ;

};

/// Set of Angles
struct TAngleSet{
	/// Container storint Angles
	std::vector<TAngles> Angles;
};

/// Define Precission
static const double PRECISSION = 0.01;

/// Structure storing Position
struct TPosition{
	/// Comparation Operator
	bool operator ==(const TPosition & rhs){
		return this->X == rhs.X && this->Y == rhs.Y;
	}

	/// X Coordinate
	double X;

	/// Y Coordinate
	double Y;
};

/// Geometric Data of Hexapod
struct HexapodSize {
	// distance between femur tibia
	double NB;

	// x pos offset for legs 2
    double ON;

    // x pos offset for legs 1 3 4 6
    double OM;

    // y offset for legs 1 3 4 6
    double OQ;

    // femur length
    double a;

    // crus lenght
    double b;
};

/// Default Leg Position
typedef T3DPosition DefaultLegPos[NUM_OF_LEGS];


#endif
