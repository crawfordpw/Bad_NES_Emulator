/////////////////////////////////////////////////////////////////////
//
// Cartridge.cpp
//
// Implementation file for the ppu.
//
/////////////////////////////////////////////////////////////////////

#include <System.hpp>

#ifdef USE_LOGGER
#include <Logger/ApiLogger.hpp>
#endif

//--------//
//
// Ppu2C02
//
//--------//

//--------//
// Ppu2C02
//
// Constructor.
//--------//
//
Ppu2C02::Ppu2C02(void)
 :  mNameTable      {{NAME_TABLE_SIZE}, {NAME_TABLE_SIZE}},
    mPatternTable   {{PATTERN_TABLE_SIZE}, {PATTERN_TABLE_SIZE}}
{
}

//--------//
// Ppu2C02
//
// Destructor.
//--------//
//
Ppu2C02::~Ppu2C02(void)
{
}

//--------//
// Read
//
// Reads data from the system.
//
// param[in] lAddress   Address to read from.
// returns  Data at the given address. 
//--------//
//
DataType Ppu2C02::Read(AddressType lAddress)
{
    if (IsDisconnected())
    {
        return 0;
    }
    return mSystem->Read(lAddress);
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
void Ppu2C02::Write(AddressType lAddress, DataType lData)
{
    if (IsDisconnected())
    {
        return;
    }
    mSystem->Write(lAddress, lData);
}
