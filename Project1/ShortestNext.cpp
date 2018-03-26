/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShortestNext.cpp
 * Author: vineeth, Rhys, Tim (Group 10)
 * 
 * Created on February 8, 2018, 3:06 PM
 */

#include "ShortestNext.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <iterator>
#include <queue>

using std::endl;
using std::cout;





/*Our program is creating a simulation through our ShortesNext (nonpreemptive) scheduling
 * See RoundRobint for the 1st scheduling
 * We have both RoundRobin and ShortestNext starting at 0.  
 */

/*
 * Shourtest Next Constructor 
 *@Parm ProcessVector   structure of vectors  processes 
 *@blockDuration        time needed for each process to get blocked
 */


ShortestNext::ShortestNext(std::vector<Process> processVector, int blockDuration) {  
    mainClock = 0;
    nofprocess = processVector.size();   
    sNext(processVector, blockDuration);
}

void ShortestNext::sNext(std::vector<Process> processVector, int blockDuration){
    
    cout << "SPN " << blockDuration << " " << endl;
    
    resetSNext();
    
    Process temp = Process();
    int startTime = processVector[0].arrival_time +1;
    
    for (int i = 0; i < processVector.size(); i++)
    {
        if (processVector[i].arrival_time < startTime)
        {
            if (temp.name != "Empty")
            {
                blockQueueSPN.push(temp);
            }  
            
            temp = processVector[i];
            startTime = processVector[i].arrival_time;
        }  
        else
        {
            processVector[i].ttb_time_left = processVector[i].arrival_time;
            blockQueueSPN.push(processVector[i]);
        } 
    }
    
    readyQueueSPN.push(temp);    
    int pmainClock = 0;
    int sNext = 0;
    
    while((readyQueueSPN.size() != 0) || (blockQueueSPN.size() != 0))
    {   
        int timeSliceClock = 0;
        pmainClock = mainClock;
        
        if (readyQueueSPN.size() == 0) 
        {
            while ((readyQueueSPN.size() == 0) && (blockQueueSPN.size() != 0))
            {
                mainClock++;
                timeSliceClock++;
                updateBList();
            }
            cout << " " << pmainClock << "\t" << "<idle>" << "\t" << timeSliceClock << "\t" << "I" << endl;
        }
        else
        {
            if (!readyQueueSPN.empty())
            {    
                Process first = readyQueueSPN.top();
                bool terminated = false;
                int currentBlockInterval = first.block_interval;
                int timerSPN = currentBlockInterval;
                int totalTimeLeft = first.burst_time;
                int arrivalTime = first.arrival_time;
    
                if(!readyQueueSPN.empty())
                {  
                    readyQueueSPN.pop();
                }
                                 
                while(timerSPN > 0)
                {
                    timerSPN--;
                    totalTimeLeft--;
                    mainClock++;

                    updateBList();                
                    if (totalTimeLeft <= 0)
                    {
                        cout << " " << pmainClock << "\t" << first.name << "\t" << (currentBlockInterval - timerSPN) << "\t" << "T" << endl;
                        terminated = true;

                        turnAroundTimesSPN.push_back(pmainClock + (currentBlockInterval - timerSPN) - arrivalTime);
                        break;
                    }
                }
            
                if (!terminated)
                { 
                    cout << " " << pmainClock << "\t" << first.name << "\t" << currentBlockInterval << "\t" << "B" << endl;

                    first.burst_time = totalTimeLeft;
                    first.ttb_time_left = (blockDuration-1);
                    blockQueueSPN.push(first);
                }
            }
    }
    }
    
    cout << " " << mainClock <<  "\t" << "<done>" <<  "\t";
    printf("%0.2f", avgTurnAround(turnAroundTimesSPN));
    cout << endl;        
}

double ShortestNext::avgTurnAround(std::vector<int> &theVector)
{
    double theTotal = 0;
    for (int i = 0; i < theVector.size(); i++)
    {
        theTotal += theVector[i];
    }
    
    return theTotal/nofprocess;
}

void ShortestNext::updateBList()
{    
  
        while(!blockQueueSPN.empty()){
            ShortestNext::Process tempSPN =blockQueueSPN.top();
            blockQueueSPN.pop();
            
            if (tempSPN.burst_time > 0)
            {
                if (tempSPN.ttb_time_left == 0)
                {
                    readyQueueSPN.push(tempSPN);
                }
                else
                {
                    tempSPN.ttb_time_left--;
                    tblockQueueSPN.push(tempSPN);
                }    
            }
        }
        
        while(!tblockQueueSPN.empty()){
            ShortestNext::Process tempSPN2=tblockQueueSPN.top();
            tblockQueueSPN.pop();
            
            blockQueueSPN.push(tempSPN2);
        }
    }    
    

void ShortestNext::resetSNext(){
        
    mainClock = 0; 
    turnAroundTimesSPN.empty();
    readyQueueSPN.empty();
    blockQueueSPN.empty(); 
}

ShortestNext::~ShortestNext() {
}
