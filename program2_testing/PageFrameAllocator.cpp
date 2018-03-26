/*  PageAllocator - allocate pages in memory
 * 
 * File:   PageFrameAllocator.cpp
 * Author: Vineeth, Tim, Rhys
 * 
 * reated on February 22, 2018, 10:35 PM
 */

#include "PageFrameAllocator.h"

#include <cstring>
#include <sstream>
#include <MMU.h>
using namespace mem;

PageFrameAllocator::PageFrameAllocator(mem::MMU &memory) 
:// memory(page_frame_count * kPageSize),
        
        page_frames_total(memory.get_frame_count()),
        page_frames_free(memory.get_frame_count()),
        free_list_head(0)
{
    memo=&memory;
  // Add all page frames to free list
  for (uint32_t frame = 0; frame < memory.get_frame_count()-1; ++frame) {
    uint32_t next = frame + 1;
    mem::Addr addr_cout= frame * mem::kPageSize;
    //memcpy(&memory[frame*kPageSize], &next, sizeof(uint32_t));
    memory.put_byte(addr_cout,
                 reinterpret_cast<uint8_t*> (&next)); 
  }
  
  // Last page frame has end of list marker
  uint32_t end_list = kEndList;
  mem::Addr addr_cout1= (memory.get_frame_count()-1)* mem::kPageSize;
  //memcpy(&memory[(page_frame_count-1)*kPageSize], &end_list, sizeof(uint32_t));
  memory.put_byte(addr_cout1,
                 reinterpret_cast<uint8_t*> (&end_list));
}

bool PageFrameAllocator::Allocate(uint32_t count, 
                                  std::vector<uint32_t> &page_frames) {
   
  if (count <= page_frames_free) {  // if enough to allocate
    while (count-- > 0) {
      // Return next free frame to caller
      page_frames.push_back(free_list_head);
      Addr alloc_addr=free_list_head* mem::kPageSize;
      // De-link frame from head of free list
      //memcpy(&free_list_head, &memory[free_list_head*kPageSize], sizeof(uint32_t));
      memo->get_byte(reinterpret_cast<uint8_t*> (&free_list_head),alloc_addr);
      
      --page_frames_free;
    }
    return true;
  } else {
    return false;  // do nothing and return error
  }
}

bool PageFrameAllocator::Deallocate(uint32_t count,
                                    std::vector<uint32_t> &page_frames) {
  // If enough to deallocate
  if(count <= page_frames.size()) {
    while(count-- > 0) {
      // Return next frame to head of free list
      uint32_t frame = page_frames.back();
      page_frames.pop_back();
      Addr addr_dealloc=frame * mem::kPageSize;
      //memcpy(&memory[frame * kPageSize], &free_list_head, sizeof(uint32_t));
      memo->put_byte(addr_dealloc,reinterpret_cast<uint8_t*> (&free_list_head));
      free_list_head = frame;
      ++page_frames_free;
    }
    return true;
  } else {
    return false; // do nothing and return error
  }
}

std::string PageFrameAllocator::FreeListToString(void) const {
  std::ostringstream out_string;
  
  uint32_t next_free = free_list_head;
  Addr addr_freelist= next_free * mem::kPageSize;
  
  while (next_free != kEndList) {
    out_string << " " << std::hex << next_free;
    //memcpy(&next_free, &memory[next_free*kPageSize], sizeof(uint32_t));
    memo->get_byte(reinterpret_cast<uint8_t*> (&next_free), addr_freelist);
  }
  
  return out_string.str();
}