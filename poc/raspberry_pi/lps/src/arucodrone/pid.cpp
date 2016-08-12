/*
 * pid.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: nikovertovec
 */

#include "pid.h"
#include <cmath>
#include <iostream>


// --------------------------------------------------------------------------
//! @brief   Constructor of the PID controller class
//! @return  None
// --------------------------------------------------------------------------
PID::PID(double kp, double ki, double kd):
	_kp(kp),
	_ki(ki),
	_kd(kd),
	_dt(0),
	_max(5.0), //the maximum speed
	_min(-5.0), //the minimum speed (opposite direction)
	_integral(0), //the integral starts at 0
	_pre_error(0)
	{ }

// --------------------------------------------------------------------------
//! @brief   Destructor of the PID controller class
//! @return  None
// --------------------------------------------------------------------------
PID::~PID() { }

// --------------------------------------------------------------------------
//! @brief Initializes the Clock
//! @return  None
// --------------------------------------------------------------------------
void PID::initClock(){
	//SETUP HEARTBEAT TIMER
	clock_gettime(CLOCK_REALTIME, &gettime_now);
	last_heartbeat = gettime_now.tv_nsec;
}

void PID::set(double kp, double ki, double kd){
	_kp = kp;
	_ki = ki;
	_kd = kd;
}

// --------------------------------------------------------------------------
//! @brief calculates the speed according to the distance to fly
//! @param the distance to fly
//! @return the speed from -5 (backwards) to 5 (forwards)
// --------------------------------------------------------------------------
double PID::refresh(double error){

	if(std::isnan(error)) std::cerr << "Error in PID::refresh, cannot except NaN" << std::endl;
	//calculate time difference
	clock_gettime(CLOCK_REALTIME, &gettime_now);
	_dt = (gettime_now.tv_nsec - last_heartbeat) / 1000000000;

	//Proportional
	double p = error * _kp; //accounts for present values of the error.
	if(std::isnan(p)) std::cerr << "Error in PID::refresh, cannot except p, NaN" << std::endl;

	//Integral
	_integral = _integral + error;
	double i = _integral * _ki; //accounts for past values of the error.
	if(std::isnan(i)) std::cerr << "Error in PID::refresh, cannot except i, NaN" << std::endl;

	//Derivative
	double derivative = (error - _pre_error) / _dt;
	double d = derivative * _kd; //accounts for possible future values of the error, based on its current rate of change.
	if(std::isnan(d)) std::cerr << "Error in PID::refresh, cannot except d, NaN" << std::endl;

	//calculate output;
	double output = p + i + d;

	//saves current error as previous error
	_pre_error = error;
	
	//limit to min or max
	if(output > _max)
		return _max;
	if(output < _min)
		return _min;

	return output;
}
