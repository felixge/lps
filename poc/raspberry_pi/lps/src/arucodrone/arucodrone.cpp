/*
 * arucodrone.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: nikovertovec
 */

#include "arucodrone.h"
#include <unistd.h>

using namespace std;

cv::Point3d drone_location;

// --------------------------------------------------------------------------
//! @brief   Constructor of the Aruco Drone class
//! @return  None
// --------------------------------------------------------------------------
ArucoDrone::ArucoDrone() :
	tick(0),
	pid_x(0.1,0,0),
	pid_y(0.1,0,0),
	pid_z(0.1,0,0)
	{}


// --------------------------------------------------------------------------
//! @brief Initializes Drone
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::initialize_drone(){
    cout << "Initializing drone" << endl;
    // Initialize
    if (!open()) {
        cout << "Failed to initialize." << endl;
        return;
    }

    // Battery
    cout << "Battery = " << getBatteryPercentage() << "[%]" << endl;

    // Outdoor mode
    setOutdoorMode(false);

    //set all speeds to zero
    speed.x = 0;
    speed.y = 0;
    speed.z = 0;
    command = hold;
    return;
}

// --------------------------------------------------------------------------
//! @brief   Destructor of the Aruco Drone class
//! @return  None
// --------------------------------------------------------------------------
ArucoDrone::~ArucoDrone() {
	close();
}


// --------------------------------------------------------------------------
//! @brief the main loop function during the flight that executes all other functions
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::fly(){

    // detect marker and updates the drone_location
    detect();

    //this will be the move function
    move3D(speed.x, speed.y, speed.z, 0); //currently not able to rotate

    //move.cpp and this function will be removed in later versions
    //move3D(vx(), vy(), vz(), vr());

    //check will be removed in later versions
    //check();

    switch(command){
    	case off: break;
    	case hold:
        	flytocoords(holdpos);
        	break;
    	case land:
    		landing();
    		break;
    	case start:
    		if (onGround()) takeoff();
    		usleep(5000);
    		holdpos = drone_location;
    		break;
    }

    tick++;
}

// --------------------------------------------------------------------------
//! @brief Initializes the Ar.Drone, the Marker Detector, the PID controllers and the Commandline input thread
//! @return None
// --------------------------------------------------------------------------
void ArucoDrone::initAll(){
	//Initialize the AR Drone
	initialize_drone();

	//Initialize the Camera and the marker detect function
	initialize_detection();

	//Initialize PID clock
	pid_x.initClock();
	pid_y.initClock();
	pid_z.initClock();

	//Initialize thread to get input
	initialize_thread();
}

// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the GPS
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d ArucoDrone::get_GPS_position(){
	double x,y,z;
	getPosition(&x,&y,&z);
	return cv::Point3d(x,y,z);
}
