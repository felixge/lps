//
//  flyto.cpp
//  lbs
//
//  Created by Niko Vertovec on 16/07/16.
//  Copyright © 2016 Niko Vertovec. All rights reserved.
//

#include "arucodrone.h"
#include <math.h>

using namespace cv;


// --------------------------------------------------------------------------
//! @brief calculate distance to point
//! @param a point in the realworld
//! @return the distance to the point
// --------------------------------------------------------------------------
double ArucoDrone::distancetofly(Point3d point){
    return sqrt(pow(point.x - drone_location.x, 2) + pow(point.y - drone_location.y, 2) + pow(point.z - drone_location.z, 2));
}


// --------------------------------------------------------------------------
//! @brief calculate vector to point
//! @param a point in the realworld
//! @return a vector that points to the point
// --------------------------------------------------------------------------
Point3d ArucoDrone::vectortofly(Point3d point){
	Point3d vec((drone_location.x - point.x), (drone_location.y - point.y), (drone_location.z - point.z));
	Mat_<double> mat(3,1);
	mat(0,0) = vec.x;
	mat(1,0) = vec.y;
	mat(2,0) = vec.z;
	if(!rot.empty())mat = camerarot * (rot * mat); // must also calculate rotation of camera, this value is fixed
	//cout << "flying to point: " << point << " with vector: " << mat << " from location: " << drone_location << endl;
	return Point3d(mat);
}

// --------------------------------------------------------------------------
//! @brief calculate required speed to marker
//! @param which aches speed is wanted (0=x, 1=y, 2=z, 3=r)
//! @return the speed at which to fly to the marker
// --------------------------------------------------------------------------
double ArucoDrone::getspeed(int x){
	switch(x){
	case 0:
		return 5;
	case 1:
		return 5;
	case 2:
		return 5;
	case 3:
		return 5;
	}
}

// --------------------------------------------------------------------------
//! @brief sets the speeds to fly to a point
//! @param a point to which to fly
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::flytocoords(Point3d point){
	flyto(vectortofly(point));
}


// --------------------------------------------------------------------------
//! @brief sets the speeds to fly along a vector
//! @param a vector that should be flown
//! @return  None
// --------------------------------------------------------------------------
void ArucoDrone::flyto(Point3d vector){
	speed.x = pid_x.refresh((double) vector.x);
	client.gauge("pid_x-error", (float) vector.x);
	speed.y = pid_y.refresh((double) vector.y);
	client.gauge("pid_y-error", (float) vector.y);
	speed.z = pid_z.refresh((double) vector.z);
	client.gauge("pid_z-error", (float) vector.z);



    /*if(vector.x>0){
        int_straight = vector.x;
        straight = true;
    }
    
    if(vector.x<0){
        int_backward = vector.x * -1;
        backward = true;
    }
    
    if(vector.y>0){
        int_port = vector.y;
        port = true;
    }
    
    if(vector.y<0){
        int_starbourd = vector.y * -1;
        starbourd = true;
    }
    
    if(vector.z>0){
        int_up = vector.z;
        up = true;
    }
    
    if(vector.z<0){
        int_down = vector.z * -1;
        down = true;
    }*/
}
