/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vinee
 *
 * Created on January 20, 2018, 4:02 PM
 */

#include <cstdlib>
#include "ProcessTrace.h"
#include <iostream>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    ProcessTrace p(argv[1]);
    p.Execute();
    return 0;
}

