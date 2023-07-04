/////////////////////////////////////////////////////////////////////
//
// System.cpp
//
// Implementation file for the system.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>

//--------//
//
// System
//
//--------//

//--------//
// System
//
// Constructor.
//--------//
//
System::System()
  : mRam(RAM_SIZE)
{
    mCpu.Connect(this);
    mRam.Connect(this);
    mCpu.Reset();
}

//--------//
// ~System
//
// Destructor.
//--------//
//
System::~System()
{
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
DataType System::Read(AddressType lAddress)
{
    return mRam.Read(lAddress);
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
void System::Write(AddressType lAddress, DataType lData)
{
    mRam.Write(lAddress, lData);
}

//--------//
// LoadMemory
//
// Loads RAM with a program, starting at the very beginning. This will
// make an exact copy of the data.
// This is likely a temporary function for testing the CPU,
// or at the very least will be heavily modified as more pieces are added.
//
// param[in] lProgram   The program to load, an array of uint8_t.
// param[in] lSize      Size of the array.
//--------//
//
void System::LoadMemory(DataType lProgram[], AddressType lSize)
{
    for (AddressType lIndex = 0; lIndex < lSize; ++lIndex)
    {
        Write(lIndex, lProgram[lIndex]);
    }
}
