/*
 Student Name: Abdullah Furkan İlısu
 Student Number: 2013400201
 Project Number: 2
 Operating System: Ubuntu(16.10) - Linux
 Compile Status: Done
 Program Status: Done
 Notes: Anything you want to say about your code that will be helpful in the grading process.

	 I also print logs.

 */
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <queue>
#include "Event.h"
#include "Process.h"
#include "Cpu.h"
#include "IODevice.h"
using namespace std;

class Compare_Processes {
public:
	bool operator()(const Process& p1, const Process& p2) {
		return p1.cpu_work > p2.cpu_work;
	}
};

class Compare_Events {
public:
	bool operator()(const Event& e1, const Event& e2) {
		return e1.time > e2.time;
	}
};

int N; 	//number of CPUs
int M; 	//number of output devices units
int P; 	//number of tasks to process
vector<Cpu> Cpus;
vector<IODevice> IODevices;
vector<Process> processes;

priority_queue<Process, vector<Process>, Compare_Processes> cpu_queue;
priority_queue<Event, vector<Event>, Compare_Events> events;
queue<Process> io_queue;

int max_len_cpu_queue, max_len_io_queue;
int max_active_cpu_id, max_active_io_id;
double longest_wait_time;

void readInput(char* path) {
	ifstream in;
	in.open(path);
	in >> N;
	double x, y, z;
	for (int i = 0; i < N; i++) {
		in >> x;
		Cpus.push_back(Cpu(i, x, true, 0.0));
	}
	in >> M;
	for (int i = 0; i < M; i++) {
		in >> x;
		IODevices.push_back(IODevice(i, x, true, 0.0));
	}
	in >> P;
	for (int i = 0; i < P; i++) {
		in >> x >> y >> z;
		processes.push_back(Process(i, x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	}
}

int findFreeCpu() {
	for (int i = 0; i < N; i++)
		if (Cpus[i].free)
			return i;
	return -1;
}

int findFreeIOUnit() {
	for (int i = 0; i < M; i++)
		if (IODevices[i].free)
			return i;
	return -1;
}

void DES(char* path) {
	string _path(path);
	int index = _path.find("input");
	string logPath = _path.substr(0, index) + "log" + _path.substr(index + 5);
	cout << logPath << endl;
	const char * _log = logPath.c_str();
	ofstream log;
	log.open(_log);
	for (int i = 0; i < P; i++) {
		events.push(
				Event(1, processes[i].arrival_time, processes[i], Cpu(),
						IODevice()));
	}

	while (!events.empty()) {

		max_len_cpu_queue =
				max_len_cpu_queue < cpu_queue.size() ?
						cpu_queue.size() : max_len_cpu_queue;
		max_len_io_queue =
				max_len_io_queue < io_queue.size() ?
						io_queue.size() : max_len_io_queue;

		Event currentEvent = events.top();

		if (currentEvent.type == 1) {	//	new process come to CPU
			int cpu_id = findFreeCpu();
			if (cpu_id != -1) { 	// 	there is a free cpu
				double finishTime = currentEvent.time
						+ (currentEvent.process.cpu_work
								/ Cpus[cpu_id].frequency);
				log << "T:" << currentEvent.time << endl;
				log << "\tprocess#" << currentEvent.process.id
						<< " goes to CPU#" << cpu_id << " after waiting "
						<< currentEvent.process.cpuq_waiting << " seconds"
						<< endl;
				log << "\tCPU departure event set at T=" << finishTime << endl;
				Cpus[cpu_id].free = false;
				Cpus[cpu_id].active_time += (currentEvent.process.cpu_work
						/ Cpus[cpu_id].frequency);
				events.push(
						Event(2, finishTime, processes[currentEvent.process.id],
								Cpus[cpu_id], IODevice()));
			} else { 	// 	no free cpu at this time => put cpu_queue
				currentEvent.process.entry_time_to_cpu_queue =
						currentEvent.time;
				cpu_queue.push(currentEvent.process);
				log << "T:" << currentEvent.time << endl;
				log << "\tprocess#" << currentEvent.process.id
						<< " goes to first level queue" << endl;
			}
			log << endl;
		} else if (currentEvent.type == 2) { //	a process finish on one of cpus
			log << "T:" << currentEvent.time << endl;
			log << "\tprocess#" << currentEvent.process.id << " is done at CPU#"
					<< currentEvent.cpu.id << endl;
			Cpus[currentEvent.cpu.id].free = true;
			log << endl;

			events.push(
					Event(3, currentEvent.time,
							processes[currentEvent.process.id], Cpu(),
							IODevice()));

			// 	get next process from cpu_queue
			if (!cpu_queue.empty()) {
				Process nextProcess = cpu_queue.top();
				cpu_queue.pop();
				processes[nextProcess.id].cpuq_waiting = currentEvent.time
						- nextProcess.entry_time_to_cpu_queue;
				processes[nextProcess.id].total_waiting +=
						processes[nextProcess.id].cpuq_waiting;
				double finishTime = currentEvent.time
						+ (nextProcess.cpu_work
								/ Cpus[currentEvent.cpu.id].frequency);
				log << "T:" << currentEvent.time << endl;
				log << "\tprocess#" << nextProcess.id << " goes to CPU#"
						<< currentEvent.cpu.id << " after waiting "
						<< processes[nextProcess.id].cpuq_waiting << " seconds"
						<< endl;
				log << "\tCPU departure event set at T=" << finishTime << endl;
				Cpus[currentEvent.cpu.id].free = false;
				Cpus[currentEvent.cpu.id].active_time += finishTime
						- currentEvent.time;
				events.push(
						Event(2, finishTime, processes[nextProcess.id],
								Cpus[currentEvent.cpu.id], IODevice()));
				log << endl;
			}
		} else if (currentEvent.type == 3) { // 	new process come to IODevices

			//	give output of first level to second level
			int io_id = findFreeIOUnit();
			if (io_id != -1) { 	//	there is a free io device
				double finishTime = -1;
				if (currentEvent.process.output_device_work
						> IODevices[io_id].quantum) {
					finishTime = currentEvent.time + IODevices[io_id].quantum;
					processes[currentEvent.process.id].output_device_work -=
							IODevices[io_id].quantum;
					IODevices[io_id].active_time += IODevices[io_id].quantum;
				} else if (processes[currentEvent.process.id].output_device_work
						> 0) {
					finishTime = currentEvent.time
							+ currentEvent.process.output_device_work;
					processes[currentEvent.process.id].output_device_work -=
							currentEvent.process.output_device_work;
					IODevices[io_id].active_time +=
							currentEvent.process.output_device_work;
				}

				if (finishTime != -1) {
					log << "T:" << currentEvent.time << endl;
					log << "\tprocess#" << currentEvent.process.id
							<< " goes to IO_Unit#" << io_id << " after waiting "
							<< processes[currentEvent.process.id].ioq_waiting
							<< " seconds in the queue and "
							<< processes[currentEvent.process.id].total_waiting
							<< " in total" << endl;
					log << "\tIO Unit departure event set at T=" << finishTime
							<< endl;
					IODevices[io_id].free = false;

					events.push(
							Event(4, finishTime,
									processes[currentEvent.process.id], Cpu(),
									IODevices[io_id]));
				}
			} else { 	// 	no free io device => put io_queue
				currentEvent.process.entry_time_to_io_queue = currentEvent.time;
				io_queue.push(currentEvent.process);
				log << "T:" << currentEvent.time << endl;
				log << "\tprocess#" << currentEvent.process.id
						<< " goes to second level queue" << endl;
			}

			log << endl;
		} else if (currentEvent.type == 4) { //	a process finish on one of IODevices
			log << "T:" << currentEvent.time << endl;
			log << "\tprocess#" << currentEvent.process.id
					<< " is done at IO_Unit#" << currentEvent.ioDevice.id
					<< endl;
			log << endl;

			if (processes[currentEvent.process.id].output_device_work > 0) {
				currentEvent.process.entry_time_to_io_queue = currentEvent.time;
				io_queue.push(currentEvent.process);
				log << "T:" << currentEvent.time << endl;
				log << "\tprocess#" << currentEvent.process.id
						<< " goes to second level queue" << endl;
				log << endl;
			} else {
				processes[currentEvent.process.id].exit_time =
						currentEvent.time;
			}

			IODevices[currentEvent.ioDevice.id].free = true;

			if (!io_queue.empty()) {
				Process nextProcess = io_queue.front();
				io_queue.pop();
				processes[nextProcess.id].ioq_waiting = currentEvent.time
						- nextProcess.entry_time_to_io_queue;
				processes[nextProcess.id].total_waiting +=
						+processes[nextProcess.id].ioq_waiting;
				events.push(
						Event(3, currentEvent.time, processes[nextProcess.id],
								Cpu(), IODevice()));
			}
		}
		events.pop();
	}

	log << "Process finished with exit code 0";
}

void writeOutput(char* path) {
	FILE * out;
	out = fopen(path, "w+");

	fprintf(out, "%d\n%d\n", max_len_cpu_queue, max_len_io_queue);

	double max_active_time = 0;
	for (int i = 0; i < N; i++) {
		if (max_active_time < Cpus[i].active_time) {
			max_active_time = Cpus[i].active_time;
			max_active_cpu_id = i + 1;
		}
	}
	fprintf(out, "%d\n", max_active_cpu_id);

	max_active_time = 0;
	for (int i = 0; i < M; i++) {
		if (max_active_time < IODevices[i].active_time) {
			max_active_time = IODevices[i].active_time;
			max_active_io_id = i + 1;
		}
	}
	fprintf(out, "%d\n", max_active_io_id);

	double total_wait_time = 0.0;
	for (int i = 0; i < P; i++) {
		total_wait_time += processes[i].total_waiting;
		if (longest_wait_time < processes[i].total_waiting)
			longest_wait_time = processes[i].total_waiting;
	}
	double avg_wait_time = total_wait_time / P;
	fprintf(out, "%f\n%f\n", avg_wait_time, longest_wait_time);

	double total_time_spent = 0.0;
	for (int i = 0; i < P; i++) {
		total_time_spent += processes[i].exit_time - processes[i].arrival_time;
	}
	double avg_time_spent = total_time_spent / P;
	fprintf(out, "%f\n", avg_time_spent);
	fclose(out);

}

int main(int argc, char* argv[]) {
	readInput(argv[1]);
	DES(argv[1]);
	writeOutput(argv[2]);
	return 0;
}

