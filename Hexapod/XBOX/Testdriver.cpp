/*!
 *  @file   Testdriver.cpp
 *  @brief  Testdriver for XBOXController
 *  
 *  <+DETAILED+>
 *  
 *  @author  Bernd Schmidt (), bernd.schmidt@inode.at
 *  
 *  @internal
 *       Created:  04/12/14
 *      Revision:  none
 *      Compiler:  g++
 *  Organization:  
 *     Copyright:  
 *  
 *  This source code is released for free distribution under the terms of the
 *  GNU General Public License as published by the Free Software Foundation.
 */
/*
#include<iostream>
#include<unistd.h>
#include<cmath>
#include<stdlib.h>

#include "XBOXController.h"

using namespace std;

int main(int argc, char **argv){
  // create an object of class XBOXController
XBOXController Controller;

  while(1){
    if (Controller.IsButtonPressed(BUTTON_A)) {
      js_position ContPos;
      ContPos = Controller.GetPosition(POS_LSTICK);
#ifdef DEBUG
      cout << "X: " << ContPos.X << endl;
      cout << "Y: " << ContPos.Y << endl;
      cout << "R: " << ContPos.R << endl;
#endif
      cout << "A: " << ContPos.theta << endl;
    }
    if (Controller.IsButtonPressed(BUTTON_Y)) {
      cout << "RS X: " << Controller.GetAxisValue(AXIS_RSTICK_X) << endl;
      cout << "RS Y: " << Controller.GetAxisValue(AXIS_RSTICK_Y) << endl;
    }
  }


  return 0;
}

*/
