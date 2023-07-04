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

        DataType Read(AddressType lAddress);
        void     Write(AddressType lAddress, DataType lData);

    public:

        enum
        {
            RAM_SIZE = 2048
        };
    
        Cpu6502      mCpu;
        MemoryMapped mRam;
};

#endif
