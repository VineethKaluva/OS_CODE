/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShortestNext.h
 * Author: vineeth, Rhys, Tim (Group 10)
 *
 * Created on February 8, 2018, 3:06 PM
 */

#ifndef SHORTESTNEXT_H
#define SHORTESTNEXT_H

#include <queue> 
#include <vector>
#include <string>
#include <fstream>

class ShortestNext {
public:
       
    /*
     * This is a Struct for ShortestNext
     * Same exact struct as roundrobin
     * Takes arrival_time, burst_time, block_interval, ttb, ttb_time_left
     * This contains one default constructor and one constructor along with 
     * parameters
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
        
        Process()
        : name ("Empty"), arrival_time(0), burst_time(0), block_interval(0), ttb(0), ttb_time_left(0) {}
    };
    
    /*SnextCompare - reurns the shortest process
     */
    struct sNextCompare {
    bool operator()(const Process& process1, const Process& process2) const
    {
        return process1.burst_time > process2.burst_time;
    }
    };
    
    ShortestNext(std::vector<Process> processVector, int blockDuration); 
    
    /**
     *These is accomplishing rule of 5
     */
    ShortestNext(const ShortestNext& orig) = delete;
    ShortestNext(ShortestNext&&) = delete;
    ShortestNext& operator=(const ShortestNext&) = delete;
    ShortestNext& operator=(ShortestNext&&) = delete;
    
    /**
     *This is the ShortestNext Destructor
     */
    virtual ~ShortestNext();
    
private:
   /**
    * Queues for Ready and Block for both types of scheduling ShortestNext
    */
    std::priority_queue<Process, std::vector<Process>, sNextCompare> readyQueueSPN;
    std::priority_queue<Process, std::vector<Process>, sNextCompare> blockQueueSPN;
    std::priority_queue<Process, std::vector<Process>, sNextCompare> tblockQueueSPN;
    
    //Main clock
    int mainClock;
    
    //variables to calculate time around
    std::vector<int> turnAroundTimesSPN;
    int nofprocess;
    
   /**
    * SNext - this handles all of the scheduling for ShortestNext
    * @param processVector - processes of only one vector 
    * @param blockDuration - the time for processes being blocked
    */
    void sNext(std::vector<Process> processVector, int blockDuration);
    
   /**
    * resetSN - resets everything including main clock to empty
    */
    void resetSNext();
    
   /**
    * updateBList - updates everything in list and transfers them to ready 
    */
    void updateBList();
    
   /**
    * AvgTurnAround - computes turnaround average
    * @param theVector - vector with the turnaround times
    * @return the value is stored double
    */
    double avgTurnAround(std::vector<int> &theVector);

};



#endif /* SHORTESTNEXT_H */

