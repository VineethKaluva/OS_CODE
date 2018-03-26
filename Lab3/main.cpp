/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vineeth
 *
 * Created on January 29, 2018, 11:55 AM
 */
#include "PageFrameAllocator.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    std::string line,count;
    std::vector<std::string> s,data;
    vector<uint32_t> page_frames;
    std::fstream file;
    file.open(argv[1]);
    while(getline(file, line))
    {
        s.push_back(line);
    }
    /**
     * Calling PageFrameAllocator
     * @parm s vector element is set to 0 
     */
    PageFrameAllocator pfa(s[0]);
    cout<<" >"<<s[0]<<"\n";
     for(int i=1;i<s.size();i++)
    {
         cout<<" >"<<s[i]<<"\n";
    string parsed;
    stringstream istream(s[i]); 
    /**
     * pulls full line and streams with space 
     */

   while(getline(istream,parsed,' '))
   {
    data.push_back(parsed);
   }
    for(int i=0;i<data.size();i++)
    {
   if(data[0]=="1")
   { 
       /**
        * pulls htod function and sets data to one 
        */

       int d=pfa.htod(data[1]);
       /**
        * pulls allocate function and checks b is 0 (false) and prints the current 
        * page frame or 1 (true) and prints the deducted free page frames  
        */

       bool b=pfa.Allocate(d, page_frames);
       if(b)
       {
           cout<<"  T "<<hex<<pfa.get_page_frames_free()<<"\n";
       }
       else
       {
           cout<<"  F "<<hex<<pfa.get_page_frames_free()<<"\n";
       }
       
       break;
   }
    else if(data[0]=="0")
   {
       int d1=pfa.htod(data[1]);
        /**
        * pulls dealocate function and checks b1 is 1 (false) and prints the current 
        * page frame or01 (true) and prints the increased free page frames  
        */

       bool b1=pfa.Deallocate(d1, page_frames);
       if(b1)
       {
           cout<<"  T "<<hex<<pfa.get_page_frames_free()<<"\n";
       }
       else
       {
           cout<<"  F "<<hex<<pfa.get_page_frames_free()<<"\n";
       }
       
       break;
   }
    else if(data[0]=="2")
   {
       pfa.print_free_list(page_frames);
       break;
   }
    }
   
   data.clear();
   
}

    return 0;
}
