/*
 * SocketServer.h
 *
 *  Created on: May 7, 2013
 *      Author: panda
 */

#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include "Object.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>

// constants for socket
#define MESSAGE_SIZE				    128
#define CMD_BYTES					      2
#define MAX_LISTENING_CONNECTIONS	1

// defines for Command-Types via Socket
#define CMD_MOVE					   0x01
#define CMD_TXT					   0x02
#define CMD_ACK					   0x06
#define CMD_NACK				      0x15
#define CMD_BAT					   0x64
#define CMD_END					   0xFF

// defines for Command-Types
#define TYPE_CMD_WALK			   0x01
#define TYPE_CMD_ROT			      0x02
#define TYPE_CMD_HEIGHT			   0x03

// defines for Text-Types
#define TYPE_TXT_NORMAL			   0x01
#define TYPE_TXT_WARNING		   0x02
#define TYPE_TXT_ERROR			   0x03


/**
 * \brief	Structure Defining a Command from Userinterface
 * \author	Martin Haiden
 * \date	2013/05/07
 */
typedef struct Message {
	unsigned char Command;
	unsigned char Type;
   char Msg[MESSAGE_SIZE - CMD_BYTES];
} Message_t;

/**
 * \brief	Managing Communication between Hexapod and Userinterface
 * \author	Martin Haiden
 * \date	2013/05/07
 */
class CtrlSocket  : public Object {
   public:		

	  /**
	   * \brief		CTor
	   * \author	Martin Haiden
	   * \param 	PortNum	Number of Communication Port
	   */
      CtrlSocket(int PortNum);

      /**
       * \brief 	DTor
       * \author	Martin Haiden
       */
      ~CtrlSocket();

	 /**
	  * \brief	starts socketthread and listen for new commands.
	  * \author	Martin Haiden
	  * \return false if an error occurred.
	  */
      bool Run();

	  /**
	   * \brief	stops socketthread
	   * \author	Martin Haiden
	   * \return false if an error occurred.
	   */
      bool Stop();

      /**
       * \brief		types of commands received by socket.
       * \author	Martin Haiden
       */
	  enum TCmd {nocmd, 
                moveleft, moveright, 
                moveforward, movereverse,
                turnleft, turnright,
                tripod, wave, ripple,
                sleep, wakeup, shutdown, 
                error = -1};

	  /**
 	   * \brief	get last received command.
 	   * \author Martin Haiden
 	   * \return received command, nocmd if no command was received or error if an socket error occurred.
 	   */
     TCmd GetCmd();
	
   private:
      /// Messagecounter
      int mMsgCtr;

      /// Command to Execute
      TCmd mRxCmd;

      /// Mutex to Lock Variable
      std::mutex mRxMsg_Mutex;

      /// Stop Execution of Server
      bool mTerminat;

      /// Workerthread
      std::thread mWorker;

      //int const PortNum;

      /// file descriptor to listening Port
      int mSockfd;

      /// File descriptor to Communicationport
      int mNewsockfd;

      /// messagebuffer Receiver
      Message_t * mpReceiveMessage;

      /// Messagebuffer Sender
      Message_t * mpSendMessage;

      /// Listeningsocket Data
      struct sockaddr_in mServer_addr;

      /// Communicationsocket Data
      struct sockaddr_in mClient_addr;

      /// Size of socketdata
      socklen_t mClient_length;
};

#endif /* SOCKETSERVER_H_ */
