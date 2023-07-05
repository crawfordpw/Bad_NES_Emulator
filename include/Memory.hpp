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

        virtual DataType Read(AddressType lAddress) = 0;
        virtual void     Write(AddressType lAddress, DataType lData) = 0;
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

        virtual DataType Read(AddressType lAddress);
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
// returns  Data at the given address. 
//--------//
//
inline DataType MemoryMapped::Read(AddressType lAddress)
{
    if (lAddress < mSize)
    {
        return mMemory[lAddress];
    }
    return 0;
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
