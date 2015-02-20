

/*!
 *  @file   XBOXController.h
 *  @brief  Header for XBOXController class
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

#ifndef XBOXCONTROLLER_H_
#define XBOXCONTROLLER_H_

#define CONTROLLER_DEV  "/dev/input/js0"
#define EVENT_DEV       "/dev/input/event0"

#include<string>
#include<vector>

#include"joystick.h"

// Definitions for controller buttons
#define BUTTON_A      0
#define BUTTON_B      1
#define BUTTON_X      2
#define BUTTON_Y      3
#define BUTTON_LB     4
#define BUTTON_RB     5
#define BUTTON_BACK   6
#define BUTTON_START  7
#define BUTTON_XBOX   8
#define BUTTON_LSTICK 9
#define BUTTON_RSTICK 10
#define MAX_BUTTONS   11
#define MOVING 		  12

#define DEAD_ZONE_LEFT_JOYSTICK 20000

// Definition for controller sick positions
#define POS_LSTICK    0
#define POS_RSTICK    2

// Definitions for controller axis
#define AXIS_LSTICK_X 0
#define AXIS_LSTICK_Y 1
#define AXIS_RSTICK_X 2
#define AXIS_RSTICK_Y 3
#define AXIS_RT       4
#define AXIS_LT       5
#define AXIS_PAD_X    6
#define AXIS_PAD_Y    7
#define MAX_AXIS      8

// Definitions for rumble effects
#define RMBL_SIN_VIBE 0
#define RMBL_CONST    1
#define RMBL_SPRING   2
#define RMBL_DAMP     3
#define RMBL_STRONG   4
#define RMBL_WEAK     5
#define MAX_RUMBLES   6

struct js_state {
    std::vector<signed short> button;
    std::vector<signed short> axis;
};

struct js_position {
    double theta;
    double R;
    double X;
    double Y;
};


class XBOXController {
  public:
    XBOXController();
    ~XBOXController();

    int DoRumble(int Effect);
    int StopRumble(void);
    js_position GetPosition(int Nr);
    int GetAxisValue(int Nr);
    static void * Thread (void* object);
    void readEvent();
    bool IsButtonPressed(int Nr);
  private:
    double CalcTheta(double X, double Y);
    pthread_t thread;
    bool IsActive;
    int Controller_fd;
    int OpenRumble(std::string fd = "");
    void CloseRumble(void);
    js_event *Controller_event;
    js_state *Controller_state;
    __u32 version;
    __u8 axes;
    __u8 buttons;
    std::string name;
};

#endif
