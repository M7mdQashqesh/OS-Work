#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "PCB.h"
#include "Helpers.h"

using namespace std;

// Function for FCFS
void FCFS() {
	cout << "First Come First is \n";
	sorty();  // callsorty function to rearange processes
	getInfo();  // call getInfo function to display Gantt Chart & other information  

}

// Function for SJF (nonpreemptive)
void SJF() {
	cout << "SJF (nonpreemptive)\n";
	sorty();     // callsorty function to rearange processes
	int check = 0;      // check is counter to check if the process has arrived
	for (int i = 0; i < numberOfProcesses - 1; i++) {
		check += process[i].BT;       // inserting arrival time of each process to be looped ..
		for (int j = i + 1; j < numberOfProcesses - 1; j++) {
			if (check >= process[j + 1].AT && process[j].BT > process[j + 1].BT) { // sorting by burst time and if process arrived
				swap(process[j], process[j + 1]);
			}
		}
	}
	getInfo();
}

// Function for Round Robin Scheduling with q time quantum 
void RR() {
	sorty();
	cout << "Round Robin\n";
	cout << " \n";
	int totalProcessTime, remain_processes, flag = 0;
	remain_processes = numberOfProcesses; // to decide how many processes remain (not finish)
	totalProcessTime = 0;
	int i;
	cout << "\n  ===================" << endl; // To display Gantt Chart 
	cout << "//    Gantt Chart   //" << endl;
	cout << "  ===================" << endl << endl;

	for (i = 0; i < numberOfProcesses; i++) { // To set each processes left time
		process[i].timeLeft = process[i].BT;
	}

	cout << "========================================================================================================================" << endl;
	for (currentTime = 0, i = 0; remain_processes != 0;) {
		cout << "(" << currentTime << ")|==P" << process[i].id << "==|";

		if (process[i].timeLeft <= q && process[i].timeLeft > 0) { // If burst time is smaller than or equal quantoum 
			currentTime += process[i].timeLeft + cs;
			process[i].timeLeft = 0;
			flag = 1;
		}

		else if (process[i].timeLeft > 0) { // If burst time is greater than quantoum
			process[i].timeLeft -= q;
			currentTime += q + cs;
		}

		if (process[i].timeLeft == 0 && flag == 1) { // If we finish process 
			remain_processes--; // decremente number of processes
			process[i].finishTime = currentTime - cs;
			process[i].TAT_time = currentTime - process[i].AT; // to calculate turn around time
			process[i].wt = currentTime - process[i].AT; // waiting time			

			flag = 0;
		}

		if (i == numberOfProcesses - 1) {
			i = 0; //if finish all processes
		}

		else if (process[i + 1].AT <= currentTime) {
			i++;
		}

		else {
			i = 0;
		}

	}

	cout << "\n========================================================================================================================" << endl;
	totalWT = 0;
	totalTAT = 0;

	for (int i = 0; i < 5; i++) {
		process[i].TAT_time = process[i].wt + process[i].BT;
		//to calculate Tptal WT 
		totalWT += process[i].wt;
		//to claculate total TAT
		totalTAT += process[i].TAT_time;
	}

	cout << "(" << currentTime << ")" << endl;
	cout << "Process\t|TurnAround Time| Waiting Time  |  finish Time\n";
	totalBurstTime = 0;

	for (int i = 0; i < numberOfProcesses; i++) {
		//to set process turn around time
		cout << "P(" << process[i].id << ")\t|\t" << process[i].TAT_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].finishTime << endl;
		totalBurstTime += process[i].BT;
	}

	cout << endl;
	//to show Average times
	cout << "Avg Waiting time = " << (double)totalWT * 1.0 / numberOfProcesses << endl;
	cout << "Avg Turnaround time = " << (double)totalTAT / numberOfProcesses << endl;
	cpuUtilization = float(totalBurstTime * 100.0 / (currentTime));
	cout << "Cpu Utilization = " << cpuUtilization << "%" << endl;
}