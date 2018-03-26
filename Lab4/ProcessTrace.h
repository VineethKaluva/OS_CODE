/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace.h
 * Author: vinee
 *
 * Created on January 20, 2018, 4:03 PM
 */

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/* 
 * .h file includes the rule of 5
 */

class ProcessTrace {
public:

/*
 * Set constructor
 */
    ProcessTrace(const std::string &file_name);
    
/*
 * Set deconstuctor
 */
    virtual ~ProcessTrace();
    
/*
 * the unused copy and move functions are set to delete
 */
    ProcessTrace(const ProcessTrace &orig) = delete;
    ProcessTrace(ProcessTrace &&orig) = delete;
    ProcessTrace operator=(const ProcessTrace &orig) = delete;
    ProcessTrace operator=(ProcessTrace &&orig) = delete;
   
/*
 * we create a void function called execute
 */
    void Execute();
    
/*
 * we create a function that will return an integer called htod - hex to decimal conversion
 */
    int htod(string s1);
    
/* private variables
 * file stream variable called text_file
 * vector of strings called "s"
 */
private:
    fstream text_file;
    vector<string> s;

};

#endif /* PROCESSTRACE_H */
