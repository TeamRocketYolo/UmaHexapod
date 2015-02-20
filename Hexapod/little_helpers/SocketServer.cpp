/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "SocketServer.h"

using namespace std;

/**
 * \brief		CTor
 * \author	Martin Haiden
 * \param 	PortNum	Number of Communication Port
 */
CtrlSocket::CtrlSocket(int PortNum) : mMsgCtr(0), mRxCmd(nocmd), mTerminat(false), mNewsockfd(0) {


     mpReceiveMessage = new Message_t;
     mpSendMessage    = new Message_t;
     
     // create Socket
     mSockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (mSockfd < 0) {
        throw "CtrlSocket::CtrlSocket():ERROR opening socket";
     }

     memset((char *) &mServer_addr, 0, sizeof(mServer_addr));
     mServer_addr.sin_family      = AF_INET;
     mServer_addr.sin_addr.s_addr = INADDR_ANY;
     mServer_addr.sin_port        = htons(PortNum);

     // set socket option to avoid time_wait state
     int yes = 1;
     if(setsockopt( mSockfd, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) < 0 ) {
         throw "CtrlSocket::CtrlSocket():ERROR on setting socket options";
     }

     // bind Server to Address
     if (bind( mSockfd,
    		    (struct sockaddr *) &mServer_addr,
    		    sizeof(mServer_addr)) < 0) {
        // throw "CtrlSocket::CtrlSocket():ERROR on binding";
	  }

     // listening for Connections (max. 10)
     listen(mSockfd, MAX_LISTENING_CONNECTIONS);
     mClient_length = sizeof(mClient_addr);
}

/**
 * \brief 	DTor
 * \author	Martin Haiden
 */
CtrlSocket::~CtrlSocket() {

	 // Close Sockets
	 close(mNewsockfd);
     close(mSockfd);

     // Free Pointer
     delete mpReceiveMessage;
     delete mpSendMessage;
}

/**
 * \brief	get last received command.
 * \author Martin Haiden
 * \return received command, nocmd if no command was received or error if an socket error occurred.
 */
CtrlSocket::TCmd CtrlSocket::GetCmd() {

   // Lock Access
   mRxMsg_Mutex.lock();

   // Read Command
   TCmd tmp = mRxCmd;

   // Clear Command
   mRxCmd = nocmd;

   // Unlock Access
   mRxMsg_Mutex.unlock();
   return tmp;
}

/**
 * \brief	stops socketthread
 * \author	Martin Haiden
 * \return false if an error occurred.
 */
bool CtrlSocket::Stop() {

   // Terminate Process
   mTerminat = true;

   // Wait for Workerthread finished
   mWorker.join();

   return true;
}

/**
  * \brief	starts socketthread and listen for new commands.
  * \author	Martin Haiden
  * \return false if an error occurred.
  */
bool CtrlSocket::Run() {

	  // Start new Thread
      mWorker = std::thread([this](){
      mTerminat = false;

      // Repeat until Terminated
      while(!mTerminat) {
    	    string 	MessageString;

            // timeout for select
            timeval mTimeout;
            mTimeout.tv_sec  = 0;
            mTimeout.tv_usec = 0;

            // read filediscr. set
            fd_set rd;
            FD_ZERO(&rd);
            FD_SET(mSockfd,&rd);

            // check if able to accept -- none 
            if(select(mSockfd+1, &rd, NULL, NULL, &mTimeout) < 1) {
               continue;
            }

            // accept a connection
            mNewsockfd = accept(mSockfd,
                        (struct sockaddr *) &mClient_addr,
                         &mClient_length);
            if (mNewsockfd < 0) {
               throw "CtrlSocket::CtrlSocket():ERROR on accept";
               return;
            }

            memset(mpReceiveMessage, 0, MESSAGE_SIZE);
            memset(mpSendMessage,    0, MESSAGE_SIZE);
            
		    // reads data from socket
		    int n = read(mNewsockfd, mpReceiveMessage, MESSAGE_SIZE);
             
		    if (n < 0) {
		    	throw "CtrlSocket::CtrlSocket():ERROR reading Message from socket";
                return;
		    } else if (n != 0) {

            cout << string(60,'=') << endl;
            cout << "msg nr.: " << dec << ++mMsgCtr                 << endl;
            cout << "received bytes: " << n                         << endl;
            cout << "cmd: " << hex << (int)mpReceiveMessage->Command << endl;
            cout << "typ: " << hex << (int)mpReceiveMessage->Type    << endl;
            cout << "msg: " << mpReceiveMessage->Msg << endl;

            // lock access
            mRxMsg_Mutex.lock();

            // check commandtype
		    switch (mpReceiveMessage->Command)
		    {
		    	case CMD_MOVE:

                   // +++++++++++++++++ add new commands hier ++++++++++++++++++++
		          	 switch (mpReceiveMessage->Type)
		          	 {
		           	 case TYPE_CMD_WALK:

		           		 // Check walk directrion
						 if (strcmp(mpReceiveMessage->Msg,"move-forward")       == 0) {
							MessageString = "move forward: ack";
							mRxCmd = moveforward;

						 } else if(strcmp(mpReceiveMessage->Msg,"move-reverse") == 0) {
							MessageString = "move reverse: ack";
							mRxCmd = movereverse;

						 } else if(strcmp(mpReceiveMessage->Msg,"move-left")    == 0) {
							MessageString = "move left: ack";
							mRxCmd = moveleft;

						 } else if(strcmp(mpReceiveMessage->Msg,"move-right")   == 0) {
							MessageString = "move right: ack";
							mRxCmd = moveright;

						 } else if(strcmp(mpReceiveMessage->Msg,"turn-left")    == 0) {
							MessageString = "turn left: ack";
							mRxCmd = turnleft;

						 } else if(strcmp(mpReceiveMessage->Msg,"turn-right")   == 0) {
							MessageString = "turn right: ack";
							mRxCmd = turnright;

						 } else if(strcmp(mpReceiveMessage->Msg,"wakeup")       == 0) {
							 MessageString = "wakeup : ack";
							mRxCmd = wakeup ;

						 } else if(strcmp(mpReceiveMessage->Msg,"sleep")        == 0) {
							 MessageString = "sleep  : ack";
							mRxCmd = sleep;

						 } else if(strcmp(mpReceiveMessage->Msg,"shutdown")     == 0) {
							 MessageString = "shutdown: ack";
							mRxCmd = shutdown;

						 } else if(strcmp(mpReceiveMessage->Msg,"tripod-gait")  == 0) {
							 MessageString = "tripod-gait : ack";
							mRxCmd = tripod;

						 } else if(strcmp(mpReceiveMessage->Msg,"wave-gait")    == 0) {
							 MessageString = "wave-gait: ack";
							mRxCmd = wave;

						 } else if(strcmp(mpReceiveMessage->Msg,"ripple-gait")  == 0) {
							 MessageString = "ripple-gait : ack";
							mRxCmd = ripple;

						 } else {
							MessageString = "walk: nack";
							mRxCmd = error;
						 }
							break;

		           	 default:
		           		mpSendMessage->Command = CMD_NACK;
		           		MessageString = "false CMD_TYPE";
		           		break;
		          	 }
		          	 break;

		          default:
		          	 mpSendMessage->Command = CMD_NACK;
		          	 MessageString = "false CMD sent!";
			          break;
		       }

		    // Unlock Access
             mRxMsg_Mutex.unlock();

		       cout << "Answer to client: " << MessageString << endl << endl;
             MessageString += "\r\n\0";
		       n = write(mNewsockfd, MessageString.c_str(), MessageString.size());
		       if (n < 0) {
               throw "CtrlSocket::CtrlSocket():ERROR writing answer to client";
             }
		       // clean FIFO
               
               MessageString.clear();
             }

		    // Close Socket
         close(mNewsockfd);
         cout << string(60,'=') << endl;
         cout << "End of transmission!" << endl;
         cout << string(60,'=') << endl;
         
      }
   });

   return true;
}
