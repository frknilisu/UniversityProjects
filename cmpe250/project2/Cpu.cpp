/*
 * Cpu.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#include "Cpu.h"

Cpu::Cpu() {
	// TODO Auto-generated constructor stub
	this->id = -1;
	this->frequency = -1.0;
	this->free = false;
	this->active_time = 0;
}

Cpu::Cpu(int id, double f, bool fr, double at) {
	this->id = id;
	this->frequency = f;
	this->free = fr;
	this->active_time = at;
}

Cpu::~Cpu() {
	// TODO Auto-generated destructor stub
}

