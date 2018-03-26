/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ProcessTrace_1.cpp
 * Author: vinee
 * 
 * Created on January 20, 2018, 4:03 PM
 */

#include "ProcessTrace.h"
#include "MMU.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

/*Uses the class constructor to pass in the file_name as a constant value
 * we use a string called line to grab the input file
 * getline call in this function and pushback into our vector of strings "s"
 * text_file is a file stream operator - 
 *     used to open the file, error check and do the getline call
 */
ProcessTrace::ProcessTrace(const std::string &file_name) {

    string line;
    // Open the input file
    text_file.open(file_name);
    if (text_file.fail()) {
        std::cerr << "ERROR: file not found: " << file_name << "\n";
        exit(2);
    }
    if (text_file) {
        while (getline(text_file, line)) {
            s.push_back(line);
        }
    }
}

/*Call to the class destructor to close the input file
 * text_file is the input file
 */
ProcessTrace::~ProcessTrace() {
    text_file.close();
}

/* Call to the class function Execute
 * This class is structured as a series of if statements - similar to a case structure
 * we read in the first word of each line to determine the actions we need to take
 * 
 * data is a vector of strings used as an input/output buffer
 * we use int z as a counting parameter to output each line number
 * in the remainder of the function we create a for loop 
 * create a string parsed to hold a line from the string vector "s"
 * we then nest a do-while loop to continue if we have a line in the string vector "s"
 * we then set up a series of "if" statements that act as cases
 * 
 * in each "if statement" we use the class function htod to convert our string vector "data" to decimal values
 *we used virtual memory to allocate memory.
 * the if statements:
 * allocating memory - used no new variables, it allocates the amount of memory requested in line one of the fstream file
 * put data - which included a vector of ints rdata as a buffer we deleted the location before inserting the data
 * fill data - used variables to hold the starting address, ending address and how many bits to fill in
 * copy data - used variables to hold the starting address, ending address and how many bits to fill in
 * dump data - used variables to hold the starting and ending addresses, then use a for loop to put the data into 16 bit strings
 */
void ProcessTrace::Execute() {
    vector<string> data;
    int z = 1;
    string par;
    stringstream istream1(s[0]);
    while (getline(istream1, par, ' ')) {
        data.push_back(par);
    }
    cout << z << ":" << s[0] << "\n";
    z++;

    //Pulls datafrom 2nd part of array and equates to float f
    float f = htod(data[1]);
    //Pulls constructor MMU and sets ceiling at float f/ 0x1000
    mem::MMU mem(std::ceil(f / 0x1000));
    data.clear();

    for (int i = 1; i < s.size(); i++) {
        string parsed;
        stringstream istream(s[i]);
        while (getline(istream, parsed, ' ')) {
            data.push_back(parsed);
        }
        if (data[0] == "put") {
            cout << z << ":" << s[z - 1] << "\n";
            z++;
            int d1 = htod(data[1]);
            vector<uint8_t> rdata;
            int a = 0;
            mem::Addr vaddr = d1;
            for (int i = 0; i < data.size(); i++) {
                uint8_t j = htod(data[i]);
                rdata.push_back(j);
            }
            uint8_t dat[rdata.size() - 2], dat1[rdata.size() - 2];
            for (int i = 0; i < rdata.size() - 2; i++) {
                dat[i] = rdata.at(rdata.size() - 1 - i);
            }
            int m = sizeof (dat) / sizeof (dat[0]);
            for (int i = 0; i < m; i++) {
                dat1[i] = dat[m - 1 - i];
            }
            /**
             * put_bytes - copy a range of bytes into physical memory
             * 
             * @param vaddr virtual address destination
             * @param sizeof equivalent count number of bytes to copy
             * @param dat1 equivalent src source buffer
             */
            mem.put_bytes(vaddr, sizeof (dat1), dat1);

        }
        if (data[0] == "fill") {
            cout << z << ":" << s[z - 1] << "\n";
            z++;
            int fdata = htod(data[1]);
            uint8_t fcount = htod(data[2]);
            uint8_t fvalue = htod(data[3]);
            mem::Addr vaddr = fdata;
            uint8_t arrfdata[fcount];
            for (uint8_t i = 0; i < fcount; i++) {
                arrfdata[i] = fvalue;
            }
            mem.put_bytes(vaddr, sizeof (arrfdata), arrfdata);

        }
        if (data[0] == "compare") {
            cout << z << ":" << s[z - 1] << "\n";
            z++;
            int caddr = htod(data[1]);
            int caddr1 = htod(data[1]);
            uint8_t comparr[data.size() - 2];
            mem::Addr vaddr = caddr;
            int comp = 0;
            for (int i = 2; i < data.size(); i++) {
                /**
                 * get_byte - get a single byte from the specified virtual address
                 * 
                 * @param comparr - destination byte
                 * @param vaddr - address of data in memory
                 */
                mem.get_byte(&comparr[comp], vaddr);
                caddr += 1;
                vaddr = caddr;
                comp++;
            }
            comp = 0;
            for (int i = 2; i < data.size(); i++) {
                uint8_t cvalue = htod(data[i]);
                uint8_t cvalue3 = comparr[comp];

                if (to_string(cvalue).compare(to_string(cvalue3)) != 0) {
                    cout << "compare error at address " << hex << (unsigned) caddr1 << ", expected " << (unsigned) cvalue << ", actual is " << (unsigned) cvalue3 << "\n";
                    caddr1++;
                }
                comp++;

            }
        }
        if (data[0] == "copy") {
            cout << z << ":" << s[z - 1] << "\n";
            z++;
            int daddr = htod(data[1]);
            int saddr = htod(data[2]);
            uint8_t ccount = htod(data[3]);
            uint8_t cpyarr[ccount];
            mem::Addr svaddr = saddr;
            mem::Addr dvaddr = daddr;
            for (uint8_t i = 0; i < ccount; i++) {
                /**
                 * get_byte - get a single byte from the specified virtual address
                 * 
                 * @param cpyarr - destination byte
                 * @param svaddr - address of data in memory
                 */
                mem.get_byte(&cpyarr[i], svaddr);
                saddr += 1;
                svaddr = saddr;
            }

            /**
             * put_bytes - copy a range of bytes into physical memory
             * 
             * @param dvaddr virtual address destination
             * @param sizeof equivalent to count number of bytes to copy
             * @param cpyarr equivalent to src source buffer
             */
            mem.put_bytes(dvaddr, sizeof (cpyarr), cpyarr);
        }
        if (data[0] == "dump") {
            cout << dec << z << ":" << s[z - 1] << "\n";
            z++;
            int duaddr = htod(data[1]);
            int dcount = htod(data[2]);
            uint8_t dmparr[dcount];
            mem::Addr duvaddr = duaddr;
            cout << hex << data[1] << "\n";
            int lcount = 0, lcount1 = 0;
            for (int i = 0; i < dcount; i++) {

                /**
                 * get_byte - get a single byte from the specified virtual address
                 * 
                 * @param dmparr - destination byte
                 * @param duvaddr - address of data in memory
                 */
                mem.get_byte(&dmparr[i], duvaddr);
                duaddr += 1;
                duvaddr = duaddr;
            }
            for (int i = 0; i<sizeof (dmparr) / sizeof (dmparr[0]); i++) {
                lcount1++;
                if (dmparr[i] < 16)
                    cout << "0" << (unsigned) dmparr[i] << " ";

                else
                    cout << (unsigned) dmparr[i] << " ";


                if (lcount1 == 16) {
                    cout << "\n";
                    lcount1 = 0;
                }
            }
        }
        data.clear();

    }
}

/* Function call to class function htod.
 * This function converts hex values to decimal values.
 * int a is just a buffer to place the value into
 * s1 is the string value passed into this function from the execute function
 * ss1 is an internal stringstream object for us to do use std::hex to convert to dec
 * we return "a" back to the execute function
 */
int ProcessTrace::htod(string s1) {
    int a;
    stringstream ss1;
    ss1 << s1;
    ss1 >> std::hex >> a;
    return a;
}
