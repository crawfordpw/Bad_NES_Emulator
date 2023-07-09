//////////////////////////////////////////////////////////////////////////////////////////
//
// Memory.hpp
//
// Contains classes to manage memory.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string.h>
#include "Common.hpp"

//========//
// Memory
//
// Base class that every type of memory should inherit from.
//========//
//
class Memory : public Device
{
    public:

        Memory(AddressType lSize) : mSize(lSize) {}
        virtual ~Memory(void) {}

        virtual DataType Read(AddressType lAddress, DataType lLastRead) = 0;
        virtual void     Write(AddressType lAddress, DataType lData)    = 0;
        AddressType      GetSize(void) {return mSize;}
        
    protected:

        AddressType mSize;

        virtual void Initialize(AddressType lSize) = 0;
};

//========//
// MemoryMapped
//
// Memory directly managed in RAM.
//========//
//
class MemoryMapped : public Memory
{
    public:

        MemoryMapped(AddressType lSize) : Memory(lSize) {Initialize(lSize);}
        virtual ~MemoryMapped(void);

        virtual DataType Read(AddressType lAddress, DataType lLastRead);
        virtual void     Write(AddressType lAddress, DataType lData);

    protected:

        uint8_t * mMemory;

        virtual void Initialize(AddressType lSize);
};

//========//
// ~MemoryMapped
//
// Destructor.
//========//
//
inline MemoryMapped::~MemoryMapped(void)
{
    if (mMemory)
    {
        delete [] mMemory;
    }
}

//--------//
// Initialize
//
// Initializes memory.
//
// param[in]    lSize   Size of memory. 
//--------//
//
inline void MemoryMapped::Initialize(AddressType lSize)
{
    mMemory = new uint8_t[lSize];
    memset(mMemory, 0, lSize);
}

//--------//
// Read
//
// Reads some data out of memory.
//
// param[in] lAddress   Address to read from.
// param[in] lLastRead  If some devices are not connected, this variable
//                      simulates "open bus behavior". Where a read of
//                      a disconnected device results in the last value read.
//                      This shouldn't happen, but if we go outside the address
//                      range, just return this value.
// returns  Data at the given address. 
//--------//
//
inline DataType MemoryMapped::Read(AddressType lAddress, DataType lLastRead)
{
    if (lAddress < mSize)
    {
        return mMemory[lAddress];
    }
    return lLastRead;
}

//--------//
// Write
//
// Writes data to memory at a given address.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
inline void MemoryMapped::Write(AddressType lAddress, DataType lData)
{
    if (lAddress <= mSize)
    {
        mMemory[lAddress] = lData;
    }
}

#endif
