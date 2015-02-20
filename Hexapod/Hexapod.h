/////////////////////////////////////////////////////////////////////////////
// Workfile    : Hexapod.h
// Author      : Daniel Wolfmayr
// Date        : 2013/11/20
// Description :
// Project	   : RoboMotion
// Revision    :
/////////////////////////////////////////////////////////////////////////////

#ifndef HEXAPOD_H_INCLUDED
#define HEXAPOD_H_INCLUDED

#define SOCK_DELAY 1000000

#include <signal.h>

#include "little_helpers/Object.h"
#include "little_helpers/SocketServer.h"
#include "gaits/Movement.h"
#include "gaits/TripodGait.h"
#include "gaits/WaveGait.h"
#include "gaits/RippleGait.h"
#include "gaits/TripodGait_Response.h"
#include "gaits/RippleGait_Response.h"
#include <unistd.h>
#include <vector>
#include <string>
#include "XBOX/XBOXController.h"

#include "little_helpers/joining_thread.h"
#include <mutex>

typedef std::vector<Movement*> Movements;

/*!
 * \brief This is the Mainclass of Hexapod and includes CtrlSocket-Class and Movement-Class
 * \date 2013/11/20
 * \author Daniel Wolfmayr
 */
class Hexapod : public Object{
public:

	//! CTor
	/*!
	\param PortNbr Portnumber of Server
	\param lc Textfile for servo-definition
	\author Daniel Wolfmayr
	*/
	Hexapod(int PortNbr, LegCtrl * lc);


	void Run();

	//! DTor
	/*!
	 * \author Daniel Wolfmayr
	 */
	~Hexapod();

	//! Enumeration of Movement-Styles
	/*
	 * \author Daniel Wolfmayr
	 */
	enum TGait {TRIPOD, WAVE, RIPPLE,TRIPOD_RESPONSE, RIPPLE_RESPONSE};

	//! set Gait/Movement style
	/*! Lukas Plazovnik
	// TRIPOD, WAVE, RIPPLE
	\param enum Gait Movement/predefined
	*/
	void SetGait(TGait Gait);
	int CurrentPos();
	

	//! starts the server and the Hexapod and interprets the commands from server
	/*!
	 * \author	Daniel Wolfmayr
	 * \return 	a "1" in case of stop-command
	*/
	void RunHexapod_Thread();

	void ControllerThread();
private:
	//void* Controller_Thread(void *object);
	//!Vector with all gaits
	Movements mMov;
	//!Pointer to an specified gait
	Movement * mpGait;
	CtrlSocket mSocket;
	//bool isSocketRunning;
	CtrlSocket::TCmd mCmd;
	XBOXController * MyController;
	std::condition_variable mCondVar_newCommand;
	std::mutex mCommandMutex;
	js_position mContPos;
	bool newCommand;
	int CommandID;
};

#endif
