/*
 * Process.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#include "Process.h"

Process::Process() {
	// TODO Auto-generated constructor stub
	this->id = -1;
	this->arrival_time = -1;
	this->cpu_work = -1;
	this->output_device_work = -1;
	this->cpuq_waiting = 0;
	this->ioq_waiting = 0;
	this->total_waiting = 0;
	this->entry_time_to_cpu_queue = 0;
	this->entry_time_to_io_queue = 0;
	this->exit_time = 0;
}

Process::Process(int id, double a, double c, double o, double cw, double iw,
		double tw, double e1, double e2, double et) {
	this->id = id;
	this->arrival_time = a;
	this->cpu_work = c;
	this->output_device_work = o;
	this->cpuq_waiting = cw;
	this->ioq_waiting = iw;
	this->total_waiting = tw;
	this->entry_time_to_cpu_queue = e1;
	this->entry_time_to_io_queue = e2;
	this->exit_time = et;
}

Process::~Process() {
	// TODO Auto-generated destructor stub
}

