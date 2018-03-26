/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RoundRobin.cpp
 * Author: vineeth, Rhys, Tim (Group 10)
 * 
 * Created on February 8, 2018, 3:02 PM
 */

#include "RoundRobin.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <iterator>
#include <queue>

using std::endl;
using std::cout;


/*Our program is creating a simulation through our Round-Robin (preemptive) scheduling
 * See ShortestNext for the 2nd scheduling
 * We have both RoundRobin and ShortestNext starting at 0.  
 */

/*
 * Round Robin Constructor 
 *@Parm ProcessVector   structure of vectors  processes 
 *@blockDuration        time needed for each process to get blocked
 **timeSlice            amount of the time a process needs to run
 */
 
RoundRobin::RoundRobin(std::vector<Process> processVector, int blockDuration, int timeSlice ) {

    mainClock = 0;
    nofprocess = processVector.size();
        
/*Round Robin Function 
 *@Parm ProcessVector   structure of vectors  processes 
 *@blockDuration        time needed for each process to get blocked
 *timeSlice            amount of the time a process needs to run*@param 
 */
    RRobin(processVector, blockDuration, timeSlice);

}


/*
 * For each line, the process either run or be idle and state so.
 * if it is running, it will input B (blocked), S(Time slice ended), T(termination of process), 
 * or I(interval is Idle).  
 */

void RoundRobin::RRobin(std::vector<Process> processVector, int blockDuration, int timeSlice){
    
    cout << "RR " << blockDuration << " " << timeSlice << endl;
    
    blockDuration--;
    
    //reset roundrobin each time so no data present in priority queues
    resetRR();
        
    for (int i = 0; i < processVector.size(); i++)
    {
        readyQueueRR.push_back(processVector[i]);
    }
    
    bool keep_going = true;
    int pmainClock = 0;
    int RR = 0;
    
    while((readyQueueRR.size() != 0) || (blockQueueRR.size() != 0))
    {
        pmainClock = mainClock;
        int timeSliceClock = 0;
        
        if (readyQueueRR.size() == 0) 
        {
            //This is checking for idle conditions
            while ((readyQueueRR.size() == 0) && (blockQueueRR.size() != 0))
            {
                mainClock++;
                timeSliceClock++;
                updateBList();
            }
            cout << " " << pmainClock << "\t" << "<idle>" << "\t" << timeSliceClock << "\t" << " I" << endl;
        }
        else
        {
            int totTimeTest = 0;             
            while (totTimeTest == 0)
            { 
                if (!readyQueueRR.empty())
                {  
                    totTimeTest = readyQueueRR.at(0).burst_time;
                }
                else
                {  
                    break;
                }
            } 
            if(totTimeTest != 0)
            {    
                Process currProc = readyQueueRR.at(0);
                if (!readyQueueRR.empty())
                { 
                    readyQueueRR.pop_front();
                }
                
                int timetilBlock = currProc.ttb;
                int totalTimeLeft = currProc.burst_time;

                int timer = timeSlice;
                keep_going = true;
                while(timer > 0 && keep_going)
                {   
                    mainClock++;
                    timer--;
                    timeSliceClock++;
                    
                    timetilBlock--;
                    totalTimeLeft--;

                    updateBList();

                    if ((timetilBlock == 0) || (totalTimeLeft == 0))
                    { 
                        keep_going = false;

                        if (totalTimeLeft > 0)
                        { 
                            currProc.ttb = currProc.block_interval;

                            currProc.ttb_time_left = blockDuration;
                            currProc.burst_time = totalTimeLeft;
                            blockQueueRR.push(currProc);
                            
                            cout << " " << pmainClock << "\t" << currProc.name << "\t" << timeSliceClock << "\t" << " B " << endl ;
                        } 
                        else
                        { 
                            turnAroundTimes.push_back(mainClock - currProc.arrival_time);                                   
                            
                            cout << " " << pmainClock << "\t" 
                            << currProc.name << "\t" 
                            << timeSliceClock << "\t" << " T " << endl ;
                        }
                    } 
                }

                if (keep_going)
                {                     
                    cout << " " << pmainClock << "\t" 
                        << currProc.name << "\t" 
                        << timeSliceClock << "\t" << " S " << endl;
                    
                    currProc.ttb =  timetilBlock;
                    currProc.burst_time = totalTimeLeft;
                    
                    currProc.ttb_time_left = blockDuration;
                                            
                    readyQueueRR.push_back(currProc); 
                }     
            }
        }
    
    }
    cout << " " << mainClock <<  "\t" << "<done>" << "\t";
    printf("%0.3f", avgTurnAround(turnAroundTimes));
    cout << endl;  
}


double RoundRobin::avgTurnAround(std::vector<int> &theVector)
{
    double theTotal = 0;
    for (int i = 0; i < theVector.size(); i++)
    {
        theTotal += theVector[i];
    }
    
    return theTotal/nofprocess;
}

void RoundRobin::updateBList()
{    

        while(!blockQueueRR.empty()){
            RoundRobin::Process temp =blockQueueRR.top();
            blockQueueRR.pop();
            
            if (temp.burst_time > 0)
            {                
                if (temp.ttb_time_left == 0)
                {
                    readyQueueRR.push_back(temp);
                }
                else
                {
                    temp.ttb_time_left--;
                    tblockQueueRR.push(temp);
                }    
            }
        }
        
        while(!tblockQueueRR.empty()){
            RoundRobin::Process temp2=tblockQueueRR.top();
            tblockQueueRR.pop();           
            blockQueueRR.push(temp2);
        }
    }

void RoundRobin::resetRR(){
        
    mainClock = 0;
    
    turnAroundTimes.empty();
    
    readyQueueRR.empty();
    blockQueueRR.empty();
}

RoundRobin::~RoundRobin() {
}


