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
    sorty();
	getInfo();  // call getInfo function to display Gantt Chart & other information  
}

// Function for SJF (nonpreemptive)
void SJF() {
    cout << "SJF (nonpreemptive)\n";
    sorty(); // Ensure processes are sorted by arrival time
    int currentTime = 0;

    for (int i = 0; i < numberOfProcesses; i++) {
        // Find the process with the minimum burst time from the set of processes that have arrived
        int idx = -1;
        int minBT = INT_MAX; // Initialize minBT to the maximum possible for comparison
        for (int j = i; j < numberOfProcesses; j++) {
            if (process[j].AT <= currentTime && process[j].BT < minBT) {
                minBT = process[j].BT;
                idx = j;
            }
        }
        if (idx != -1 && idx != i) {
            swap(process[i], process[idx]); // Swap the current process with the one with the minimum burst time
        }
        currentTime += process[i].BT + cs; // Update the current time by adding the burst time of the current process and context switch time
    }
    getInfo(); // Calculate and display information
}

//void SRTF() {
//    sorty(); // Sort processes by arrival time
//    int remainingTime[5]; // Remaining burst time for each process
//    int minIndex; // Index of the process with the shortest remaining time
//
//    // Initialize remaining time array
//    for (int i = 0; i < numberOfProcesses; i++) {
//        remainingTime[i] = process[i].BT;
//    }
//
//    int currentTime = 0; // Current time
//    int completed = 0; // Number of completed processes
//
//    // Keep scheduling until all processes are completed
//    while (completed < numberOfProcesses) {
//        int shortestTime = INT_MAX; // Initialize shortest remaining time to maximum possible value
//
//        // Find the process with the shortest remaining time that has arrived
//        for (int i = 0; i < numberOfProcesses; i++) {
//            if (process[i].AT <= currentTime && remainingTime[i] < shortestTime && remainingTime[i] > 0) {
//                shortestTime = remainingTime[i];
//                minIndex = i;
//            }
//        }
//
//        // Reduce remaining time of the selected process
//        remainingTime[minIndex]--;
//
//        // If the process is completed
//        if (remainingTime[minIndex] == 0) {
//            completed++;
//            int finishTime = currentTime + 1; // Finish time of the completed process
//            process[minIndex].TAT_time = finishTime - process[minIndex].AT; // Turnaround time
//            process[minIndex].wt = finishTime - process[minIndex].AT - process[minIndex].BT; // Waiting time
//            currentTime = finishTime; // Update current time
//
//        }
//        else {
//            currentTime++; // Move to the next time unit
//        }
//    }
//
//    // Display Gantt chart
//    GanttChart();
//
//    // Display results
//    cout << "Shortest Remaining Time First (SRTF)\n";
//    cout << "Process\t| Turnaround Time | Waiting Time | Finish Time\n";
//    for (int i = 0; i < numberOfProcesses; i++) {
//        cout << "P(" << process[i].id << ")\t|\t" << process[i].TAT_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].AT + process[i].TAT_time << endl;
//        totalWT += process[i].wt;
//        totalTAT += process[i].TAT_time;
//    }
//    cout << "\nAverage Waiting Time= " << (double)totalWT / numberOfProcesses << endl;
//    cout << "Average Turnaround Time= " << (double)totalTAT / numberOfProcesses << endl;
//    cpuUtilization = (double)totalBurstTime * 100.0 / currentTime;
//    cout << "CPU Utilization= " << cpuUtilization << "%" << endl;
//}



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

    for (i = 0; i < numberOfProcesses; i++) { // To set each process's left time
        process[i].timeLeft = process[i].BT;
    }

    cout << "====================================================================================================" << endl;
    for (currentTime = 0, i = 0; remain_processes != 0;) {
        if (process[i].timeLeft > 0) { // Check if process is not finished
            cout << "(" << currentTime << ")|==P" << process[i].id << "==|";

            if (process[i].timeLeft <= q) { // If burst time is smaller than or equal to quantum 
                currentTime += process[i].timeLeft + cs;
                process[i].timeLeft = 0;
                flag = 1;
            }
            else { // If burst time is greater than quantum
                process[i].timeLeft -= q;
                currentTime += q + cs;
            }

            if (process[i].timeLeft == 0 && flag == 1) { // If we finish process 
                remain_processes--; // decrement number of processes
                process[i].finishTime = currentTime - cs;
                process[i].TAT_time = currentTime - process[i].AT; // to calculate turn around time
                process[i].wt = process[i].TAT_time - process[i].BT; // waiting time
                flag = 0;
            }
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

    cout << "(" << currentTime << ")" << endl; // Print the last current time
    cout << "====================================================================================================" << endl;
    totalWT = 0;
    totalTAT = 0;

    for (int i = 0; i < numberOfProcesses; i++) {
        //to calculate Total WT 
        totalWT += process[i].wt;
        //to calculate total TAT
        totalTAT += process[i].TAT_time;
    }

    cout << "Process\t|TurnAround Time| Waiting Time  |  Finish Time\n";
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