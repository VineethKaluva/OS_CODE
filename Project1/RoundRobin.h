/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RoundRobin.h
 * Author: vineeth, Rhys, Tim (Group 10)
 *
 * Created on February 8, 2018, 3:02 PM
 */

#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <queue> 
#include <vector>
#include <string>
#include <fstream>

class RoundRobin {
public: 

    /* Struct Process contains name, arrival time, burst_time, blocked_interval
     * ttb(time of block), ttb_time_left(how much time is left)
     */
    struct Process {
        std::string name;
        int arrival_time;
        int burst_time;
        int block_interval;
        int ttb; 
        int ttb_time_left;

        Process(std::string n, int arr, int tot, int bloc, int tBloc, int blocLeft)
        : name(std::move(n)), arrival_time(arr), burst_time(tot), block_interval(bloc), ttb(tBloc), ttb_time_left(blocLeft) {}
    };
    
    struct RRCompare {
    bool operator()(const Process& process1, const Process& process2) const
    {
        return process1.arrival_time > process2.arrival_time; 
    }
    };
    
    //RoundRobin Constructor
    RoundRobin(std::vector<Process> processVector, int blockDuration, int timeSlice );
    //RoundRobin rule of 5
    RoundRobin(const RoundRobin& orig) = delete;
    RoundRobin(RoundRobin&&) = delete;
    RoundRobin& operator=(const RoundRobin&) = delete;
    RoundRobin& operator=(RoundRobin&&) = delete;
    //RoundRobin Destructor
    virtual ~RoundRobin();
    
    

private:
 /**
    * Queues for Ready and Block for both types of scheduling RR
    */
    std::deque<Process> readyQueueRR;
    std::priority_queue<Process, std::vector<Process>, RRCompare> blockQueueRR;
    std::priority_queue<Process, std::vector<Process>, RRCompare> tblockQueueRR;
    
    //Main clock
    int mainClock;
    
    //variables calculates average turnaround
    std::vector<int> turnAroundTimes;
    int nofprocess;
    
    /**
    * RRobinScheduler - this hands all of the scheduling
    * @param processVector - processes of only one vector 
    * @param blockDuration - the time for processes being blocked
    * @param timeSlice - the time needed for slicing
    */
    void RRobin(std::vector<Process> processVector, int blockDuration, int timeSlice);
    
    /**
    * resetRR - resets everything including main clock to empty
    */
    void resetRR();
    
    /**
    * updateBList - updates everything in list and transfers them to ready 
    */
    void updateBList();
    
    /**
    * computeAvgTurnAround - computes turnaround average
    * @param theVector - vector with the turnaround times
    * @return the value is stored double
    */
    double avgTurnAround(std::vector<int> &theVector);

};

#endif /* ROUNDROBIN_H */



