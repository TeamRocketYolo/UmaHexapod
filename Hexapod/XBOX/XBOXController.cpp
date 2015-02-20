/*!
 *  @file   XBOXController.cpp
 *  @brief  Implementation of XBOXController
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

// Includes for Controller
#include <unistd.h>

#include <iostream>
#include <cmath>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>

#include <linux/input.h>

// Includes for rumble feature
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <sys/ioctl.h>
#include <errno.h>

#define BITS_PER_LONG (sizeof(long) * 8)
#define OFF(x) ((x)%BITS_PER_LONG)
#define BIT(x) (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

// Custom header
#include "XBOXController.h"


using namespace std;

XBOXController::XBOXController() {
	IsActive = false;
	Controller_state = new js_state;
	Controller_event = new js_event;
	// reset device pointer
	Controller_fd = 0;

	bool opened = 0;
	while (!opened) {
		Controller_fd = open(CONTROLLER_DEV, O_RDWR);
		if (Controller_fd != -1) {
			ioctl(Controller_fd, JSIOCGNAME(256), name.c_str());
			ioctl(Controller_fd, JSIOCGVERSION, &version);
			ioctl(Controller_fd, JSIOCGAXES, &axes);
			ioctl(Controller_fd, JSIOCGBUTTONS, &buttons);
			std::cout << "   Name: " << name << std::endl;
			std::cout << "Version: " << version << std::endl;
			std::cout << "   Axes: " << (int) axes << std::endl;
			std::cout << "Buttons: " << (int) buttons << std::endl;
			std::cout << "Set Axis.reserve" << std::endl;
			Controller_state->axis.reserve((int) axes);
			std::cout << "Set Button.reserve" << std::endl;
			Controller_state->button.reserve((int) buttons);
			IsActive = true;
			std::cout << "create thread" << std::endl;
			pthread_create(&thread, 0, &XBOXController::Thread, this);
			opened = 1;
		} else {
			std::cout
					<< "error in opening /dev/input/js0....is the controller plugged in?"
					<< std::endl;
			std::cout << "trying again in 5 seconds" << std::endl;
			sleep(5);
		}
	}
	//OpenRumble();

}

XBOXController::~XBOXController() {
  std::cout << "Deleting object..." << std::endl;
  if (Controller_fd > 0) {
    IsActive = false;
    pthread_join(thread, 0);
    close(Controller_fd);
  }
  delete Controller_state;
  delete Controller_event;
  Controller_fd = 0;
  CloseRumble();
}


bool XBOXController::IsButtonPressed(int Nr) {
  if (IsActive) {
    return (Controller_state->button[Nr] == 1);
  } else {
    return false;
  }
}

int XBOXController::GetAxisValue(int AxisNr) {
  if (!IsActive || AxisNr < 0 || AxisNr >= MAX_AXIS) {
    return -1;
  }
  int retVal = Controller_state->axis[AxisNr];
  if (AxisNr == AXIS_RT || AxisNr == AXIS_LT) {
    int tmp = retVal;
    retVal = (255.0/(2.0*32767.0)) * (tmp + 32767.0);
  }
  return retVal;
}

double XBOXController::CalcTheta(double X, double Y) {
  if (X != 0.0 && Y != 0.0)
  {
    double angle = atan2(Y, X) * 180.0 / M_PI;
    if (Y < 0)
    {
      angle = 180.0 + angle;
    }
    else if (Y > 0)
    {
      angle += 180.0;
    }

    return angle;
  }
  else
  {
    /* Middle Position */
    return 0.0;
  }
}

js_position XBOXController::GetPosition(int Nr) {
  js_position ReturnPos;
  if (IsActive) {
    if (Nr != 0 && Nr != 2) {
      //throw ("No Position for Stick on Nr " + Nr);
      return ReturnPos;
    }
    double tmp_X = Controller_state->axis[Nr];
    double tmp_Y = Controller_state->axis[Nr+1];

    ReturnPos.X = tmp_X;
    ReturnPos.Y = tmp_Y;
    ReturnPos.R = sqrt(pow(tmp_X,2)+pow(tmp_Y,2));
    ReturnPos.theta = -1.0;
    // caluclate theta
    ReturnPos.theta = CalcTheta(tmp_X, tmp_Y);
  }
  return ReturnPos;
}

void* XBOXController::Thread(void *object) {
    while (reinterpret_cast<XBOXController *>(object)->IsActive) {
      reinterpret_cast<XBOXController *>(object)->readEvent();
    }
    return 0;
}

void XBOXController::readEvent() {
  int bytes = read(Controller_fd, Controller_event, sizeof(*Controller_event));
  if (bytes > 0) {
    Controller_event->type &= ~JS_EVENT_INIT;
    if (Controller_event->type & JS_EVENT_BUTTON) {
      if (Controller_event->number < MAX_BUTTONS) {
        Controller_state->button[Controller_event->number] = Controller_event->value;
      }
    }
    if (Controller_event->type & JS_EVENT_AXIS) {
      if (Controller_event->number < MAX_AXIS) {
        Controller_state->axis[Controller_event->number] = Controller_event->value;
      }
    }
  }
}

// constant values for rumble feature

static int event_fd;
static int NoEffects;
static unsigned long features[4];
static struct ff_effect effects[MAX_RUMBLES];

int XBOXController::StopRumble(void) {
  struct input_event stop;

  for (size_t i = 0; i < MAX_RUMBLES; ++i) {
    stop.type = EV_FF;
    stop.code = effects[i].id;
    stop.value = 0;

    if (write(event_fd, (const void*) &stop, sizeof(stop)) == -1) {
      exit(1);
    }
  }
  return 0;
}

int XBOXController::OpenRumble(string fd) {
  if (fd == "") {
    fd = EVENT_DEV;
  }

  event_fd = open(fd.c_str(), O_RDWR);
  if (event_fd < 0) {
    cerr << "Could not open event " << fd << endl;
    cerr << "Rumble might not work"<< endl;
    return -1;
  }

  cout << "Device " << fd << " opened." << endl;

  if (ioctl(event_fd, EVIOCGBIT(EV_FF, sizeof(unsigned long) * 4), features) == -1) {
    return -1;
  }
  cout << "Rumble device detected!" << endl;

	if (ioctl(event_fd, EVIOCGEFFECTS, &NoEffects) == -1) {
		NoEffects = 1;	 /* assume 1. */
	}


	/* download a periodic sinusoidal effect */
	effects[0].type = FF_PERIODIC;
	effects[0].id = -1;
	effects[0].u.periodic.waveform = FF_SINE;
	effects[0].u.periodic.period = 0.1*0x100;	/* 0.1 second */
	effects[0].u.periodic.magnitude = 0x4000;	/* 0.5 * Maximum magnitude */
	effects[0].u.periodic.offset = 0;
	effects[0].u.periodic.phase = 0;
	effects[0].direction = 0x4000;	/* Along X axis */
	effects[0].u.periodic.envelope.attack_length = 0x100;
	effects[0].u.periodic.envelope.attack_level = 0;
	effects[0].u.periodic.envelope.fade_length = 0x100;
	effects[0].u.periodic.envelope.fade_level = 0;
	effects[0].trigger.button = 0;
	effects[0].trigger.interval = 0;
	effects[0].replay.length = 20000;  /* 20 seconds */
	effects[0].replay.delay = 0;

	if (ioctl(event_fd, EVIOCSFF, &effects[0]) == -1) {
    cerr << "Failure on 0" << endl;
	}

	/* a strong rumbling effect */
	effects[4].type = FF_RUMBLE;
	effects[4].id = -1;
	effects[4].u.rumble.strong_magnitude = 0x8000;
	effects[4].u.rumble.weak_magnitude = 0;
	effects[4].replay.length = 250;
	effects[4].replay.delay = 0;

	if (ioctl(event_fd, EVIOCSFF, &effects[4]) == -1) {
    cerr << "Failure on 4" << endl;
	}

	/* a weak rumbling effect */
	effects[5].type = FF_RUMBLE;
	effects[5].id = -1;
	effects[5].u.rumble.strong_magnitude = 0;
	effects[5].u.rumble.weak_magnitude = 0xc000;
	effects[5].replay.length = 250;
	effects[5].replay.delay = 0;

	if (ioctl(event_fd, EVIOCSFF, &effects[5]) == -1) {
    cerr << "Failure on 5" << endl;
	}

	return 0;
}

int XBOXController::DoRumble(int Effect) {
  struct input_event play;

  if (Effect < 0 || Effect >= MAX_RUMBLES) {
    return -1;
  }
  play.type = EV_FF;
  play.code = effects[Effect].id;
  play.value = 1;

  if (write(event_fd, (const void *) &play, sizeof(play)) == -1) {
    return -1;
  }

  return 0;
}



void XBOXController::CloseRumble(void) {
  close(event_fd);
  event_fd = 0;
}
