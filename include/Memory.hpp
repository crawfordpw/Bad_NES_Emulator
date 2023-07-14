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
#include <File/ApiFile.hpp>
#include <Errors/ApiErrors.hpp>
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

        Memory(void)              : mSize(0) {}
        Memory(AddressType lSize) : mSize(lSize) {}
        virtual ~Memory(void) {}

        virtual DataType Read(AddressType lAddress, DataType lLastRead) = 0;
        virtual void     Write(AddressType lAddress, DataType lData)    = 0;
        virtual void     Resize(AddressType lSize)                      = 0;
        virtual int      LoadMemoryFromFile(File * lFile, size_t lSize) = 0;

        AddressType      GetSize(void) {return mSize;}
        
    protected:

        AddressType mSize;
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

        MemoryMapped(void) : Memory(), mMemory(NULL) {}
        MemoryMapped(AddressType lSize) : Memory(lSize), mMemory(NULL) {Resize(lSize);}
        virtual ~MemoryMapped(void);

        virtual DataType Read(AddressType lAddress, DataType lLastRead) override;
        virtual void     Write(AddressType lAddress, DataType lData)    override;
        virtual void     Resize(AddressType lSize)                      override;
        virtual int      LoadMemoryFromFile(File * lFile, size_t lSize) override;

    protected:

        uint8_t * mMemory;
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
// Resize
//
// Resizes allocated memory, detroying contents in process.
//
// param[in]    lSize   Size of memory. 
//--------//
//
inline void MemoryMapped::Resize(AddressType lSize)
{
    // Clean up old memory
    if (mMemory)
    {
        delete [] mMemory;
        mMemory = NULL;
    }

    // Nothing else to do if size is 0.
    if (lSize == 0)
    {
        return;
    }

    // Create memory at the new size and intialize to all 0's.
    mMemory = new(std::nothrow) uint8_t[lSize];
    if (mMemory == NULL)
    {
        gErrorManager.Post(ErrorCodes::OUT_OF_MEMORY);
        return;
    }

    mSize = lSize;
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
    if (lAddress < mSize && mMemory != NULL)
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
    if (lAddress < mSize && mMemory != NULL)
    {
        mMemory[lAddress] = lData;
    }
}

#endif
