/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Vineeth, Tim, Rhys
 *
 * Created on February 22, 2018, 10:35 PM
 */

#include "PageFrameAllocator.h"
#include "ProcessTrace.h"
#include "MMU.h"
#include <cstdlib>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    mem::MMU memory(256);
    PageFrameAllocator allocator(memory);
    ProcessTrace proc(argv[1],memory,allocator);
    proc.Execute();

    return 0;
}



