/*
 * servo.cpp
 *
 *  Created on: 02.07.2013
 *      Author: josef
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include <cmath>

/*#ifdef __unix__
#include <curses.h>
#endif*/

#include <string>

#include "Servo.h"

using namespace std;



/**
 * \brief	CTor
 * \author	Lukas Plazovnik
 * \param	uCAddress		Address of uC
 * \param	ServoID 	ID of Serv(0=Femur, 1=Tibia, 2=Tarsus)
 * \param	Angle_Min	Minmal Angle
 * \param	Angle_Max	Maximal Angle
 */
Servo::Servo(unsigned char ServoID,double Angle_Min, double Angle_Max):
		mAngle_Min(Angle_Min),mAngle_Max(Angle_Max) ,mAngle_factor(double()),  mAngle(double()),mServoID(ServoID){}

/**
 * \brief	DTor
 * \author	Josef Sommerauer
 */
Servo::~Servo() {
	Off();
}

/**
 * \brief	deactivates servo, send to min pos
 * \author	Lukas Plazovnik
 * \return false in case of an driver error.
 */
bool Servo::Off() {

	SetAngle(mAngle_Min);
	return true;
}

/**
 * \brief	sets the angle of the servo.
 * \author	Lukas Plazovnik
 * \param	value	angle in degree
 * \return	false if angle is out of range or in case of an driver error.
 */
bool Servo::SetAngle(double const& value) {


	mAngle_factor=1.41667;


	if(value>mAngle_Max ||value<mAngle_Min){
		std::cout << "maxValue: " << mAngle_Max << std::endl;
		std::cout << "Value out of Range "<<(int) value<<std::endl;
		return false;
	}

	mAngle = value;
//	std::cout << "Servo: "<<std::hex<< (int)mServoID <<" was set to Angel: "<<std::dec<< value <<std::endl;


	return true;
}

/**
 * \brief	send angle via i2c
 * \author	Lukas Plazovnik
 * \param
 * \return	false if couldn't be send
 // */
// bool Servo::SendAngle(){

	//
// std::cout << "error: old send angle in servo.cpp excecuted--> should be in leg.cpp" << std::endl;
	   // unsigned char buffer[3];//char == byte

	   // buffer[0]=mServoID;
	   // buffer[1]=mAngle;
	
	   // //i2c.send(0x12, buffer, 2);   //send 2 bytes from buffer to register-address 0x12
	   // if(i2c.send(buffer,3)==-1){
		   // std::cout << "error in sending to 0x"<< std::hex <<i2c.getAddress() <<std::endl;
		   // return false;
	   // }
	   // else{
		   // std::cout << "angle "<< (int)mAngle  << " was send to 0x"<< std::hex<<i2c.getAddress() << std::endl;
		   // return true;
	   // }

	  // // i2c.receive(buffer, 3);       // read 3 bytes and store them in RxBuf
	   // //i2c.receive(0x23, RxBuf, 3);  // read from  register-address 0x23 3 bytes and store them in RxBuf


// }
/**
 * \brief	receive data  from slave
 * \author	Lukas Plazovnik
 * \param
 * \return	false if could be received
 */
// bool Servo::RequestAngle(){
//	possible to read a register from uC?
	//i2c.receive(RegisterAddress, RxBuf, length);
	// unsigned char rxbuffer[2];



	// if(i2c.receive(rxbuffer, 2)){
		// std::cout << "error in receiving from 0x"<< std::hex <<i2c.getAddress() <<std::endl;
		// return false;
	// }
	// else{
		// std::cout << "angle "<< (int)mAngle  << " was received from 0x"<< std::hex<<i2c.getAddress() << std::endl;
		// return true;
	// }

	// return true;

// }

/**
 * \brief	returns the current angle of the servo.
 * \author	Josef Sommerauer
 *	\return current angle of the servo
 */
int Servo::GetAngle() const{
	return mAngle;
}

/**
 * \brief	return the maximal angle of the servo.
 * \author	Josef Sommerauer
 * \return maximal angle of the servo
 */
int Servo::GetMaxAngle() const{
    return mAngle_Max;
}

/**
 * \brief	return the minimal angle of the servo.
 * \author	Josef Sommerauer
 * \return minimal angle of the servo
 */
int Servo::GetMinAngle() const{
    return mAngle_Min;
}

double Servo::GetAngle_Factor() const{

return mAngle_factor;
}

/**
 * \brief		calibrate servo. the user has the enter + and - to find the maximal und minimal angle of the servo.
 * \author		Josef Sommerauer
 * \attention	{only works on pandaboard with connected servos}
 */
 void Servo::Calibrate() {


		mAngle_factor=255/(mAngle_Max - mAngle_Min);



	//

	// // ncurses starten und initialisieren
	// initscr();
	// start_color();			/* Start color 			*/
	// init_pair(1, COLOR_RED, COLOR_BLACK);
	// attron(COLOR_PAIR(1));

	// // set minimal Angle
	// if(mAngle_Min == 0) {
		// std::cout << "Min. Angle: ";
		// std::cin >> mAngle_Min;
	// }

	// // set maximal Angle
	// if(mAngle_Max == 0) {
		// std::cout << "Max. Angle: ";
		// std::cin >> mAngle_Max;
	// }


	// mPWM_Min = 0;
	// char Input = 0;

	// char header[256];
	// sprintf(header, "Servo Number %d\n", mPort);

	// printw(header);

	// printw("Try to find PWM Value for Min. Angle\n");
	// printw("  - Press [+] or [-] to find correct Angle\n");
	// printw("  - Press [Enter] if you found the right Angle\n");

	// refresh();

	// noecho();

	// while(Input!='\n') {
		// SetPWM(mPWM_Min);

		// Input=getch();

		// if(Input=='+') mPWM_Min++;
		// if(Input=='-') mPWM_Min--;
	// }

	// Input = 0;

	// printw("Try to find PWM Value for Max. Angle\n");
	// printw("  - Press [+] or [-] to find correct Angle\n");
	// printw("  - Press [Enter] if you found the right Angle\n");

	// refresh();

	// noecho();
	// while(Input!='\n') {
		// SetPWM(mPWM_Max);

		// Input=getch();

		// if(Input=='+') mPWM_Max++;
		// if(Input=='-') mPWM_Max--;
	// }

	// // close ncurses
	// attroff(COLOR_PAIR(1));
	// clear();
	// endwin();
}

/**
 * \brief	prints all settings of an servo.
 * \author	Lukas Plazovnik
 * \return
*/
void Servo::Print(std::ostream &o) const {
	o << "uCAddress: " /*<< i2c.getAddress(void)*/<< " ID of Servo: "<< mServoID << " Current Angel: "<< mAngle << endl;
}
