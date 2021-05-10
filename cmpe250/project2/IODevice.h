/*
 * IODevice.h
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#ifndef IODEVICE_H_
#define IODEVICE_H_

class IODevice {
public:
	int id;
	double quantum;
	bool free;
	double active_time;
	IODevice();
	IODevice(int id, double q, bool fr, double at);
	~IODevice();
};

#endif /* IODEVICE_H_ */
