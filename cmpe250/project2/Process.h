/*
 * Process.h
 *
 *  Created on: Nov 11, 2016
 *      Author: frkn
 */

#ifndef PROCESS_H_
#define PROCESS_H_

class Process {
public:
	int id;
	double arrival_time, cpu_work, output_device_work;
	double entry_time_to_cpu_queue, entry_time_to_io_queue;
	double cpuq_waiting, ioq_waiting, total_waiting;
	double exit_time;
	Process();
	Process(int id, double a, double c, double o, double cw, double iw,
			double tw, double e1, double e2, double et);
	~Process();
};

#endif /* PROCESS_H_ */
