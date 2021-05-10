/*
 * Cpu.h
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#ifndef CPU_H_
#define CPU_H_

class Cpu {
public:
	int id;
	double frequency;
	bool free;
	double active_time;
	Cpu();
	Cpu(int id, double f, bool free, double at);
	~Cpu();
};

#endif /* CPU_H_ */
