///////////////////////////////////////////////////////////////////////////
// Workfile    : Parser.h
// Author      : Patrick Felixberger
// Date        :
// Description : Reads infos from config files
// Remarks     : -
// Revision    : 0
///////////////////////////////////////////////////////////////////////////
#ifndef PARSER_H
#define PARSER_H

#include "Definitions.h"
#include "Object.h"

typedef int TServoValues[NUM_OF_LEGS * SERVOS_PER_LEG][NUM_OF_SERVO_VALS];

int const MAX_LEN = 512;    /**< Maximal line length */

/**
 *	\brief	Reads all the necessary info from the param files
 *	\author Patrick Felixberger
 *	\date	2013/12/04
 */
class Parser : public Object {
    public:
        /**
         * \brief	Default CTor
         * \author	Patrick Felixberger
         * \param	path Path of file to open
         */
        Parser(std::string const &path);

        /**
         * \brief	Default DTor
         * \author	Patrick Felixberger
         */
        ~Parser();

        /**
         * \brief
         * 		Parses file to get dimensions of hexapod
		 * 		Default "Hexapod.txt"
		 * \author Patrick Felixberger
         * \return	True if file was read successfully
         */
        bool ParseFile();

        /**
         * \brief
         * 		Parses file to get default position of legs (start position)
		 * 		Default "DefaultLegPos.txt"
		 * \author	Patrick Felixberger
         * \return True if file was read successfully
         */
        bool ParseFileDefaultPos();

        /**
         * \brief
         * 		Parses file to get default servo values
		 * 		Default "ServoValues"
		 * 	\author	Patrick Felixberger
         *	\return True if file was read successfully
         */
        bool ParseFileServos();

        /**
         * \brief	Copies read values into passed variable
         * \author	Patrick Felixberger
         * \param	hs HexapodSize struct
         * \return	Positive value if data was copied
         */
        bool GetData(HexapodSize &hs) const;

        /**
         * \brief	Copies read values into passed variable
         * \author	Patrick Felixberger
         * \param	hs DefaultLegPos struct
         * \return	Positive value if data was copied
         */
        bool GetData(DefaultLegPos &hs) const;

        /**
         * \brief	Copies read values into passed variable
         * \author	Patrick Felixberger
         * \param	hs Array
         * \return	Positive value if data was copied
         */
        bool GetData(TServoValues &hs) const;

    private:
        /// Values of servo i2c addres and Angles
        TServoValues mServoValues;

        /// Geometric Data of Hexapod
        HexapodSize mSizeData;

        /// Default Position of Legs
        DefaultLegPos mLegPos;

        /// Path to File
        std::string mPath;

};


#endif // PARSER_H
