///////////////////////////////////////////////////////////////////////////
// Workfile    : InverseKinematic.h
// Author      : Patrick Felixberger, Josef Sommerauer
// Date        :
// Description : Calculates leg positions
// Remarks     : -
// Revision    : 0
///////////////////////////////////////////////////////////////////////////
#ifndef INVERSEKINEMATIC_H
#define INVERSEKINEMATIC_H

#include "little_helpers/Definitions.h"
#include "little_helpers/Parser.h"
#include "little_helpers/Object.h"

std::string const hexa_size = "/home/Hexapod/Hexapod.txt";    /**< File with hexapod dimensions */


/** \brief
 * 		class InverseKinematic.
 * 		Calculates the angles of the current position.
 *
 * 	\author Patrick Felixberger, Josef Sommerauer
 * 	\date 	2013/12/04
 */
class InverseKinematic : public Object {
public:
    /** \brief	Default CTor
     * \author	Patrick Felixberger
     */
    InverseKinematic();

    /** \brief Default DTor
     * \author Patrick Felixberger
     */
    ~InverseKinematic();


    TAngles CalculateLegIK_new(int ID, T3DPosition const &Pos) const;
    /** \brief	Calculates the angles of a leg
     * \author	Patrick Felixberger
     * \param	ID 	Number of leg
     * \param	Pos Position of leg
     * \return	Angles of leg
     */
    TAngles CalculateAnglesServoIK(int ID, T3DPosition const &Pos) const;

    /** \brief	Calculates the angles of the legs for body
         * \author	Lukas Plazovnik
         * \param
         * \param
         * \return
         */
    //void CalculateAnglesBodyIK(int ID, TBodyPos const&CurrentPos,T3DPosition const &Pos);
    void CalculateBodyIK(int ID, TBodyPos &CurrentPos,T3DPosition const &Pos)const;
private:
    /*
     * \brief 	Get Angle of Femur
     * \author	Patrick Felixberger
     * \param	x
     * \param	ny
     * ToDo: Explain Paramter
     */
    double AngleFemur(  double const &x, double const &ny) const;

    /**
     * \brief
     * \author	Patrick Felixberger
     * \param	a
     * \param	b
     * \param	c
     */
    double AngleTarsus( double const &a, double const &b,
                        double const &c) const;

    /**
     * \brief
     * \author	Patrick Felixberger
     * \param	a
     * \param	b
     * \param	c
     * \param	z
     * \param	dist
     */
    double AngleTibia(  double const &a, double const &b,
                        double const &c, double const &z,
                        double const &dist) const;

    /// Geometric Data of Hexapod
    HexapodSize mSizeData;

};


#endif // INVERSEKINEMATIC_H
