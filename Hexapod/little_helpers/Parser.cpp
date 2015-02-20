#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "Parser.h"

using namespace std;


/**
 * \brief	Default CTor
 * \author	Patrick Felixberger
 * \param	path Path of file to open
 */
Parser::Parser(std::string const &path): mPath(path) {}

/**
 * \brief	Default DTor
 * \author	Patrick Felixberger
 */
Parser::~Parser() {}

/**
 * \brief
 * 		Parses file to get dimensions of hexapod
 * 		Default "Hexapod.txt"
 * \author Patrick Felixberger
 * \return	True if file was read successfully
 */
bool Parser::ParseFile() {

	// open File
	ifstream file(mPath.c_str());
    char line[MAX_LEN];

    if(file.is_open()) {
        // read values
        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get distance between femur and tibia
            if(sscanf(line, "NB = %lf;", &mSizeData.NB) > 0)
                break;
        }

        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get x offset for leg 2
            if(sscanf(line, "ON = %lf;", &mSizeData.ON) > 0)
                break;
        }

        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get x offset for leg 1, 3, 4, 6
            if(sscanf(line, "OM = %lf;", &mSizeData.OM) > 0)
                break;
        }

        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get y offset for leg 1, 3, 4, 6
            if(sscanf(line, "OQ = %lf;", &mSizeData.OQ) > 0)
                break;
        }

        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get femur length
            if(sscanf(line, "a = %lf;", &mSizeData.a) > 0)
                break;
        }

        file.seekg(0);  // start of file
        while(file.getline(line, MAX_LEN)) {
        	// get crus length
            if(sscanf(line, "b = %lf;", &mSizeData.b) > 0)
                break;
        }
    } else {
        // file not open
        cout << "Parser::ParseFile(): Can't open: " << mPath << endl;

        return false;
    }
	
    // close file
	file.close();

    return true;
}

/**
 * \brief
 * 		Parses file to get default position of legs (start position)
 * 		Default "DefaultLegPos.txt"
 * \author	Patrick Felixberger
 * \return True if file was read successfully
 */
bool Parser::ParseFileDefaultPos() {

	// open file
    ifstream file(mPath.c_str());
    char line[MAX_LEN];
    int timeout = 0;

    if(file.is_open()) {
        // read values
        file.seekg(0);  // start of file

        for(int i = 0; i < NUM_OF_LEGS;) {
            file.getline(line, MAX_LEN);
            if(sscanf(line, ";%lf, %lf, %lf;",
            		&mLegPos[i].X, &mLegPos[i].Y, &mLegPos[i].Z) > 0)
            	{
            		i++;
            	}

            if(timeout > 100)
                return false;
        }

    } else {
        // file not open
        cout << "Parser::ParseFileDefaultPos(): Can't open: " << mPath << endl;

        return false;
    }
	
    // close file
	file.close();

    return true;
}

/**
 * \brief
 * 		Parses file to get default servo values
 * 		Default "ServoValues"
 * 	\author	Patrick Felixberger/Luka Plazovnik
 *	\return True if file was read successfully
 */
bool Parser::ParseFileServos() {
    // open file
	ifstream file(mPath.c_str());
    char line[MAX_LEN];
    int timeout = 0;

    if(file.is_open()) {
        file.seekg(0);  // start of file

        for(int i = 0; i < NUM_OF_LEGS*3; timeout++) {
            file.getline(line, MAX_LEN);
            if(sscanf(line, ";%d, %d, %d;",
            		//address, min_angle, max_angle
					&mServoValues[i][0], &mServoValues[i][1], &mServoValues[i][2]) > 0)

            	{
//            	std::cout <<"address: "<< mServoValues[i][0]<< " min_angel: "<< mServoValues[i][1]<<" max_angel: "<< mServoValues[i][2]<<std::endl;
            		i++;
            	}

            if(timeout > 100)
                return false;
        }

    } else {
        // file not open
        cout << "Parser::ParseFileServos(): Can't open: " << mPath << endl;

        return false;
    }
	
    // close file
	file.close();

    return true;
}

/**
 * \brief	Copies read values into passed variable
 * \author	Patrick Felixberger
 * \param	hs HexapodSize struct
 * \return	Positive value if data was copied
 */
bool Parser::GetData(HexapodSize &hs) const {
	hs = mSizeData;
	return true;
}

/**
 * \brief	Copies read values into passed variable
 * \author	Patrick Felixberger
 * \param	hs DefaultLegPos struct
 * \return	Positive value if data was copied
 */
bool Parser::GetData(DefaultLegPos &lp) const {
    for(int i = 0; i < NUM_OF_LEGS; i++)
    {
    	lp[i] = mLegPos[i];
    }
    return true;
}

/**
 * \brief	Copies read values into passed variable
 * \author	Patrick Felixberger
 * \param	hs Array
 * \return	Positive value if data was copied
 */
bool Parser::GetData(TServoValues &sv) const {
    for(int i = 0; i < (NUM_OF_LEGS * SERVOS_PER_LEG); i++)
    {
    	for(int j = 0; j < NUM_OF_SERVO_VALS; j++)
    	{
    		sv[i][j] = mServoValues[i][j];
    	}
    }

	return true;
}
