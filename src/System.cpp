/////////////////////////////////////////////////////////////////////
//
// System.hpp
//
// Implementation file for the system.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>

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

// TODO  - Remove
void System::Test()
{
    mCpu.Test();
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
