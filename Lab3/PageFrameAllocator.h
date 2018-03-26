/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageFrameAllocator.h
 * Author: vineeth
 *
 * Created on January 29, 2018, 11:56 AM
 */

#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class PageFrameAllocator {
public:
    /**
 * PageFrameAllocator Constructor   - allocates data from file to link list
 * @param nspage    returns string in length of 1000.  This will be used for the 
 *                  setting number of default blocks with size htod(s)  
 * @return      page_frames_free for the exact size for default 
 *              of all free page frames of 0XFFFFFF if all pages used  
 */

    PageFrameAllocator(string nspage);
    virtual ~PageFrameAllocator();
    /**
     * PageFrameAllocator = deletes allocator
     * @param orig  - returns the reference to the original 
     *                page frame allocator to deleted
     */

    PageFrameAllocator(const PageFrameAllocator &orig) = delete;
    /**
     * PageFrameAllocator = deletes allocator
     * @param orig  - returns the reference to the reference original 
     *                page frame allocator to deleted
     */

    PageFrameAllocator(PageFrameAllocator &&orig) = delete;
    /**
     * PageFrameAllocator = deletes allocator
     * @param orig  - returns the reference to the original 
     *                page frame allocator to deleted
     */

    PageFrameAllocator operator=(const PageFrameAllocator &orig) = delete;
    /**
     * PageFrameAllocator = deletes allocator
     * @param orig  - returns the reference to the reference original 
     *                page frame allocator to deleted
     */

    PageFrameAllocator operator=(PageFrameAllocator &&orig) = delete;
    /**
     * PageFrameAllocator::Allocate Command - allocate to link list
     * @param count         - returns the number of blocks used in allocate
     * @param page_frames   - returns number of page frames used
     * @return              - false if too many page frames popped off and reset 
     *                        pages, true 
     */

    bool Allocate(uint32_t count, vector<uint32_t> &pageframes);
    /**
     * PageFrameAllocator - Constructor for deallocating from link list
     * @param count       - Unsigned 32 bits usead to track number of blocks used
     * @param page_frames - Vector that stores values from memory after each 
     *                      deallocation
     * @return false if count and free page frames is less than 0,
     *         true if count and free pages is greater than 0 and adds count 
     *         to page_frames_free
     */

    bool Deallocate(uint32_t count, vector<uint32_t> &pageframes);
    uint32_t get_page_frames_free() const;
    /**
    * htod command -  
    * @param s returns string reporting the blocks u  
    */

    int htod(string s);
    void print_free_list(vector<uint32_t> &pageframes);
    
private:
    vector<uint32_t> memory;
    uint32_t page_frames_total;
    uint32_t page_frames_free;
    uint32_t free_list_head;
    uint32_t blocksize= 0x1000;
    int ctrackalloc=0;
};

#endif /* PAGEFRAMEALLOCATOR_H */

