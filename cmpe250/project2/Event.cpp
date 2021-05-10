/*
 * Event.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#include "Event.h"

Event::Event() {
	// TODO Auto-generated constructor stub
	this->type = -1;
	this->time = -1;
	this->process = Process();
	this->cpu = Cpu();
	this->ioDevice = IODevice();
}

Event::Event(int type, double time, Process process, Cpu cpu,
		IODevice ioDevice) {
	this->type = type;
	this->time = time;
	this->process = process;
	this->cpu = cpu;
	this->ioDevice = ioDevice;
}

Event::~Event() {
	// TODO Auto-generated destructor stub
}

