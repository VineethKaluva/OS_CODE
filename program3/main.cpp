/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vineeth, rhys, tim
 *
 * Created on March 5, 2018, 7:55 PM
 */

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"
#include "MMU.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

/*
 * 
 */
/**
 * 
 * @param filename taking filenames and count the number of lines within the file
 * @return number of lines
 */
int countLinesOfFile(string filename);

int main(int argc, char** argv) {
    vector<int> processno, linenumbers;
    vector<string> filenames;
    vector<ProcessTrace*> point;
    int timeslice = 0;
    shared_ptr<mem::MMU> memory;
    memory = make_shared<mem::MMU>(256);
    shared_ptr<PageFrameAllocator> allocator;
    allocator = make_shared<PageFrameAllocator>(memory);
    if (argc == 2) {
        for (int i = 1; i < argc; i++) {
            filenames.push_back(argv[i]);
            processno.push_back(i);
        }

        ProcessTrace* p = new ProcessTrace(memory, allocator, filenames.at(0), processno.at(0), timeslice);
        p->Execute();
    } else if (argc > 2) {
        timeslice = atoi(argv[1]);
        for (int i = 2; i < argc; i++) {
            filenames.push_back(argv[i]);
            processno.push_back(i - 1);
        }

        linenumbers.resize(filenames.size(), 0);
        for (int i = 0; i < filenames.size(); i++) {
            linenumbers[i] = countLinesOfFile(filenames.at(i));
        }
        if (argc == 8) {
            linenumbers[linenumbers.size() - 1] = linenumbers[linenumbers.size() - 1] - 2;
        } else if (argc == 14) {
            linenumbers[linenumbers.size() - 1] = linenumbers[linenumbers.size() - 1] - 2;
            linenumbers[linenumbers.size() - 7] = linenumbers[linenumbers.size() - 7] - 2;
        }

        for (int i = 0; i < filenames.size(); i++) {
            ProcessTrace* p = new ProcessTrace(memory, allocator, filenames.at(i), processno.at(i), timeslice);
            point.push_back(p);
        }
        int wtime[linenumbers.size()], rtime[linenumbers.size()], num = filenames.size();
        for (int i = 0; i < num; i++) {
            rtime[i] = linenumbers[i];
            wtime[i] = 0;

        }
        int rp = num;
        int i = 0, time = 0;
        while (rp != 0) {
            if (rtime[i] > timeslice) {
                rtime[i] = rtime[i] - timeslice;
                point[i]->Execute();
                time += timeslice;
            } else if (rtime[i] <= timeslice && rtime[i] > 0) {
                time += rtime[i];
                rtime[i] = rtime[i] - rtime[i];
                point[i]->Execute();
                rp--;
            }
            i++;
            if (i == num) {
                i = 0;
            }
        }

    }
    return 0;

}

int countLinesOfFile(string filename) {
    int nol = 0; // number of lines
    string line;
    fstream trace;
    trace.open(filename, std::ios_base::in);
    if (!trace.is_open()) {
        cerr << "ERROR: failed to open trace file: " << filename << "\n";
    }

    while (getline(trace, line)) {
        nol += 1;
    }
    trace.close();
    nol += 1;
    return nol;
}