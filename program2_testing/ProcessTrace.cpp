/*
 * ProcessTrace implementation 
 */

/* 
 * File:   ProcessTrace.cpp
 * Author: Vineeth, Tim, Rhys
 * 
 * reated on February 22, 2018, 10:35 PM
 */

#include "ProcessTrace.h"
#include "MMU.h"
#include "MemoryDefs.h"
#include "PageTable.h"
#include "PageFrameAllocator.h"
#include "PageTable.h"
#include "MemoryDefs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace mem;

ProcessTrace::ProcessTrace(string file_name_, MMU &memory, PageFrameAllocator &pfa)
: file_name(file_name_), line_number(0) {
    memo = &memory;
    pf = &pfa;
    // Open the trace file.  Abort program if can't open.
    trace.open(file_name, std::ios_base::in);
    if (!trace.is_open()) {
        cerr << "ERROR: failed to open trace file: " << file_name << "\n";
        exit(2);
    }
}

ProcessTrace::~ProcessTrace() {
    trace.close();
}

void ProcessTrace::Execute(void) {
    // Read and process commands
    string line; // text line read
    string cmd; // command from line
    vector<string> cmdArgs; // arguments from line
    // Select the command to execute
    while (ParseCommand(line, cmd, cmdArgs)) {
        if (cmd == "alloc") {
            CmdAlloc(line, cmd, cmdArgs); // allocate memory
        } else if (cmd == "compare") {
            CmdCompare(line, cmd, cmdArgs); // get and compare multiple bytes
        } else if (cmd == "put") {
            CmdPut(line, cmd, cmdArgs); // put bytes
        } else if (cmd == "fill") {
            CmdFill(line, cmd, cmdArgs); // fill bytes with value
        } else if (cmd == "copy") {
            CmdCopy(line, cmd, cmdArgs); // copy bytes to dest from source
        } else if (cmd == "dump") {
            CmdDump(line, cmd, cmdArgs); // dump byte values to output
        } else if (cmd == "#") {
            CmdComments(line, cmd, cmdArgs); // comments
        } else if (cmd == "writable") {
            CmdWritable(line, cmd, cmdArgs); // writing data
        }else {
            cerr << "ERROR: invalid command at line " << line_number << ":\n"
                    << line << "\n";
            exit(2);
        }
    }
}

bool ProcessTrace::ParseCommand(
        string &line, string &cmd, vector<string> &cmdArgs) {
    cmdArgs.clear();
    line.clear();

    // Read next line
    if (std::getline(trace, line)) {
        ++line_number;
        cout << std::dec << line_number << ":" << line << "\n";

        // Make a string stream from command line
        istringstream lineStream(line);

        // Get command
        lineStream >> cmd;

        // Get arguments
        string arg;
        while (lineStream >> arg) {
            cmdArgs.push_back(arg);
        }
        return true;
    } else if (trace.eof()) {
        return false;
    } else {
        cerr << "ERROR: getline failed on trace file: " << file_name
                << "at line " << line_number << "\n";
        exit(2);
    }
}
/**
 * Cmd Alloc - Transfer from physical to virtual
 * @param line -takes each line 
 * @param cmd  -take each and every parameter
 * @param cmdArgs -this is a vector of strings
 */
void ProcessTrace::CmdAlloc(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    memo->get_PMCB(p_pmcb);
    p_pmcb.vm_enable = false;
    memo->set_PMCB(p_pmcb);

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }
    if(once==1)
    {
    pf->Allocate(1, allocated);
    kPageTableBase = allocated.back() * mem::kPageSize;
   once++;   
    }
    pf->Allocate(1, allocated);
    kPageTableL2 = allocated.back() * mem::kPageSize;
    uint32_t check = unsigned_cmdArgs.at(0);
    int size_of_data = unsigned_cmdArgs.at(1) / mem::kPageSize;
    for (int i = 0; i < size_of_data; i++) {
        pf->Allocate(1, allocated);
        multi_page.push_back(allocated.back()* mem::kPageSize);
    }
    kVAddrStart = check;
    Addr update_kVAddrStart=kVAddrStart;
    Addr update_kVAddrStart1=kVAddrStart;
    for(int i=0;i<multi_page.size();i++)
    {
    Addr l1_offset = (update_kVAddrStart >> (kPageSizeBits + kPageTableSizeBits)) & kPageTableIndexMask;
    page_table_l1[l1_offset] = kPageTableL2 | kPTE_PresentMask | kPTE_WritableMask;
    memo->put_bytes(kPageTableBase, kPageTableSizeBytes,
            reinterpret_cast<uint8_t*> (&page_table_l1)); // write L1 page table
    Addr l2_offset = (update_kVAddrStart >> kPageSizeBits) & kPageTableIndexMask;
    page_table_l2[l2_offset] = multi_page[i] | kPTE_PresentMask | kPTE_WritableMask;
    memo->put_bytes(kPageTableL2, kPageTableSizeBytes,
            reinterpret_cast<uint8_t*> (&page_table_l2)); // write L2 page table
    update_kVAddrStart+=0x1000;
    }
    PMCB v_pmcb(true, kPageTableBase); // load to start virtual mode
    memo->set_PMCB(v_pmcb);

    for(int i=0;i<multi_page.size();i++)
    {
    uint8_t data = 0;
    for (int i = 0; i < mem::kPageSize; i++) {
        memo->put_byte(update_kVAddrStart1 + i, &data);
    }
    update_kVAddrStart1+=0x1000;
    }
    
    unsigned_cmdArgs.clear();
    multi_page.clear();

}

void ProcessTrace::CmdCompare(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }

    uint32_t addr = unsigned_cmdArgs.at(0);
    uint32_t check_new_addr=addr;
    // Compare specified byte values
    size_t num_bytes = unsigned_cmdArgs.size() - 1;
    uint8_t buffer[num_bytes];
    try{   
        for(int k=0;k<num_bytes;k++)
        {
             memo->get_byte(&buffer[k], check_new_addr);
             check_new_addr++;
        }
    for (int i = 1; i < unsigned_cmdArgs.size(); ++i) {
        if (buffer[i - 1] != unsigned_cmdArgs.at(i)) {
            cout << "compare error at address " << std::hex << addr
                    << ", expected " << static_cast<uint32_t> (unsigned_cmdArgs.at(i))
                    << ", actual is " << static_cast<uint32_t> (buffer[i - 1]) << "\n";
        }
        ++addr;
    }
    } catch(PageFaultException e)
    {
        cout<< "Exception type PageFaultException occurre at input line "<<
                line_number<< " at virtual address 0x"<<hex<< setw(8) << setfill('0')<<(check_new_addr)<< ":" << endl;
        mem::PMCB::NONE;
        
    } catch(WritePermissionFaultException e)
    {
        cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<<(addr)<< ":" << endl;
        mem::PMCB::NONE;
    }
    
    unsigned_cmdArgs.clear();
}

void ProcessTrace::CmdPut(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }
    // Put multiple bytes starting at specified address
    uint32_t addr = unsigned_cmdArgs.at(0);
    uint32_t check_new_addr=addr;
    size_t num_bytes = unsigned_cmdArgs.size() - 1;
    uint8_t buffer[num_bytes];
    for (int i = 1; i < unsigned_cmdArgs.size(); ++i) {
        buffer[i - 1] = unsigned_cmdArgs.at(i);
    }
    try{
        for(int k=0;k<num_bytes;k++)
        {
            memo->put_byte(check_new_addr, &buffer[k]);
            check_new_addr++;
        }
    }catch(PageFaultException e)
    {
        cout<< "Exception type PageFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<< setw(8)<<(check_new_addr)<< ":" << endl;
        mem::PMCB::NONE;
    } catch(WritePermissionFaultException e)
    {
        cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<<(addr)<< ":" << endl;
        mem::PMCB::NONE;
    }
    
    unsigned_cmdArgs.clear();
}

void ProcessTrace::CmdCopy(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }

    // Copy specified number of bytes to destination from source
    Addr dst = unsigned_cmdArgs.at(0);
    Addr src = unsigned_cmdArgs.at(1);
    uint32_t check_new_addr=unsigned_cmdArgs.at(0);
    uint32_t check_new_addr1=unsigned_cmdArgs.at(1);
    Addr num_bytes = unsigned_cmdArgs.at(2);
    uint8_t buffer[num_bytes];
    try{
        for(int k=0;k<num_bytes;k++)
        {
            memo->get_byte(&buffer[k], check_new_addr1);
            check_new_addr1++;
        }
    //memo->put_bytes(dst, num_bytes, buffer);
    }catch(PageFaultException e)
    {
        cout<< "Exception type PageFaultException on read occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<< setw(8)<<(check_new_addr1)<< ":" << endl;
        mem::PMCB::NONE;
    } catch(WritePermissionFaultException e)
    {
        cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<<(src)<< ":" << endl;
    }
    try{
    //memo->get_bytes(buffer, src, num_bytes);
        for(int k=0;k<num_bytes;k++)
        {
            memo->put_byte(check_new_addr, &buffer[k]);
            check_new_addr++;
        }
    }catch(PageFaultException e)
    {
        cout<< "Exception type PageFaultException on write occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<< setw(8) <<(check_new_addr)<< ":" <<endl;
        mem::PMCB::NONE;
    } catch(WritePermissionFaultException e)
    {
       cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<<(src)<< ":" << endl;
        mem::PMCB::NONE;
    }
    unsigned_cmdArgs.clear();
}

void ProcessTrace::CmdFill(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }
    // Fill a sequence of bytes with the specified value
    Addr addr = unsigned_cmdArgs.at(0);
    Addr num_bytes = unsigned_cmdArgs.at(1);
    uint8_t val = unsigned_cmdArgs.at(2);
    try{
    for (int i = 0; i < num_bytes; ++i) {
        memo->put_byte(addr++, &val);
    }
    }catch(PageFaultException e)
    {
        cout<< "Exception type PageFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<< setw(8) << (addr-1)<< ":" <<endl;
        mem::PMCB::NONE;
    } catch(WritePermissionFaultException e)
    {
        cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<cmdArgs.at(0)<< ":" << endl;
        mem::PMCB::NONE;
    }
    unsigned_cmdArgs.clear();
}

void ProcessTrace::CmdDump(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs) {

    for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    }


    uint32_t addr = unsigned_cmdArgs.at(0);
    uint32_t count = unsigned_cmdArgs.at(1);

    // Output the address
    cout << std::hex << addr;

    // Output the specified number of bytes starting at the address
    try{
    for (int i = 0; i < count; ++i) {
        if ((i % 16) == 0) { // line break every 16 bytes
            cout << "\n";
        }
        uint8_t byte_val;
        memo->get_byte(&byte_val, addr++);
        cout << " " << std::setfill('0') << setw(2)
                << static_cast<uint32_t> (byte_val);
    }
    cout << "\n";
    }catch(PageFaultException e)
    {
        cout<< "\nException type PageFaultException occurred at input line "<<
                dec<<line_number<< " at virtual address 0x"<<hex<<setw(8)<<setfill('0')<<(addr-1)<< ":" <<endl;
        mem::PMCB::NONE;
    } catch(WritePermissionFaultException e)
    {
        cout<< "Exception type WritePermissionFaultException occurred at input line "<<
                line_number<< " at virtual address 0x"<<hex<<(addr)<< ":" << endl;
        mem::PMCB::NONE;
    }
    unsigned_cmdArgs.clear();
}

void ProcessTrace::CmdComments(const string &line,
        const string &cmd,
        const vector<string> &cmdArgs)
{
    return;
}

void ProcessTrace::CmdWritable(const std::string &line, 
               const std::string &cmd, 
               const std::vector<string> &cmdArgs)
{
    memo->get_PMCB(p_pmcb);
    p_pmcb.vm_enable = false;
    memo->set_PMCB(p_pmcb);
   for (int i = 0; i < cmdArgs.size(); i++) {
        int j = stoh(cmdArgs[i]);
        unsigned_cmdArgs.push_back(j);
    } 
    uint32_t check=unsigned_cmdArgs.at(0);
    int pos=unsigned_cmdArgs.at(1);
    int status=unsigned_cmdArgs.at(2);
    kVAddrStart=check;
    if(status==0)
    {
        Addr l2_offset = (kVAddrStart >> kPageSizeBits) & kPageTableIndexMask;
    page_table_l2[l2_offset] =  kPTE_PresentMask;
    memo->put_bytes(kPageTableL2, kPageTableSizeBytes,
            reinterpret_cast<uint8_t*> (&page_table_l2));
    }
    else if(status==1)
    {
        Addr l2_offset = (kVAddrStart >> kPageSizeBits) & kPageTableIndexMask;
        page_table_l2[l2_offset] = kPTE_PresentMask | kPTE_WritableMask;
        memo->put_bytes(kPageTableL2, kPageTableSizeBytes,
            reinterpret_cast<uint8_t*> (&page_table_l2));
    }
    
    PMCB vm_pmcb(true, kPageTableBase); // load to start virtual mode
    memo->set_PMCB(vm_pmcb);
   unsigned_cmdArgs.clear();
}

int ProcessTrace::stoh(string s1) {
    uint32_t a;
    stringstream ss1;
    ss1 << s1;
    ss1 >> std::hex >> a;
    return a;
}