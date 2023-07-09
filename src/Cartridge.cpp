/////////////////////////////////////////////////////////////////////
//
// Cartridge.cpp
//
// Implementation file for a cartridge.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>
//#include <File/ApiFile.hpp>

#ifdef USE_LOGGER
#include <Logger/ApiLogger.hpp>
#endif


//--------//
//
// Cartridge
//
//--------//

//--------//
// Cartridge
//
// Constructor.
//--------//
//
Cartridge::Cartridge(void)
  : mAddressStart(System::CARTRIDGE_START),
    mAddressEnd(mAddressStart + System::CARTRIDGE_SIZE)
{
}

//--------//
// Read
//
// Reads data from the system.
//
// param[in] lAddress   Address to read from.
// param[in] lLastRead  If some devices are not connected, this variable
//                      simulates "open bus behavior". Where a read of
//                      a disconnected device results in the last value read.
// returns  Data at the given address. 
//--------//
//
DataType Cartridge::Read(AddressType lAddress, DataType lLastRead)
{
    return lLastRead;
}

//--------//
// Write
//
// Writes data to memory on the system.
//
// param[in] lAddress   Address to write to. 
// param[in] lData      Data to write. 
//--------//
//
void Cartridge::Write(AddressType lAddress, DataType lData)
{
    return;
}
