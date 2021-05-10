/*
 * IODevice.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#include "IODevice.h"

IODevice::IODevice() {
	// TODO Auto-generated constructor stub
	this->id = -1;
	this->quantum = -1;
	this->free = false;
	this->active_time = 0;
}

IODevice::IODevice(int id, double q, bool fr, double at) {
	this->id = id;
	this->quantum = q;
	this->free = fr;
	this->active_time = at;
}

IODevice::~IODevice() {
	// TODO Auto-generated destructor stub
}

