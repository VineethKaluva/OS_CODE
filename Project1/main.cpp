/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vineeth, Rhys, Tim (Group 10)
 *
 * Created on February 7, 2018, 2:57 PM
 */
#include "RoundRobin.h"
#include "ShortestNext.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;
using std::endl;
using std::fstream;
using std::hex;

bool readNextLine(fstream &filestream, vector<RoundRobin::Process> &RRProcess, vector<ShortestNext::Process> &SNProcess)
{
    /*Get the name, arrival_time, total_time, block_interval
     *@Param RoundRobin::Process oneProcess is used for RoundRobin Class;
     *@Parm ShortestNext::Process twoProcess is used for ShortestNext Class;
     */
     
     //Prints first line of the file each and every line
     string nextLine;
     
    
    if (std::getline(filestream, nextLine)) 
    {
        // Make a string stream from command line
        istringstream lineStream(nextLine);
        string elem;
        vector<string> data;
        
        //get 4 elements "name, arrival_time, total_time, and block_interval"
        while(lineStream >> elem)
        {
            data.push_back(elem);

        }
        
        RoundRobin::Process oneProcess(data[0],atoi(data.at(1).c_str()),atoi(data.at(2).c_str()),atoi(data.at(3).c_str()),atoi(data.at(3).c_str()),0);
        RRProcess.push_back(oneProcess);
        ShortestNext::Process twoProcess(data[0],atoi(data.at(1).c_str()),atoi(data.at(2).c_str()),atoi(data.at(3).c_str()),atoi(data.at(3).c_str()),0);
        SNProcess.push_back(twoProcess);     
        return true; 
    }
    else
    {
        return false;
    }
}

int main(int argc, char** argv) {

    std::vector<RoundRobin::Process> RRProcess;
    std::vector<ShortestNext::Process> SNProcess;
    //The arguments are filename, block duration and timeslice   
    // Check argument is printing for name, arrival_time, total_time, and block_time
    if(argc != 4) {
        cerr << "Arguments missing" << endl;
        exit(1);
    }    
    //Read the arguments
    string fileName = argv[1];
    int block_duration = atoi(argv[2]);
    int tslice = atoi(argv[3]);   
    //Open the file, read line by line, save the processes in list
    fstream filestream;

    // Open the file; abort program if it can't open
    filestream.open(fileName, std::ios_base::in);
    if (!filestream.is_open()) {
        cerr << "ERROR: failed to open file: " << fileName << "\n";
        exit(2);
    }
    while(readNextLine(filestream, RRProcess, SNProcess))
    {
        continue;
    }
    //Create and Call Scheduler with list of processes and parameters
    //1st Scheduler Round Robin (RRProcesses, block_duration, tslice);
    RoundRobin(RRProcess, block_duration, tslice);
    //2nd Secheduler Shortest Next Class(SNProcess, block_duration)
    ShortestNext(SNProcess, block_duration);
    filestream.close();    
    return 0;
}
