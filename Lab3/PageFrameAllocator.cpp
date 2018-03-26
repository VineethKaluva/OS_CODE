/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageFrameAllocator.cpp
 * Author: vineeth, Tim, Rhys
 * Group 10
 * Created on January 29, 2018, 11:56 AM
 */

#include "PageFrameAllocator.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <sstream>
using namespace std;
/**
 * PageFrameAllocator Constructor   - allocates data from file to link list
 * @param nspage    returns string in length of 1000.  This will be used for the 
 *                  setting number of default blocks with size htod(s)  
 * @return      page_frames_free for the exact size for default 
 *              of all free page frames of 0XFFFFFF if all pages used  
 */

PageFrameAllocator::PageFrameAllocator(string nspage) {
    int npage=htod(nspage);
    memory.resize(npage*blocksize,0);
    uint32_t v32 , index=blocksize;
    page_frames_total = memory.size()/ 0x1000;
    for(uint32_t i=0;i<npage;i++)
    {
    if(i== npage-1)
    {
        v32=0xFFFFFFFF;
        memcpy(&memory[index*i], &v32, sizeof(uint32_t));
    }
    else
    {
        v32=i+1;
        memcpy(&memory[index*i], &v32, sizeof(uint32_t));
    }
    }
    page_frames_free= npage;
    free_list_head=memory[0];
}

/**
 * PageFrameAllocator::Allocate Command - allocate to link list
 * @param count         - returns the number of blocks used in allocate
 * @param page_frames   - returns number of page frames used
 * @return              - false if too many page frames popped off and reset 
 *                        pages, true 
 */

bool PageFrameAllocator::Allocate(uint32_t count, vector<uint32_t>& pageframes)
{
    if(count > page_frames_free)
    {
        return false;
    }
    else
    {
        for(int i=ctrackalloc; i<ctrackalloc+count;i++)
        {
            pageframes.push_back(memory[i*blocksize]);           
        }
        free_list_head=memory[count*blocksize];
        page_frames_free=page_frames_free-count;
        ctrackalloc+=count;
        return true;
    }
}

/**
 * PageFrameAllocator - Constructor for deallocating from link list
 * @param count       - Unsigned 32 bits usead to track number of blocks used
 * @param page_frames - Vector that stores values from memory after each 
 *                      deallocation
 * @return false if count and free page frames is less than 0,
 *         true if count and free pages is greater than 0 and adds count 
 *         to page_frames_free
 */

bool PageFrameAllocator::Deallocate(uint32_t count, vector<uint32_t>& pageframes)
{
    if(count > page_frames_total)
    {
        return false;
    }
    else if((page_frames_total == page_frames_free))
    {
        return false;
    }
    else
    {
        for(int i=count;i>0;i--)
        {
          pageframes.pop_back();
          free_list_head=memory[(i-1)*blocksize];
        }
        page_frames_free=page_frames_free+count;
        ctrackalloc-=count;
        return true;
        
    }
}

/**
 * PageFramesFree
 * @return the number of free page frames after each allocation
 */


uint32_t PageFrameAllocator::get_page_frames_free() const
{
    return page_frames_free;
}

/* Function call to class function htod.
 * This function converts hex values to decimal values.
 * int a is just a buffer to place the value into
 * s1 is the string value passed into this function from the execute function
 * ss1 is an internal stringstream object for us to do use std::hex to convert to dec
 * we return "a" back to the execute function
*/
int PageFrameAllocator::htod(string s1)
{
    int a;
    stringstream ss1;
    ss1<< s1;
    ss1>> std::hex >> a;
    return a;
}

/**
 * PageFrameAllocator::Prin_free_list Command - prints free page frames
 * @param vector        - returns the number of blocks used in allocate
 * @param page_frames   - returns number of page frames used
 * @return              - prints the number of free pages or 0XFFFFFFF if page 
 *                        are all used.
 */

void PageFrameAllocator::print_free_list(vector<uint32_t> &pageframes)
{
    int count=0;
    if(page_frames_free==page_frames_total)
    {
        for(int i=0;i<page_frames_total;i++)
        {
            
            if(memory[blocksize*i]==0xFFFFFFFF)
                cout<<"  "<<hex<<page_frames_total-1;
            else
            {
                count=memory[blocksize*i];
              cout<<"  "<<hex<<count-1<<" "; 
            }
            
        }
       
        cout<<"\n";
    }
    else if(page_frames_free>0)
    {
        for(int i=free_list_head-1;i<free_list_head-1+page_frames_free;i++)
        {
            if(memory[blocksize*i]==0xFFFFFFFF)
                cout<<"  "<<hex<<page_frames_total-1;
            else
            {
                count=memory[blocksize*i];
              cout<<"  "<<hex<<count-1<<" "; 
            }
        }
        cout<<"\n";
    }
    else if(page_frames_free==0)
    {
        cout<<"  "<<"\n";
    }
    
}

/**
 * ~PageFrameAllocator  - Deconstructor for PageFrameAllocator
 */

PageFrameAllocator::~PageFrameAllocator(){
    
}