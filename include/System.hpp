//////////////////////////////////////////////////////////////////////////////////////////
//
// System.hpp
//
// File for the entire system.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Memory.hpp"
#include "Cpu6502.hpp"

//========//
// System
//
// Contains the main components for the system.
//========//
//
class System
{
    public:

        System(void);
        ~System(void);

        void     Start(void);
        DataType Read(AddressType lAddress);
        void     Write(AddressType lAddress, DataType lData);
        void     LoadMemory(char * lProgram, AddressType lSize, AddressType lOffset);
        void     DumpMemoryAsHex(const char * lFilename);
        void     DumpMemoryAsRaw(const char * lFilename);

    public:

        enum
        {
            //RAM_SIZE = 2048
            RAM_SIZE = 1024 * 64 - 1    // Temporary for testing until ROM is in.
        };
    
        Cpu6502      mCpu;
        MemoryMapped mRam;
};

#endif
