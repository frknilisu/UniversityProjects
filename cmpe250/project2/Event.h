/*
 * Event.h
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "Cpu.h"
#include "IODevice.h"
#include "Process.h"

class Event: public Process, public Cpu, public IODevice {
public:
	int type;
	double time;
	Process process;
	Cpu cpu;
	IODevice ioDevice;
	Event();
	Event(int type, double time, Process process, Cpu cpu, IODevice ioDevice);
	~Event();
};

#endif /* EVENT_H_ */
