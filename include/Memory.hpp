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
class Memory
{
    public:

        Memory(void)                       : mSize(0) {}
        explicit Memory(AddressType lSize) : mSize(lSize) {}
        virtual ~Memory(void) {}

        virtual DataType Read(AddressType lAddress)                     = 0;
        virtual void     Write(AddressType lAddress, DataType lData)    = 0;
        virtual void     Resize(AddressType lSize)                      = 0;
        virtual int      LoadMemoryFromFile(File * lFile, size_t lSize) = 0;

        AddressType      GetSize(void) {return mSize;}
        
    protected:

        int              LoadMemoryFromFile(File * lFile, size_t lSize, uint8_t * mMemory);

        AddressType mSize;
};

//========//
// MemoryRom
//
// Memory directly managed in RAM.
//========//
//
class MemoryRom : public Memory
{
    public:

        MemoryRom(void) : Memory(), mMemory(nullptr) {}
        explicit MemoryRom(AddressType lSize) : Memory(lSize), mMemory(nullptr) {Resize(lSize);}
        virtual ~MemoryRom(void);

        virtual DataType Read(AddressType lAddress)                     override;
        virtual void     Write(AddressType lAddress, DataType lData)    override;
        virtual void     Resize(AddressType lSize)                      override;
        virtual int      LoadMemoryFromFile(File * lFile, size_t lSize) override;

    protected:

        uint8_t * mMemory;
};

//========//
// ~MemoryRom
//
// Destructor.
//========//
//
inline MemoryRom::~MemoryRom(void)
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
inline void MemoryRom::Resize(AddressType lSize)
{
    // Clean up old memory
    if (mMemory)
    {
        delete [] mMemory;
        mMemory = nullptr;
    }

    // Nothing else to do if size is 0.
    if (lSize == 0)
    {
        return;
    }

    // Create memory at the new size and intialize to all 0's.
    mMemory = new(std::nothrow) uint8_t[lSize];
    if (nullptr == mMemory)
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
// returns  Data at the given address. 
//--------//
//
inline DataType MemoryRom::Read(AddressType lAddress)
{   
    // Don't post error as that should've happened already during construction.
    if (nullptr == mMemory)
    {
        return 0;
    }
    if (lAddress >= mSize)
    {
        gErrorManager.Post(ErrorCodes::INTERNAL_ERROR, "memory index out of range");
        return 0;
    }
    return mMemory[lAddress];
}

//--------//
// Write
//
// Do nothing, this is ROM.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
inline void MemoryRom::Write(AddressType lAddress, DataType lData)
{
    (void)lAddress;
    (void)lData;
    return;
}

//========//
// MemoryRam
//
// Memory directly managed in RAM.
//========//
//
class MemoryRam : public MemoryRom
{
    public:

        MemoryRam(void) : MemoryRom() {}
        explicit MemoryRam(AddressType lSize) : MemoryRom(lSize) {}
        virtual ~MemoryRam(void) = default;

        virtual void Write(AddressType lAddress, DataType lData) override;
};

//--------//
// Write
//
// Writes data to memory at a given address.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
inline void MemoryRam::Write(AddressType lAddress, DataType lData)
{
    // Don't post error as that should've happened already during construction.
    if (nullptr == mMemory)
    {
        return;
    }
    if (lAddress >= mSize)
    {
        gErrorManager.Post(ErrorCodes::INTERNAL_ERROR, "memory index out of range");
        return;
    }
    mMemory[lAddress] = lData;
}

#endif
