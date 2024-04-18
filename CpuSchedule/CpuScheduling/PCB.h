#pragma once

// This file is the place to build the Process control Block (PCB)

using namespace std;

struct Process {
	// The Attributes of the PCB struct

	int id; // Process id
	int AT; // Arrival time
	int BT;    // Burst time
	int timeLeft; // For round robin
	int cs; // Context switch
	int wt; // Waiting time
	int TAT_time; // Turnaround time
	int finishTime;  // Finish time
};